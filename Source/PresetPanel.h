/*
  ==============================================================================

    PresetPanel.h
    Created: 2 Oct 2022 10:57:45pm
    Author:  lmend

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
namespace Gui
{
    class PresetPanel : public juce::Component, juce::Button::Listener
    {
    public:
        PresetPanel();

    private:
        juce::TextButton saveButton, deleteButton, previousPresetButton, nextPresetButton;
        juce::ComboBox presetList;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel);
    };
}