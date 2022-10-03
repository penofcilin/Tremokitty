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
    ), apvts(*this, nullptr, "apvts", createParameters())
#endif
{
    
    tremLFO.initialise([](float x) {return std::sin(x); }, 128);
    panLFO.initialise([](float x) {return std::sin(x); },128);
    filterLFO.initialise([](float x) {return std::sin(x); }, 128);
    modLFO.initialise([](float x) {return std::sin(x); }, 128);
    
    apvts.addParameterListener("TREMWAVE", this);
    apvts.addParameterListener("PANWAVE", this);
    apvts.addParameterListener("FILTERTYPE", this);
    apvts.addParameterListener("FILTERWAVE", this);
    apvts.addParameterListener("MODWAVETYPE", this);
    apvts.addParameterListener("MODCHOICE", this);

    //Adding listeners to each of the modable parameters- see the enumerator ModParams
    for (int i = 1; i < 7; i++)
    {
        ModParams param = ModParams(i);
        juce::String paramID = discernParameterID(param);
        apvts.addParameterListener(paramID, this);
        DBG("Added param listener to " + paramID);
    }

    getFilterType(false);
    shouldPrepare = false;
    apvts.state = juce::ValueTree("SavedParams");
    
}

TremoKittyAudioProcessor::~TremoKittyAudioProcessor()
{
    
}

//TO DO: check for each modable parameter if the value changes, if it's currenlt ybeing modded, update the previousmodparamvalue. 
//To make it so you can change the value that's being modded and it wont fucke up
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
    else if (parameterID == "MODWAVETYPE")
    {
        getWave(modules::mod);
    }
    else if (parameterID == "MODCHOICE")
    {
        switchProcessMod(newValue);
    }//else if
    //Mod Params Bullshit:
    else
    {
        updateModParam(newValue);
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

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample <= buffer.getNumSamples(); ++sample)
        {
        }
    }




    //My Stuff
    juce::dsp::AudioBlock<float> block(buffer);
    float modChoice = apvts.getRawParameterValue("MODCHOICE")->load();
    if (modChoice != 0.f)
    {
        //Calls the function with the name of the thing you want to actually modify
        int index = (int)modChoice;
        processMod(ModParams(modChoice));
    }


    //Tremolo Section
    //Loading the tremolo rate and depth parameters, only if the currently modded option isn't Tremolo
    float tremDepth = apvts.getRawParameterValue("TREMDEPTH")->load();
    float tremRate = apvts.getRawParameterValue("TREMRATE")->load();
    tremLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, tremRate);

    //Having the LFO process the gain sample, then setting the gainmodules gain to the new value given by the LFO,
    //then processing with the gain mod
    //Could literally just be 1.f
    float gain = apvts.getRawParameterValue("GAIN")->load();
    float gainMod = (tremLFO.processSample(0.f)) * tremDepth;
    if ((!apvts.getRawParameterValue("TREMBP")->load()))
    {
        if (tremRate != 0.f)
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
    panLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, panRate);
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
    
    filterLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, filterModRate);
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
juce::String TremoKittyAudioProcessor::discernParameterID(ModParams p)
{
    juce::String ParameterID;
    switch (p)
    {
    case(ModParams::None):
        ParameterID = "NONE";
        break;
    case(ModParams::TremRate):
        ParameterID = "TREMRATE";
        break;
    case(ModParams::TremDepth):
        ParameterID = "TREMDEPTH";
        break;
    case(ModParams::PanRate):
        ParameterID = "PANRATE";
        break;
    case(ModParams::PanDepth):
        ParameterID = "PANDEPTH";
        break;
    case(ModParams::FilterModRate):
        ParameterID = "FILTERRATE";
        break;
    case(ModParams::FilterModDepth):
        ParameterID = "FILTERMODLEVEL";
        break;
    default:
        DBG("Error discerning parameter ID from MODPARAMS");
    }
    return ParameterID;
}

