/*
  ==============================================================================

    This file contains the innards of our DSP and stuff

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TremoKittyAudioProcessor::TremoKittyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), apvts(*this, nullptr, "apvts", createParameters())
#endif
{
    playHead = nullptr;

    tremLFO.setWaveType(KOLFO::WaveType::Sine);
    panLFO.setWaveType(KOLFO::WaveType::Sine);
    filterLFO.setWaveType(KOLFO::WaveType::Sine);
    modLFO.setWaveType(KOLFO::WaveType::Sine);

    LFOList.push_back(tremLFO);
    LFOList.push_back(panLFO);
    LFOList.push_back(filterLFO);
    LFOList.push_back(modLFO);
    
    //Adding listeners to each of the modable parameters- see the enumerator ModParams
  /*  for (int i = 1; i < 7; i++)
    {
        juce::String paramID = ModParams[i];
        apvts.addParameterListener(paramID, this);
    }*/

    addListenersToAllParameters();

    getFilterType(false);
    shouldPrepare = false;
    presetManager = std::make_unique<Service::PresetManager>(apvts);


    auto options = juce::PropertiesFile::Options();
    options.applicationName = ProjectInfo::projectName;
    options.commonToAllUsers = true;
    options.filenameSuffix = ".kprpt";
    options.folderName = ProjectInfo::projectName;
    options.osxLibrarySubFolder = "Application Support";
    globalProperties.setStorageParameters(options);

    auto userSettings = globalProperties.getUserSettings();
    auto display =  userSettings->getBoolValue("DONTDISPLAYKITTY");
    auto skin = userSettings->getIntValue("DEFAULTSKIN");

    PresetNames = presetManager->getAllPresets();
}

TremoKittyAudioProcessor::~TremoKittyAudioProcessor()
{
    for (const auto& paramID : registeredParamIDs)
    {
        apvts.removeParameterListener(paramID, this);
    }

    registeredParamIDs.clear();
}

void TremoKittyAudioProcessor::addListenersToAllParameters()
{
    registeredParamIDs.clear();

    for (auto* p : getParameters())
    {
        if (auto* ranged = dynamic_cast<juce::RangedAudioParameter*>(p))
        {
            const auto id = ranged->paramID;

            apvts.addParameterListener(id, this);
            registeredParamIDs.addIfNotAlreadyThere(id);
        }
    }
}

//When a parameter changes, this one will run, figure out which parameter was changed, and do something accordingly.
void TremoKittyAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "TREMWAVE")
    {
        getWave(modules::tremolo);
    }
    else if (parameterID == "PANWAVE")
    {
        getWave(modules::pan);
    }
    else if (parameterID == "FILTERWAVE")
    {
        getWave(modules::filter);
    }
    else if (parameterID == "FILTERTYPE")
    {
        getFilterType(shouldPrepare);
    }
    else if (parameterID == "MODWAVETYPE")
    {
        getWave(modules::mod);
    }
    else if (parameterID == "MODCHOICE")
    {
        switchProcessMod();
    }
    else if (parameterID == "TREMSYNCCHOICE" || (parameterID == "TREMSYNC" && newValue == 1.f))
    {
        resetLFOPhase(tremLFO, "TREMSYNCCHOICE");
    }
    else if (parameterID == "PANSYNCCHOICE" || (parameterID == "PANSYNC" && newValue == 1.f))
    {
        resetLFOPhase(panLFO, "PANSYNCCHOICE");
    }
    else if (parameterID == "FILTERSYNCCHOICE" || (parameterID == "FILTERSYNC" && newValue == 1.f))
    {
        resetLFOPhase(filterLFO, "FILTERSYNCCHOICE");
    }
    else if (parameterID == "MODSYNCCHOICE" || (parameterID == "MODSYNC" && newValue == 1.f))
    {
        resetLFOPhase(modLFO, "MODSYNCCHOICE");
    }
    else if (parameterID == "TREMBP")
    {
        if (apvts.getRawParameterValue("TREMSYNC")->load() && newValue == 0.f)
        {
            resetLFOPhase(tremLFO, "TREMSYNCCHOICE");
        }
    }
    else if (parameterID == "PANBP")
    {
        if (apvts.getRawParameterValue("PANSYNC")->load() && newValue == 0.f)
        {
            resetLFOPhase(panLFO, "PANSYNCCHOICE");
        }
    }
    else if (parameterID == "FILTERBP")
    {
        if (apvts.getRawParameterValue("FILTERSYNC")->load() && newValue == 0.f)
        {
            resetLFOPhase(filterLFO, "FILTERSYNCCHOICE");
        }
    }
    else if (parameterID == "MODBP")
    {
        if (apvts.getRawParameterValue("MODSYNC")->load() && newValue == 0.f)
        {
            resetLFOPhase(modLFO, "MODSYNCCHOICE");
        }
    }
    else
    {
        //Updates the Modded parameter if the changed parameter was indeed the modded parameter :)
        if (parameterID == ModParams[apvts.getRawParameterValue("MODCHOICE")->load()])
        {
            updateModParam(newValue);
        }
    }
}

