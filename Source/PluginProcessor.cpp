/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

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
    ), noteDuration{ NoteDurations() } , apvts(*this, nullptr, "apvts", createParameters())
#endif
{
    
    tremLFO.initialise([](float x) {return std::sin(x); }, 128);
    panLFO.initialise([](float x) {return std::sin(x); },128);
    filterLFO.initialise([](float x) {return std::sin(x); }, 128);
    modLFO.initialise([](float x) {return std::sin(x); }, 128);

    testLFO.setWaveType(KOLFO::WaveType::Sine);
    
    apvts.addParameterListener("TREMWAVE", this);
    apvts.addParameterListener("PANWAVE", this);
    apvts.addParameterListener("FILTERTYPE", this);
    apvts.addParameterListener("FILTERWAVE", this);

    getFilterType(false);
    shouldPrepare = false;
    apvts.state = juce::ValueTree("SavedParams");
    
}

TremoKittyAudioProcessor::~TremoKittyAudioProcessor()
{
    
}

void TremoKittyAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    DBG("Param changed");
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
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();


    gainModule.prepare(spec);
    tremLFO.prepare(spec);
    filterLFO.prepare(spec);
    panLFO.prepare(spec);
    modLFO.prepare(spec);

    testLFO.prepare(spec);

    panner.prepare(spec);
    panner.setRule(juce::dsp::PannerRule::sin3dB);

    filter.prepare(spec);
    shouldPrepare = true;
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
    //If master bypass is enabled, no processing will occur whatsoever.
    if (apvts.getRawParameterValue("MASTERBP")->load())
        return;

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    testLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, 100.f);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample <= buffer.getNumSamples(); ++sample)
        {
            float value = testLFO.getNextValue();
            DBG("TestVal = " + std::to_string(value));
        }
    }

    
    

    //My Stuff
    juce::dsp::AudioBlock<float> block(buffer);

    //Getting BPM of current project
    playHead = this->getPlayHead();
    if (playHead != nullptr)
    {
        playHead->getCurrentPosition(playheadCurrentPosition);
        auto bpm = playheadCurrentPosition.bpm;
        noteDuration.prepare(bpm);
    }
    else
    {
        noteDuration.prepare(60.f);
    }

    //Tremolo Section
    //Loading the tremolo rate and depth parameters
    float tremDepth = apvts.getRawParameterValue("TREMDEPTH")->load();
    float tremRate = apvts.getRawParameterValue("TREMRATE")->load();
    //If the tremsyncrate is free, set the frequency to whatever is stored by the slider. Else, find out what sync time it is.
    if (apvts.getRawParameterValue("TREMSYNCRATE")->load() == 0)
    {
        //setting the parameters in the tremolo LFO to match the correct ones
        tremLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, tremRate * 50);
        //DBG("Everything is happy and WONDERFul");
    }
    else
    {
        processSyncTime(modules::tremolo);
        //DBG(std::to_string(tremLFO.getFrequency()));
    }
    
    
    //Having the LFO process the gain sample, then setting the gainmodules gain to the new value given by the LFO,
    //then processing with the gain mod
    //Could literally just be 1.f
    float gain = apvts.getRawParameterValue("GAIN")->load();
    float gainMod = (tremLFO.processSample(0.f))*tremDepth;
    if ((!apvts.getRawParameterValue("TREMBP")->load()))
    {
        if(tremRate != 0.f)
            gainModule.setGainLinear(gain + gainMod);
        else
        {
            gainModule.setGainLinear(1.f);
        }
    }
    else
    {
        gainModule.setGainLinear(1.f);
    }

    gainModule.process(juce::dsp::ProcessContextReplacing<float>(block));


    //Panning section
    float panDepth = apvts.getRawParameterValue("PANDEPTH")->load();
    float panRate = apvts.getRawParameterValue("PANRATE")->load();
    panLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, panRate * 50);
    //Add panning functionality
    float newPanVal = panLFO.processSample(0.f);
    if(panDepth != 0.f)
        panner.setPan(newPanVal*panDepth);
    else
    {
        panner.setPan(0.f);
    }
    if (!apvts.getRawParameterValue("PANBP")->load())
        panner.process(juce::dsp::ProcessContextReplacing<float>(block));


    //Filter Section
    
    float filterResonance = apvts.getRawParameterValue("FILTERRES")->load();
    float filterModRate = apvts.getRawParameterValue("FILTERRATE")->load();
    float filterCutoff = apvts.getRawParameterValue("FILTERCUTOFF")->load();
    
    filterLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, filterModRate * 100);
    //Value between -1 and 1
    float filterModder = filterLFO.processSample(0.f) * apvts.getRawParameterValue("FILTERMODLEVEL")->load() * 19980;
    float filterCutoffInHertz = juce::jmap(filterCutoff, 20.f, 20000.f);
    
    float finalCutoff = (filterCutoffInHertz + filterModder);
    if (finalCutoff > 19980)
    {
        finalCutoff = 19980;
    }
    else if (finalCutoff < 21)
        finalCutoff = 21;
    filter.setCutoffFrequency(finalCutoff);
    filter.setResonance(filterResonance);

    if(!apvts.getRawParameterValue("FILTERBP")->load())
        filter.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void  TremoKittyAudioProcessor::processSyncTime(modules m)
{
    float finalFrequency;
    float syncRate;
    float syncRateMod;
    switch(m)
    {
        //If it's tremolo...
    case(modules::tremolo):
        syncRate = apvts.getRawParameterValue("TREMSYNCRATE")->load();
        syncRateMod = apvts.getRawParameterValue("TREMSYNCMOD")->load();
        //If its a straight note...
        if (syncRateMod == 0)
        {
            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getSixteenthNote();
                break;
            default:
                DBG("Couldn't Properly Assign Sync Time Tremolo");
                break;
            }
        }
        //If its a dotted note...
        else if (syncRateMod == 1)
        {
            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getDottedHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getDottedQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getDottedEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getDottedSixteenthNote();
                break;
            default:
                DBG("Couldn't select dotted sync time tremolo");
                break;
            }
        }
        //If it's a triplet...
        else
        {
            
            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getTripletQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getTripletEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getTripletSixteenthNote();
                break;
            default:
                DBG("Couldn't set triplet sync rate tremolo");
                break;
            }
        }
        tremLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, finalFrequency*163.75f);
        break;
    case(modules::pan):
        syncRate = apvts.getRawParameterValue("PANSYNCRATE")->load();
        syncRateMod = apvts.getRawParameterValue("PANSYNCMOD")->load();
        //If its a straight note...
        if (syncRateMod == 0)
        {
            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getSixteenthNote();
                break;
            default:
                DBG("Couldn't Properly Assign Sync Time Pan");
                break;
            }
        }
        //If its a dotted note...
        else if (syncRateMod == 1)
        {
            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getDottedHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getDottedQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getDottedEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getDottedSixteenthNote();
                break;
            default:
                DBG("Couldn't select dotted sync time Pan");
                break;
            }
        }
        //If it's a triplet...
        else
        {

            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getTripletQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getTripletEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getTripletSixteenthNote();
                break;
            default:
                DBG("Couldn't set triplet sync rate Pan");
                break;
            }
        }
        panLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, finalFrequency);
        break;
    case(modules::filter):
        syncRate = apvts.getRawParameterValue("FILTERMODSYNCRATE")->load();
        syncRateMod = apvts.getRawParameterValue("FILTERMODSYNCMOD")->load();
        //If its a straight note...
        if (syncRateMod == 0)
        {
            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getSixteenthNote();
                break;
            default:
                DBG("Couldn't Properly Assign Sync Time Filter");
                break;
            }
        }
        //If its a dotted note...
        else if (syncRateMod == 1)
        {
            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getDottedHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getDottedQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getDottedEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getDottedSixteenthNote();
                break;
            default:
                DBG("Couldn't select dotted sync time Filter");
                break;
            }
        }
        //If it's a triplet...
        else
        {

            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getTripletQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getTripletEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getTripletSixteenthNote();
                break;
            default:
                DBG("Couldn't set triplet sync rate Filter");
                break;
            }
        }
        filterLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, finalFrequency);
        break;
    case(modules::mod):
        syncRate = apvts.getRawParameterValue("LFOSYNCRATE")->load();
        syncRateMod = apvts.getRawParameterValue("LFOSYNCMOD")->load();
        //If its a straight note...
        if (syncRateMod == 0)
        {
            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getSixteenthNote();
                break;
            default:
                DBG("Couldn't Properly Assign Sync Time mod LFO");
                break;
            }
        }
        //If its a dotted note...
        else if (syncRateMod == 1)
        {
            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getDottedHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getDottedQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getDottedEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getDottedSixteenthNote();
                break;
            default:
                DBG("Couldn't select dotted sync time mod LFO");
                break;
            }
        }
        //If it's a triplet...
        else
        {

            switch ((int)syncRate)
            {
            case(1):
                finalFrequency = noteDuration.getWholeNote();
                break;
            case(2):
                finalFrequency = noteDuration.getHalfNote();
                break;
            case(3):
                finalFrequency = noteDuration.getTripletQuarterNote();
                break;
            case(4):
                finalFrequency = noteDuration.getTripletEighthNote();
                break;
            case(5):
                finalFrequency = noteDuration.getTripletSixteenthNote();
                break;
            default:
                DBG("Couldn't set triplet sync rate mod LFO");
                break;
            }
        }
        modLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, finalFrequency*120.f);
        break;
    default:
        DBG("Something went horribly wrong in the big function. Good luck");
    }

}

