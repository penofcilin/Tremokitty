/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <chrono>
#include <mutex>
#include <vector>
#include "Service/PresetManager.h"
#include "KOLFO.h"
#include "KOTempo.h"


#define WAVE_TYPES  "Sine", "Cosine", "NegativeCosine",  "Saw", "SawDown", "Square", "InverseSquare", "Random"

struct ParamUpdate {
    juce::Identifier id;
    float value;
};

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

    void processBlockBypassed(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override; // doesnt seem to work

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
    void setStateInformation(const void* data, int sizeInBytes) override;

    //My Public Members
    Service::PresetManager& getPresetManager() { return *presetManager; }
    void changeTremWaveManually(int index);
    juce::AudioProcessorValueTreeState apvts;
    juce::ApplicationProperties globalProperties;

    //Used for the frontend filter preview with modulation enabled.
    std::atomic<float> filterLFOCurrentPosition{ 0.0f };
    std::atomic<float> tremLFOCurrentPosition{ 0.0f };
    std::atomic<float> panLFOCurrentPosition{ 0.0f };
    std::atomic<float> modLFOCurrentPosition{ 0.0f };

    //Some Information Structures
    juce::StringArray ModParams{"None", "TREMRATE", "TREMDEPTH", "PANRATE", "PANDEPTH", "FILTERRATE", "FILTERMODLEVEL"};
    juce::StringArray WaveTypes{ WAVE_TYPES };
    juce::StringArray FilterTypes{ "Low Pass", "High Pass", "Band Pass" };

    //Keeps track of if the plugin has been "initialized" (built for the first time) vs the gui being built
    bool hasRestoredState = false;
    bool hasLoadedInitialPreset = false;
    
    juce::StringArray PresetNames;
    enum class modules { tremolo, pan, filter, mod, master };

    void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
    //LFOlookuptable holds the values that our LFO processes so it can be checked in the processing period.
    std::vector<float> lfoLookupTable;
    juce::dsp::ProcessSpec spec;

    //LFO Section, so pretty, all in a row, like red toy soldiers marching through the snow
    KOLFO tremLFO;
    KOLFO panLFO;
    KOLFO filterLFO;
    KOLFO modLFO;

    //A list of the LFOs.
    std::vector<KOLFO> LFOList;

    //Tempo object
    KOTempo tempo;
    juce::AudioPlayHead* playHead;
    juce::AudioPlayHead::CurrentPositionInfo currentPosition;

   //DSP modules, gainModFilter filters out the tremolo LFO to avoid clicking when processing highly transient waveforms like saw and square
    juce::dsp::Gain<float> gainModule;
    juce::dsp::StateVariableTPTFilter<float> gainModFilter;
    juce::dsp::Panner<float> panner;
    juce::dsp::StateVariableTPTFilter<float> filter;
    //This filter actually applies to the mod LFO of the filter. This is to stop it from crackling at high modulation levels. XD
    juce::dsp::StateVariableTPTFilter<float> filterLFOFilter;
    
    //APVTS helper methods
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    void addListenersToAllParameters();
    juce::StringArray registeredParamIDs;

    //Our presetManager Instance
    std::unique_ptr<Service::PresetManager> presetManager;

   //Some Member Functions
    void getFilterType(bool shouldPrepare);
    void getWave(modules module);
   
    void processMod(const juce::String& parameterID, float bufferSize);
    void switchProcessMod();
    void updateModParam(float newValue);
    //This function will change the LFOs phase to match the current position of the playhead. For instance if the playhead is on the second quarter note of a bar and the sync choice is set to "whole", the LFO's phase will be advanced to halfway through it's period.
    void resetLFOPhase(KOLFO& LFO, const juce::String& parameterID);
    void resetAllLFOPhases();
    void playbackStart(const int bufferSamples);
    void playbackStop();

    //Used for preparing the filter module.
    bool shouldPrepare;
    bool playbackStopped{ true };
    bool bypassed{ false };

    int64_t nextExpectedPlaybackSample;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoKittyAudioProcessor)
};