//==============================================================================
const juce::String TremoKittyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TremoKittyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TremoKittyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TremoKittyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TremoKittyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TremoKittyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TremoKittyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TremoKittyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TremoKittyAudioProcessor::getProgramName (int index)
{
    return {};
}

void TremoKittyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TremoKittyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    if (!hasRestoredState && !hasLoadedInitialPreset && presetManager != nullptr)
    {
        presetManager->loadPreset("Default");
        hasLoadedInitialPreset = true;
    }

    //Set up spec
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    //Allocating memory for our lookuptable vector
    lfoLookupTable.resize(samplesPerBlock);

    //Set up lfotempo object
    tempo.setSampleRate(sampleRate);

    //Set up LFOs
    tremLFO.prepare(sampleRate);
    panLFO.prepare(sampleRate);
    filterLFO.prepare(sampleRate);
    modLFO.prepare(sampleRate);

    //Set up gain module
    gainModule.prepare(spec);

    //The filter for our tremolo LFO
    gainModFilter.prepare(spec);
    gainModFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    gainModFilter.setCutoffFrequency(150.f);

    //Pan Module
    panner.prepare(spec);
    panner.setRule(juce::dsp::PannerRule::sin3dB);

    //Filter Module
    filter.prepare(spec);
    shouldPrepare = true;
    filterLFOFilter.prepare(spec);
    filterLFOFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    //Doesn't seem to be really necessary
    filterLFOFilter.setCutoffFrequency(3000.f);
}

void TremoKittyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TremoKittyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TremoKittyAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    /*My Stuff Starts Here*/
    juce::dsp::AudioBlock<float> block(buffer);

    if (bypassed)
    {
        bypassed = false;
    }

    //Set up the Tempo obj, will only get set up when playhead is available ie in a daw
    playHead = this->getPlayHead();
    if (playHead)
    {
        //Set currentPosition to the playheads CurrentPosition info and set bpm
        tempo.setBPM(currentPosition.bpm);
        playHead->getCurrentPosition(currentPosition);

        //If audio is playing and the playback stopped field is equal to true, indicating that we have not called the playback start method yet
        if (currentPosition.isPlaying && playbackStopped)
            playbackStart(buffer.getNumSamples());
        //If audio is not playing and playback stopped field is false, meaning playback has stopped but we haven't called the playback stop function yet
        else if (!currentPosition.isPlaying && !playbackStopped)
            playbackStop();

        //If were in an unexpected position, from either effect bypassed or playhead moving around, reset all LFO phases.
        if (currentPosition.isPlaying && currentPosition.timeInSamples != nextExpectedPlaybackSample)
        {
            resetAllLFOPhases();
        }
         nextExpectedPlaybackSample = currentPosition.timeInSamples + buffer.getNumSamples();
    }
    else
        tempo.setBPM(120.f);
        
    
    //The Mod LFO section is processed first, as it will affect the value of the others.
    int modChoice = static_cast<int>(apvts.getRawParameterValue("MODCHOICE")->load());
    if (modChoice != 0)
    {
        //Calls the function with the name of the thing you want to actually modify
        processMod(ModParams[modChoice], block.getNumSamples());
    }

    //Tremolo Section
    //Loading the tremolo rate and depth parameters
    if ((!apvts.getRawParameterValue("TREMBP")->load()))
    {
        float tremDepth = apvts.getRawParameterValue("TREMDEPTH")->load();
        float tremRate;
        if (apvts.getRawParameterValue("TREMSYNC")->load())
        {
            int option = apvts.getRawParameterValue("TREMSYNCCHOICE")->load();
            tremRate = tempo.getNoteLengthHertz(static_cast<KOTempo::NoteTypes>(option));
        }
        else
        {
            tremRate = apvts.getRawParameterValue("TREMRATE")->load();
        }

        if (tremRate != 0.f || (ModParams[apvts.getRawParameterValue("MODCHOICE")->load()] == "TREMRATE"))
        {
            tremLFO.setFrequency(tremRate);
            //Having the LFO process the gain sample, then setting the gainmodules gain to the 
            // new value given by the LFO,
            //then processing with the gain mod
            //Could literally just be 1.f
            float gain = apvts.getRawParameterValue("GAIN")->load();
            float f{0};
            float tremscaler = 1 - tremDepth;

            tremLFOCurrentPosition.store(tremLFO.peekNextValue());
            for (int samples = 0; samples < buffer.getNumSamples(); ++samples)
            {
                f = (tremLFO.getNextValue() + 1) * 0.5f;
                lfoLookupTable[samples] = tremscaler + gainModFilter.processSample(0, f) * tremDepth;
            }

            for (int channel = 0; channel < totalNumInputChannels; ++channel)
            {
                float* channelData = buffer.getWritePointer(channel);

                for (int samples = 0; samples < buffer.getNumSamples(); ++samples)
                {
                    channelData[samples] *= lfoLookupTable[samples];
                }
            }
        } // BIG IF
    }//if trembp is false
    else
    {
        gainModule.setGainLinear(1.f);
        gainModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    }

    //==Panning section==
    //Setting the pan rate
    float panDepth = apvts.getRawParameterValue("PANDEPTH")->load();
    float panRate = 1;
    if (apvts.getRawParameterValue("PANSYNC")->load())
    {
        int option = apvts.getRawParameterValue("PANSYNCCHOICE")->load();
        panRate = tempo.getNoteLengthHertz(static_cast<KOTempo::NoteTypes>(option));
    }
    else
        panRate = apvts.getRawParameterValue("PANRATE")->load();

    panLFO.setFrequency(panRate);

    float newPanVal = 0;

    //Add panning functionality
    if (panDepth != 0.f)
    {
        panLFOCurrentPosition.store( panLFO.peekNextValue(block.getNumSamples()));
        newPanVal = panLFO.getNextValue(block.getNumSamples());
        panner.setPan(newPanVal * panDepth);
    }
    else
    {
        panner.setPan(0.f);
    }

    if (!apvts.getRawParameterValue("PANBP")->load())
        panner.process(juce::dsp::ProcessContextReplacing<float>(block));

    //Filter Section
    if (!apvts.getRawParameterValue("FILTERBP")->load())
    {
        float filterResonance = apvts.getRawParameterValue("FILTERRES")->load();
        float filterModLevel = apvts.getRawParameterValue("FILTERMODLEVEL")->load();
        float filterCutoff = apvts.getRawParameterValue("FILTERCUTOFF")->load();
        float filterCutoffInHertz = juce::jmap(filterCutoff, 20.f, 20000.f);

        filter.setResonance(filterResonance);

        float filterModRate = 1;
        if (apvts.getRawParameterValue("FILTERSYNC")->load())
        {
            int option = apvts.getRawParameterValue("FILTERSYNCCHOICE")->load();
            filterModRate = tempo.getNoteLengthHertz(static_cast<KOTempo::NoteTypes>(option));
        }
        else
            filterModRate = apvts.getRawParameterValue("FILTERRATE")->load();

        filterLFO.setFrequency(filterModRate);

        if (filterModLevel > 0)
        {
            //Value between -1 and 1
            float nextLFOValue = filterLFO.getNextValue(block.getNumSamples());
            float lfoResult = filterLFOFilter.processSample(0, nextLFOValue);
            float filterModder =  lfoResult * filterModLevel * 19980;

            filterLFOCurrentPosition.store(nextLFOValue);
            
            float finalCutoff = (filterCutoffInHertz + filterModder);

            if (finalCutoff > 20000)
            {
                
                finalCutoff = 20000;
            }
            else if (finalCutoff < 20)
                finalCutoff = 20;
            filter.setCutoffFrequency(finalCutoff);
        }
        else
        {
            filter.setCutoffFrequency(filterCutoffInHertz);
        }
        filter.process(juce::dsp::ProcessContextReplacing<float>(block));
    }
}

