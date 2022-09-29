/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
/**
*/
class TremoKittyAudioProcessor  : 
    public juce::AudioProcessor,
    public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    TremoKittyAudioProcessor();
    ~TremoKittyAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    enum class waveForms { sine, saw, square };
    enum class modules { tremolo, pan, filter, mod, master };
    void resetEverything();
    void loadPreset(const juce::String& name);

    juce::AudioProcessorValueTreeState apvts;
    juce::dsp::Gain<float> gainModule;

    juce::StringArray ModParamsStrings{"None", "Trem Rate", "Trem Depth", "Pan Rate", "Pan Depth", "Filter Cutoff", "Filter Mod Rate", "Filter Mod Depth", "Filter Resonance"};
    enum ModParams{None, TremRate, TremDepth, PanRate, PanDepth, FilterCutoff, FilterModRate, FilterModDepth, FilterResonance};
    juce::StringArray WaveTypes{ "Sine", "Saw", "SawDown", "Triangle", "Square" };

    

private:

    juce::dsp::ProcessSpec spec;

    viator_dsp::LFOGenerator tremLFO;
    viator_dsp::LFOGenerator panLFO;
    viator_dsp::LFOGenerator filterLFO;
    viator_dsp::LFOGenerator modLFO;
   
    
    juce::dsp::StateVariableTPTFilter<float> filter;
    juce::dsp::Panner<float> panner;

    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    void getFilterType(bool shouldPrepare);
    void getWave(modules module);
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void processMod(ModParams Parameter);
    void stopProcessMod();
    juce::String discernParameterID(ModParams P);
    //void processSyncTime(modules m);

    bool shouldPrepare;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoKittyAudioProcessor)
};
