/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "NoteDurations.h"

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
    enum class modules { tremolo, pan, filter, master };
    void resetEverything();
    void loadPreset(const juce::String& name);

    NoteDurations noteDuration;
    juce::AudioProcessorValueTreeState apvts;
    juce::dsp::Gain<float> gainModule;

    float filterCutoff;
    

private:

    juce::AudioPlayHead* playHead;
    juce::AudioPlayHead::CurrentPositionInfo playheadCurrentPosition;

    

    juce::dsp::ProcessSpec spec;

    viator_dsp::LFOGenerator testLFO;

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

    bool shouldPrepare;
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoKittyAudioProcessor)
};