void TremoKittyAudioProcessor::processBlockBypassed(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages)
{
    if (!bypassed)
    {
        bypassed = true;
        playbackStop();
    }
}

//Will advance synced LFOs to their proper starting position given the current position info, or just reset them regularly.
void TremoKittyAudioProcessor::playbackStart(const int bufferSamples)
{
    nextExpectedPlaybackSample = currentPosition.timeInSamples+bufferSamples;

    resetAllLFOPhases();

    playbackStopped = false;
}

void TremoKittyAudioProcessor::resetLFOPhase(KOLFO& LFO, const juce::String& parameterID)
{
    if (playHead == nullptr)
        return;

    //The number of quarter notes since the last bar started
    double currentPhase = currentPosition.ppqPosition - currentPosition.ppqPositionOfLastBarStart;
    //Convert to samples
    double quarterNoteSamples = tempo.getNoteLengthSamples(KOTempo::NoteTypes::Quarter);

    //The number of samples that the currentPosition is past the last bar
    double currentPhaseSamples = currentPhase * quarterNoteSamples;

    LFO.reset();

    //Find how many samples the note denomination is
    double syncChoiceSamples = tempo.getNoteLengthSamples(KOTempo::NoteTypes(static_cast<int>(apvts.getRawParameterValue(parameterID)->load())));
    double remainder = (static_cast<std::uint32_t>(currentPhaseSamples) % static_cast<std::uint32_t>(syncChoiceSamples));

    double ratio = remainder / syncChoiceSamples;

    float increment = juce::MathConstants<float>::twoPi * ratio;

    LFO.advancePhase(increment);
}

void TremoKittyAudioProcessor::resetAllLFOPhases()
{
    if (apvts.getRawParameterValue("TREMSYNC")->load())
    {
        resetLFOPhase(tremLFO, "TREMSYNCCHOICE");
    }
    if (apvts.getRawParameterValue("PANSYNC")->load())
    {
        resetLFOPhase(panLFO, "PANSYNCCHOICE");
    }
    if (apvts.getRawParameterValue("FILTERSYNC")->load())
    {
        resetLFOPhase(filterLFO, "FILTERSYNCCHOICE");
    }
    if (apvts.getRawParameterValue("MODSYNC")->load())
    {
        resetLFOPhase(modLFO, "MODSYNCCHOICE");
    }
}

void TremoKittyAudioProcessor::playbackStop()
{
    playbackStopped = true;
}

