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
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "apvts", createParameters())
#endif
{

    tremLFO.initialise([](float x) {return std::sin(x); }, 256);
    panLFO.initialise([](float x) {return std::sin(x); }, 256);
    filterLFO.initialise([](float x) {return std::sin(x); }, 256);
    testLFO.initialise([](float x) {return std::sin(x); }, 256);
}

TremoKittyAudioProcessor::~TremoKittyAudioProcessor()
{
    
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
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();


    gainModule.prepare(spec);
    tremLFO.prepare(spec);
    filterLFO.prepare(spec);
    panLFO.prepare(spec);

    testLFO.prepare(spec);

    filter.prepare(spec);
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

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

void TremoKittyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

  
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel); 
        for (int sample = 0; sample <= buffer.getNumSamples(); ++sample)
        {

        }


        
    }


    //My Stuff
    juce::dsp::AudioBlock<float> block(buffer);

    //Tremolo Section
    //Loading the tremolo rate and depth parameters
    float tremDepth = apvts.getRawParameterValue("TREMDEPTH")->load();
    float tremRate = apvts.getRawParameterValue("TREMRATE")->load();
    //setting the parameters in the tremolo LFO to match the correct ones
    tremLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, tremRate*50);
    //Having the LFO process the gain sample, then setting the gainmodules gain to the new value given by the LFO,
    //then processing with the gain mod
    float newGain = tremLFO.processSample(apvts.getRawParameterValue("GAIN")->load());
    gainModule.setGainLinear(newGain*tremDepth);
    gainModule.process(juce::dsp::ProcessContextReplacing<float>(block));


    //Panning section
    float panDepth = apvts.getRawParameterValue("PANDEPTH")->load();
    float panRate = apvts.getRawParameterValue("PANRATE")->load();
    panLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, panRate * 50);
    //Add panning functionality

    //Filter Section
    float filterCutoff = apvts.getRawParameterValue("FILTERCUTOFF")->load();
    float filterResonance = apvts.getRawParameterValue("FILTERRES")->load();
    float moddedFilterCutoff = filterLFO.processSample(filterCutoff);

    testLFO.setParameter(viator_dsp::LFOGenerator::ParameterId::kFrequency, 1000.f);
    testLFO.setWaveType(viator_dsp::LFOGenerator::WaveType::kSine);
    auto value = testLFO.processSample(100.f);

    DBG(std::to_string(value));

    filter.setCutoffFrequency(filterCutoff);
    //filter.setResonance(filterResonance);
    



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

//==============================================================================
void TremoKittyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TremoKittyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TremoKittyAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout  TremoKittyAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.f, 1.f, 1.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("TREMRATE", "Tremolo Rate", 0.f, 20.f, 5.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("TREMDEPTH", "Tremolo Depth", 0.f, 1.f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterBool>("TREMBYPASS", "Tremolo Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterChoice>("TREMWAVE", "Tremolo Waveform", juce::StringArray("Sine", "Saw", "Square"), 0));


    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERRATE", "Filter Rate", 0.f, 20.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERMODLEVEL", "Filter Mod Level", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", 20.f, 20000.f, 20000.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterBool>("FILTERBYPASS", "Filter Bypass", true));
    layout.add(std::make_unique<juce::AudioParameterChoice>("FILTERWAVE", "Filter Mod Waveform", juce::StringArray("Sine", "Saw", "Square"), 0));

    layout.add(std::make_unique<juce::AudioParameterFloat>("PANRATE", "Pan Rate", 0.f, 20.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("PANDEPTH", "Pan Depth", 0.f, 1.f, 0.f));
    layout.add(std::make_unique<juce::AudioParameterBool>("PANBYPASS", "Pan Bypass", true));
    layout.add(std::make_unique<juce::AudioParameterChoice>("PANWAVE", "Pan Mod Waveform", juce::StringArray("Sine", "Saw", "Square"), 0));


    return layout;
}


void TremoKittyAudioProcessor::changeWave(int index, modules module)
{
    switch (module)
    {
    case(modules::tremolo):
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

void TremoKittyAudioProcessor::prepare(const juce::dsp::ProcessSpec& spec)
{
    
}