void TremoKittyAudioProcessor::processMod(ModParams Parameter)
{
    float modFrequency = apvts.getRawParameterValue("MODLFORATE")->load();
    float modDepth = apvts.getRawParameterValue("MODLFODEPTH")->load();
    float paramMaxValue;
    modLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, modFrequency);
    if (modFrequency == 0)
        modDepth = 0.f;
 
    
    juce::String ParameterID = discernParameterID(Parameter);
    //Determining the max value of our modded parameter based on whether it's a rate or depth parameter. (Rates are always 0-10. depth is 0-1).
    if (ParameterID.contains("RATE"))
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
        auto oldVal = apvts.getRawParameterValue(ParameterID)->load();
        DBG("oldval = " + std::to_string(oldVal));
        apvts.getRawParameterValue("MODPARAMPREVIOUSVALUE")->store(oldVal);
        apvts.getRawParameterValue("MODRESETSWITCH")->store(0.f);
        //Should store the index of the last modded parameter
        apvts.getRawParameterValue("LASTMODDEDPARAM")->store(Parameter);
    }
    
    //Mod scaler is a value between 1 and 0, times the modDepth of 0 to 1.
    float modScaler = ((modLFO.processSample(0.f) + 1) * 0.5);
    auto oldValue = apvts.getRawParameterValue("MODPARAMPREVIOUSVALUE")->load();
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
    //Storing the oldvalue +the modscaler * modDepth. Lets do the math
    /*values between 0 and 10:
    * say the original value is 7. The modscaler will be 0-1 mapped out between 0 minus 7 and 10 minus 7, in other words -7 and 13. Lets say the LFO is giving us a value of 1.0, so the scaler is at the max value. the modscaler will give us 13. Then when we add that to our original value, 7+13 = 10, therefore the max from the LFO will give us the max of the actual value. This is only if the moddepth is fully engaged. If the moddepth is at, say, 0.5, then the range of the modscaler collapses from -7 to 13 to -3.5 to 6.5. This way, the signal is only being modulated from a range that goes from the original value to halfway down to 0, and halfway to the max. In other words it modulates 7 down to 3.5, up to 13.5. And of course, if mod depth is 0, then 0 will be added to the oldvalue, so the parameter will not be being changed at all.
    * The math is pretty much the same for the 0-1 values. Just shrink the formula down.
    */
    apvts.getRawParameterValue(ParameterID)->store(oldValue + (modScaler*modDepth));
    DBG("The parameter is  " + std::to_string(apvts.getRawParameterValue(ParameterID)->load()));
    
}

//Sets the parameterID 
void TremoKittyAudioProcessor::switchProcessMod(float newValue)
{
    int oldParamIndex = apvts.getRawParameterValue("LASTMODDEDPARAM")->load();
    juce::String ParameterID = discernParameterID(ModParams(oldParamIndex));
    //sets the modded parameter id back to its original value
    if (ParameterID != "NONE")
    {
        float oldValue = (apvts.getRawParameterValue("MODPARAMPREVIOUSVALUE")->load());
        DBG("Old value = " + std::to_string(oldValue));
        DBG("param id = " + ParameterID);
        apvts.getRawParameterValue(ParameterID)->store(oldValue);
    }
    apvts.getRawParameterValue("MODRESETSWITCH")->store(true);
}

void TremoKittyAudioProcessor::updateModParam(float newValue)
{
    apvts.getRawParameterValue("MODPARAMPREVIOUSVALUE")->store(newValue);
}