void TremoKittyAudioProcessor::processMod(const juce::String& parameterID, float bufferSize)
{
    //Set up tempo sync
    float modFrequency;
    if (apvts.getRawParameterValue("MODSYNC")->load())
    {
        int option = apvts.getRawParameterValue("MODSYNCCHOICE")->load();
        modFrequency = tempo.getNoteLengthHertz(static_cast<KOTempo::NoteTypes>(option));
    }
    else
        modFrequency = apvts.getRawParameterValue("MODLFORATE")->load();
    
    modLFO.setFrequency(modFrequency);
  
    //Assign depth
    float modDepth;
    if (modFrequency == 0)
        modDepth = 0.f;
    else
        modDepth = apvts.getRawParameterValue("MODLFODEPTH")->load();
    
    //Determining the max value of our modded parameter based on whether it's a rate or depth parameter. (Rates are always 0-10. depth is 0-1).
    float paramMaxValue;
    if (parameterID.contains("TREMRATE"))
    {
        paramMaxValue = 20.f;
    }
    else if (parameterID.contains("RATE"))
    {
        paramMaxValue = 10.f;
    }
    else
    {
        paramMaxValue = 1.f;
    }

    //Saving the value before mod of our nice little eensie weensie mod value for later so we can use it and shit
    //Will only save the first time, wont save again until process mod is stopped (when the mod parameter is changed to None)
    bool setDefaultParam = apvts.getRawParameterValue("MODRESETSWITCH")->load();
    if (setDefaultParam)
    {
        auto oldVal = apvts.getRawParameterValue(parameterID)->load();
        apvts.getRawParameterValue("MODPARAMPRIORVALUE")->store(oldVal);
        apvts.getRawParameterValue("MODRESETSWITCH")->store(0.f);
        //Should store the index of the last modded parameter
        apvts.getRawParameterValue("LASTMODDEDPARAM")->store(ModParams.indexOf(parameterID));
    }
    
    //Mod scaler is a value between 1 and 0, times the modDepth of 0 to 1.
    float modScaler = ((modLFO.getNextValue(bufferSize) + 1) * 0.5);
    auto oldValue = apvts.getRawParameterValue("MODPARAMPRIORVALUE")->load();
    if (paramMaxValue > 5.f)
    {
        //if oldvalue is 10, the LFO will be mapped from 0-1 to -10 to 10. I.E if the LFO gives 1, the modscaler gives 10. If the LFO gives 0, the modscaler gives -10.
        modScaler = juce::jmap(modScaler, 0 - oldValue, 10 - oldValue);
    }
    else
    {
        //If the oldvalue is 0.5, the lfo will be mapped between 0-1 to -0.5 to 0.5. Therefore if the LFO gives 1, the modscaler gives 0.5. so our original value 0.5+ the lfo mod 0.5 will put us up to 1, the max value. 
        modScaler = juce::jmap(modScaler, 0 - oldValue, 1 - oldValue);
    }
    //Storing the oldvalue + the modscaler * modDepth. Lets do the math
    /*values between 0 and 10:
    * say the original value is 7. The modscaler will be 0-1 mapped out between 0 minus 7 and 10 minus 7, in other words -7 and 13. Lets say the LFO is giving us a value of 1.0, so the scaler is at the max value. the modscaler will give us 13. Then when we add that to our original value, 7+13 = 10, therefore the max from the LFO will give us the max of the actual value. This is only if the moddepth is fully engaged. If the moddepth is at, say, 0.5, then the range of the modscaler collapses from -7 to 13 to -3.5 to 6.5. This way, the signal is only being modulated from a range that goes from the original value to halfway down to 0, and halfway to the max. In other words it modulates 7 down to 3.5, up to 13.5. And of course, if mod depth is 0, then 0 will be added to the oldvalue, so the parameter will not be changed at all.
    * The math is pretty much the same for the 0-1 values. Just shrink the formula down.
    */
    if(!apvts.getRawParameterValue("MODBP")->load())
        apvts.getRawParameterValue(parameterID)->store(oldValue + (modScaler*modDepth));
    else
    {
        apvts.getRawParameterValue(parameterID)->store(oldValue);
    }
}

//Changes the parameter that is being modded, called when the mod param option is changed.
void TremoKittyAudioProcessor::switchProcessMod()
{
    int oldParamIndex = apvts.getRawParameterValue("LASTMODDEDPARAM")->load();
    juce::String ParameterID = ModParams[oldParamIndex];
    //sets the modded parameter id back to its original value
    if (ParameterID != "None")
    {
        float oldValue = apvts.getRawParameterValue("MODPARAMPRIORVALUE")->load();
        apvts.getRawParameterValue(ParameterID)->store(oldValue);
    }
    apvts.getRawParameterValue("MODRESETSWITCH")->store(true);
}