//==============================================================================
bool TremoKittyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TremoKittyAudioProcessor::createEditor()
{
    return new TremoKittyAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

void TremoKittyAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    std::unique_ptr<juce::XmlElement> xml(apvts.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TremoKittyAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlParams(getXmlFromBinary(data, sizeInBytes));
    if (xmlParams != nullptr)
    {
        if (xmlParams->hasTagName(apvts.state.getType()))
        {
            apvts.state = juce::ValueTree::fromXml(*xmlParams);
        }
    }

}

//Will load a preset.
void TremoKittyAudioProcessor::loadPreset(const juce::String& name)
{

}

//Resets every parameter back to it's default.
void TremoKittyAudioProcessor::resetEverything()
{
    juce::ValueTree state = apvts.copyState();  // grab a copy of the current parameters Value Tree
    std::unique_ptr<juce::XmlElement> tempXml(state.createXml());  // convert parameters Value Tree to an XML object

    //Iterate through elements in the XML with the tag of param, set their values back to default
    for (auto* element : tempXml->getChildWithTagNameIterator("PARAM"))
    {
        try
        {
            if (apvts.getParameter(element->getStringAttribute("id")) != nullptr)
            {
                float defaultValue = apvts.getParameter(element->getStringAttribute("id"))->getDefaultValue();
                element->setAttribute("value", defaultValue);
            }
        }
        catch (...)
        {
            DBG("Something fucke up");
        }
        
    }

    //Deprecated method
    /*forEachXmlChildElementWithTagName(*tempXml, child, "PARAM")
    {
        float defaultValue = apvts.getParameter(child->getStringAttribute("id"))->getDefaultValue();
        child->setAttribute("value", defaultValue);
    }*/
    apvts.replaceState(juce::ValueTree::fromXml(*tempXml));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TremoKittyAudioProcessor();
}

//Creates the parameters for the Audio Processor Value Tree State.
juce::AudioProcessorValueTreeState::ParameterLayout  TremoKittyAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    //Misc
    layout.add(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.f, 1.f, 1.f));
    layout.add(std::make_unique < juce::AudioParameterBool>("MASTERBP", "Master Bypass", false));


    //Tremolo Section
    layout.add(std::make_unique<juce::AudioParameterFloat>("TREMRATE", "Tremolo Rate", 0.f, 20.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("TREMDEPTH", "Tremolo Depth", 0.f, 1.f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("TREMWAVE", "Tremolo Modulation Waveform", juce::StringArray("Sine", "Saw", "Square"), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("TREMSYNCRATE", "Tremolo Tempo Sync", noteDuration.getStraightNoteTypes(), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("TREMSYNCMOD", "Tremolo Sync Rate Mod", juce::StringArray("Straight", "Dotted", "Triplet"), 0));
    layout.add(std::make_unique < juce::AudioParameterBool>("TREMBP", "Tremolo Bypass", false));

    
    //Panner section
    layout.add(std::make_unique<juce::AudioParameterFloat>("PANRATE", "Pan Rate", 0.f, 20.f, 7.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PANDEPTH", "Pan Depth", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("PANWAVE", "Pan Mod Waveform", juce::StringArray("Sine", "Saw", "Square"), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("PANSYNCRATE", "Pan Modulation Tempo Sync Rate", noteDuration.getStraightNoteTypes(), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("PANSYNCMOD", "Pan Mod Sync Mod", juce::StringArray("Straight", "Dotted", "Triplet"), 0));
    layout.add(std::make_unique < juce::AudioParameterBool>("PANBP", "Pan Bypass", false));

    //Filter section
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERRATE", "Filter Rate", 0.f, 20.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERMODLEVEL", "Filter Mod Level", juce::NormalisableRange<float>(0.f, 1.f, 0.001f, 0.35), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float>(0.f, 1.f, 0.000001, 0.25), 0.9f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float>(0.7f, 10.f, 0.05, 0.9), (1 / sqrt(2))));
    layout.add(std::make_unique<juce::AudioParameterChoice>("FILTERWAVE", "Filter Mod Waveform", juce::StringArray("Sine", "Saw", "Square"), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray("Low Pass", "High Pass", "Band Pass"), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("FILTERMODSYNCRATE", "Filter Modulation Tempo Snyc", noteDuration.getStraightNoteTypes(), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("FILTERMODSYNCMOD", "Filter Modulation Tempo Sync Mod", juce::StringArray("Dotted", "Triplet"), 0));
    layout.add(std::make_unique<juce::AudioParameterBool>("FILTERBP", "Filter Bypass", false));

    //ModLFO Section
    layout.add(std::make_unique<juce::AudioParameterFloat>("MODLFORATE", "Mod LFO Rate", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("MODLFODEPTH", "Mod LFO Depth", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("MODLFOWAVETYPE", "Mod LFO Wave Type", juce::StringArray("Sine", "Saw", "Square"), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("MODLFOSYNCRATE", "Mod LFO Sync Rate", noteDuration.getStraightNoteTypes(), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("MODLFOSYNCMOD", "Mod LFO Sync Rate Mod", juce::StringArray("Straight", "Dotted", "Triplet"), 0));
    //layout.add(std::make_unique<juce::AudioParameter> moddedStuff

    //Returning every parameter
    return layout;
}

//Sets the appropriate wave form for the modulation of the given module.
void TremoKittyAudioProcessor::getWave(modules module)
{
    int index;
    switch (module)
    {
    case(modules::tremolo):
       index = apvts.getRawParameterValue("TREMWAVE")->load();
        switch (index)
        {
        case 0:
            tremLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSine);
            apvts.getParameter("TREMWAVE")->setValue(0);
            break;
        case 1:
            tremLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSaw);
            apvts.getParameter("TREMWAVE")->setValue(1);
            break;
        case 2:
            tremLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSquare);
            apvts.getParameter("TREMWAVE")->setValue(2);
            break;
        default:
            DBG("There was an issue with reassigning wavetype");
        }
    case(modules::pan):
        index = apvts.getRawParameterValue("PANWAVE")->load();
        switch (index)
        {
        case 0:
            panLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSine);
            apvts.getParameter("PANWAVE")->setValue(0);
            break;
        case 1:
            panLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSaw);
            apvts.getParameter("PANWAVE")->setValue(1);
            break;
        case 2:
            panLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSquare);
            apvts.getParameter("PANWAVE")->setValue(2);
            break;
        default:
            DBG("There was an issue with reassigning wavetype");
        }
    case(modules::filter):
        index = apvts.getRawParameterValue("FILTERWAVE")->load();
        switch (index)
        {
        case 0:
            filterLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSine);
            apvts.getParameter("FILTERWAVE")->setValue(0);
            break;
        case 1:
            filterLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSaw);
            apvts.getParameter("FILTERWAVE")->setValue(1);
            break;
        case 2:
            filterLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSquare);
            apvts.getParameter("FILTERWAVE")->setValue(2);
            break;
        default:
            DBG("There was an issue with reassigning wavetype");
        }
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
        DBG("WE PREPARED AHAHHAH");
        filter.reset();
        filter.prepare(spec);
     }
}