// :_( The technology just wasnt there yet... A memorial to a feature that didn't make it in.
/*
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
        tremLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, finalFrequency);
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
        modLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, finalFrequency);
        break;
    default:
        DBG("Something went horribly wrong in the big function. Good luck");
    }

}
*/
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
    layout.add(std::make_unique<juce::AudioParameterFloat>("TREMRATE", "Tremolo Rate", juce::NormalisableRange<float>(0.f, 10.f, 0.01, 0.5f), 0.1f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("TREMDEPTH", "Tremolo Depth", 0.f, 1.f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("TREMWAVE", "Tremolo Modulation Waveform", juce::StringArray("Sine", "Saw", "SawDown", "Triangle", "Square"), 0));
    layout.add(std::make_unique < juce::AudioParameterBool>("TREMBP", "Tremolo Bypass", false));

    //Panner section
    layout.add(std::make_unique<juce::AudioParameterFloat>("PANRATE", "Pan Rate", juce::NormalisableRange<float>(0.f, 10.f, 0.01, 0.5f), 7.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PANDEPTH", "Pan Depth", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("PANWAVE", "Pan Mod Waveform", juce::StringArray("Sine", "Saw", "SawDown", "Triangle", "Square"), 0));
    layout.add(std::make_unique < juce::AudioParameterBool>("PANBP", "Pan Bypass", false));

    //Filter section
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERRATE", "Filter Rate", juce::NormalisableRange<float>(0.f, 10.f, 0.01, 0.5f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERMODLEVEL", "Filter Mod Level", juce::NormalisableRange<float>(0.f, 1.f, 0.001f, 0.35), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float>(0.f, 1.f, 0.000001, 0.25), 0.9f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float>(0.7f, 10.f, 0.05, 0.9), (1 / sqrt(2))));
    layout.add(std::make_unique<juce::AudioParameterChoice>("FILTERWAVE", "Filter Mod Waveform", juce::StringArray("Sine", "Saw", "SawDown", "Triangle", "Square"), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray("Low Pass", "High Pass", "Band Pass"), 0));
    layout.add(std::make_unique<juce::AudioParameterBool>("FILTERBP", "Filter Bypass", false));

    //ModLFO Section
    layout.add(std::make_unique<juce::AudioParameterFloat>("MODLFORATE", "Mod LFO Rate", juce::NormalisableRange<float>(0.f, 10.f, 0.01, 0.5f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("MODLFODEPTH", "Mod LFO Depth", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("MODWAVETYPE", "Mod LFO Wave Type", juce::StringArray("Sine", "Saw", "SawDown", "Triangle", "Square"), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("MODCHOICE", "Mod LFO Parameter Choice", juce::StringArray("None", "Trem Rate", "Trem Depth", "Pan Rate", "Pan Depth", "Filter Mod Rate", "Filter Mod Depth"), 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("LASTMODDEDPARAM", "The String Name of the last parameter that was modded", juce::StringArray("None", "Trem Rate", "Trem Depth", "Pan Rate", "Pan Depth", "Filter Cutoff", "Filter Mod Rate", "Filter Mod Depth"), 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("MODPARAMPREVIOUSVALUE", "Modded Parameter Pre-modded Value", 0.f, 10.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterBool>("MODRESETSWITCH", "Modded Param Reset Switch", true));

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
           tremLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSawDown);
            apvts.getParameter("TREMWAVE")->setValue(2);
            break;
        case 3:
            tremLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kTriangle);
            apvts.getParameter("TREMWAVE")->setValue(3);
            break;
        case 4:
            tremLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSquare);
            apvts.getParameter("TREMWAVE")->setValue(4);
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
            panLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSawDown);
            apvts.getParameter("PANWAVE")->setValue(2);
            break;
        case 3:
            panLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kTriangle);
            apvts.getParameter("PANWAVE")->setValue(3);
            break;
        case 4:
            panLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSquare);
            apvts.getParameter("PANWAVE")->setValue(4);
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
            filterLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSawDown);
            apvts.getParameter("FILTERWAVE")->setValue(2);
            break;
        case 3:
            filterLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kTriangle);
            apvts.getParameter("FILTERWAVE")->setValue(3);
            break;
        case 4:
            filterLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSquare);
            apvts.getParameter("FILTERWAVE")->setValue(4);
            break;
        default:
            DBG("There was an issue with reassigning wavetype");
        }
        break;
    case(modules::mod):
        index = apvts.getRawParameterValue("MODWAVETYPE")->load();
        switch (index)
        {
        case 0:
            modLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSine);
            apvts.getParameter("MODWAVETYPE")->setValue(0);
            break;
        case 1:
            modLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSaw);
            apvts.getParameter("MODWAVETYPE")->setValue(1);
            break;
        case 2:
            modLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSawDown);
            apvts.getParameter("MODWAVETYPE")->setValue(2);
            break;
        case 3:
            modLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kTriangle);
            apvts.getParameter("MODWAVETYPE")->setValue(3);
            break;
        case 4:
            modLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSquare);
            apvts.getParameter("MODWAVETYPE")->setValue(4);
            break;
        default:
            DBG("There was an issue with reassigning wavetype");
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
        DBG("WE PREPARED AHAHHAH");
        filter.reset();
        filter.prepare(spec);
     }
}