void TremoKittyAudioProcessor::updateModParam(float newValue)
{
    apvts.getRawParameterValue("MODPARAMPRIORVALUE")->store(newValue);
}

//==============================================================================
bool TremoKittyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TremoKittyAudioProcessor::createEditor()
{
    return new kitty_editor::TremoKittyAudioProcessorEditor (*this);
}

void TremoKittyAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();

    if (presetManager != nullptr)
        state.setProperty("presetName", presetManager->getCurrentPreset(), nullptr);

    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TremoKittyAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr)
    {
        auto state = juce::ValueTree::fromXml(*xml);

        if (state.isValid())
        {
            apvts.replaceState(state);
            auto presetName = state.getProperty("presetName").toString();

            if (presetManager != nullptr)
                presetManager->setCurrentPresetName(presetName);

            hasRestoredState = true;
        }
    }
}

void TremoKittyAudioProcessor::changeTremWaveManually(int index)
{
    apvts.getRawParameterValue("TREMWAVE")->store((float)index);
    getWave(modules::tremolo);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TremoKittyAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout
TremoKittyAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // =====================
    // Misc
    // =====================
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("GAIN", 1), "Gain",
        0.f, 1.f, 1.f));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("MASTERBP", 1), "Master Bypass",
        false));

    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID("PRESETINDEX", 1),
        "Preset Index",
        0,
        100000,
        0,
        juce::AudioParameterIntAttributes()
        .withAutomatable(false)
    ));

    // =====================
    // Tremolo Section
    // =====================
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("TREMRATE", 1), "Tremolo Rate",
        juce::NormalisableRange<float>(0.f, 20.f, 0.01f, 0.5f),
        0.1f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("TREMDEPTH", 1), "Tremolo Depth",
        0.f, 1.f, 0.5f));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("TREMWAVE", 1), "Tremolo Modulation Waveform",
        juce::StringArray(WAVE_TYPES), 0));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("TREMSYNCCHOICE", 1), "Tremolo Sync Rate Choice",
        KOTempo::getNoteTypesAlternative(), 3));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("TREMBP", 1), "Tremolo Bypass",
        false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("TREMSYNC", 1), "Tremolo Sync",
        false));

    // =====================
    // Panner Section
    // =====================
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("PANRATE", 1), "Pan Rate",
        juce::NormalisableRange<float>(0.f, 10.f, 0.01f, 0.5f),
        7.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("PANDEPTH", 1), "Pan Depth",
        0.f, 1.f, 0.f));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("PANWAVE", 1), "Pan Mod Waveform",
        juce::StringArray(WAVE_TYPES), 0));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("PANSYNCCHOICE", 1), "Pan Sync Rate Choice",
        KOTempo::getNoteTypesAlternative(), 3));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("PANBP", 1), "Pan Bypass",
        false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("PANSYNC", 1), "Pan Sync",
        false));

    // =====================
    // Filter Section
    // =====================
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("FILTERRATE", 1), "Filter Rate",
        juce::NormalisableRange<float>(0.f, 10.f, 0.01f, 0.5f),
        0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("FILTERMODLEVEL", 1), "Filter Mod Level",
        juce::NormalisableRange<float>(0.f, 1.f, 0.001f, 0.35f),
        0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("FILTERCUTOFF", 1), "Filter Cutoff",
        juce::NormalisableRange<float>(0.f, 1.f, 0.00001f, 0.35f),
        0.9f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("FILTERRES", 1), "Filter Resonance",
        juce::NormalisableRange<float>(0.7f, 10.f, 0.05f, 0.9f),
        1.0f / std::sqrt(2.0f)));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("FILTERWAVE", 1), "Filter Mod Waveform",
        juce::StringArray(WAVE_TYPES), 0));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("FILTERTYPE", 1), "Filter Type",
        juce::StringArray("Low Pass", "High Pass", "Band Pass"), 0));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("FILTERSYNCCHOICE", 1), "Filter Sync Rate Choice",
        KOTempo::getNoteTypesAlternative(), 3));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("FILTERBP", 1), "Filter Bypass",
        false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("FILTERSYNC", 1), "Filter Sync",
        false));

    // =====================
    // Mod LFO Section
    // =====================
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("MODLFORATE", 1), "Mod LFO Rate",
        juce::NormalisableRange<float>(0.f, 10.f, 0.01f, 0.5f),
        0.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("MODLFODEPTH", 1), "Mod LFO Depth",
        0.f, 1.f, 0.f));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("MODWAVETYPE", 1), "Mod LFO Wave Type",
        juce::StringArray(WAVE_TYPES), 0));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("MODCHOICE", 1), "Mod LFO Parameter Choice",
        juce::StringArray("None", "TREMRATE", "TREMDEPTH",
                          "PANRATE", "PANDEPTH",
                          "FILTERRATE", "FILTERMODLEVEL"),
        0));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("LASTMODDEDPARAM", 1),
        "The String Name of the last parameter that was modded",
        juce::StringArray("None", "Trem Rate", "Trem Depth",
                          "Pan Rate", "Pan Depth",
                          "Filter Mod Rate", "Filter Mod Depth"),
        0));

    layout.add(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("MODSYNCCHOICE", 1), "Mod Sync Rate Choice",
        KOTempo::getNoteTypesAlternative(), 3));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("MODPARAMPRIORVALUE", 1),
        "Modded Parameter Pre-modded Value",
        0.f, 10.f, 0.f));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("MODRESETSWITCH", 1), "Modded Param Reset Switch",
        true));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("MODBP", 1), "Mod LFO Bypass",
        false));

    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("MODSYNC", 1), "Mod Sync",
        false));

    return layout;
}

