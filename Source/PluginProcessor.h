/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Service/PresetManager.h"


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

    enum class modules { tremolo, pan, filter, mod, master };
    void resetEverything();
    Service::PresetManager& getPresetManager() { return *presetManager; }

    juce::AudioProcessorValueTreeState apvts;


    //Some Information objects
    juce::StringArray ModParams{"None", "TREMRATE", "TREMDEPTH", "PANRATE", "PANDEPTH", "FILTERRATE", "FILTERMODLEVEL"};
    juce::StringArray WaveTypes{ "Sine", "Saw", "SawDown", "Square" };
    juce::StringArray FilterTypes{ "Low Pass", "High Pass", "Band Pass" };
private:

    juce::dsp::ProcessSpec spec;

    //LFO Section, so pretty, all in a row, like red toy soldiers marching through the snow
    viator_dsp::LFOGenerator tremLFO;
    viator_dsp::LFOGenerator panLFO;
    viator_dsp::LFOGenerator filterLFO;
    viator_dsp::LFOGenerator modLFO;

   //DSP modules
    juce::dsp::Gain<float> gainModule;
    juce::dsp::Panner<float> panner;
    juce::dsp::StateVariableTPTFilter<float> filter;
    
    //APVTS helper methods
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    //Our presetManager Instance
    std::unique_ptr<Service::PresetManager> presetManager;

   //Some Member Functions
    void getFilterType(bool shouldPrepare);
    void getWave(modules module);
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void processMod(const juce::String& parameterID);
    void switchProcessMod();
    void updateModParam(float newValue);

    //Used for preparing the filter module.
    bool shouldPrepare;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoKittyAudioProcessor)
};
