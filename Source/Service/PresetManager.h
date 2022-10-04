/*
  ==============================================================================

    PresetManager.h
    Created: 3 Oct 2022 10:41:22pm
    Author:  lmend

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Service
{
    class PresetManager : juce::ValueTree::Listener
    {
    public:
        static const juce::File defaultDirectory;
        static const juce::String defaultPresetName;
        static const juce::String extension;
        static const juce::String presetNameProperty;

        PresetManager(juce::AudioProcessorValueTreeState& valueTreeState);

        void savePreset(const juce::String& presetName);
        void deletePreset(const juce::String& presetName);
        void loadPreset(const juce::String& presetName);
        int loadNextPreset();
        int loadPreviousPreset();
        void updateAPVTS(const int index);

        juce::StringArray getAllPresets() const;
        juce::String getCurrentPreset() const;

    private:
        void valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged) override;
        juce::Value currentPreset;
        juce::AudioProcessorValueTreeState& valueTreeState;

    };
}