//Sets the appropriate wave form for the modulation of the given module.
void TremoKittyAudioProcessor::getWave(modules module)
{
    int index;
    int i = 0;
    int size = juce::StringArray(WAVE_TYPES).size();
    
    switch (module)
    {
    case(modules::tremolo):
        index = apvts.getRawParameterValue("TREMWAVE")->load();
        for (i = 0; i < size; i++)
        {
            if (index == i)
            {
                tremLFO.setWaveType(KOLFO::WaveType(i));
                apvts.getParameter("TREMWAVE")->setValue(i);
            }
        }
        break;
    case(modules::pan):
        index = apvts.getRawParameterValue("PANWAVE")->load();
        for (i = 0; i < size; i++)
        {
            if (index == i)
            {
                panLFO.setWaveType(KOLFO::WaveType(i));
                apvts.getParameter("PANWAVE")->setValue(i);
            }
        }
        break;
    case(modules::filter):
        index = apvts.getRawParameterValue("FILTERWAVE")->load();
        for (i = 0; i < size; i++)
        {
            if (index == i)
            {
                filterLFO.setWaveType(KOLFO::WaveType(i));
                apvts.getParameter("FILTERWAVE")->setValue(i);
            }
        }
        break;
    case(modules::mod):
        index = apvts.getRawParameterValue("MODWAVETYPE")->load();
        for (i = 0; i < size; i++)
        {
            if (index == i)
            {
                modLFO.setWaveType(KOLFO::WaveType(i));
                apvts.getParameter("MODWAVETYPE")->setValue(i);
            }
        }
        break;
    }
}

//Sets the filtertype, and will prepare the module if appropriate i.e. spec is already properly initialized.
void TremoKittyAudioProcessor::getFilterType(bool shouldPrepare)
{
    //Loads up the value of the Filter Type Parameter
    auto  Choice = apvts.getRawParameterValue("FILTERTYPE")->load();
    int index = Choice;
    switch (index)
    {
    case(0):
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;
    case(1):
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;
    case(2):
        filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        break;
    default:
        DBG("Failed to change filter type.");
    }
    if (shouldPrepare)
    {
        filter.reset();
        filter.prepare(spec);
     }
}
