/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"
#include "PresetPanel.h"
#include "MyLNF.h"
#define modules TremoKittyAudioProcessor::modules

namespace kitty_editor {
    class TremoKittyAudioProcessorEditor : public juce::AudioProcessorEditor
    {
    public:
        TremoKittyAudioProcessorEditor(TremoKittyAudioProcessor&);
        ~TremoKittyAudioProcessorEditor() override;

        void resized() override;

    private:
        //webviews Testing section
        using Resource = juce::WebBrowserComponent::Resource;
        std::optional<Resource> getResource(const juce::String& url);
       
        juce::WebBrowserComponent webView;

        //Frontend Events
        void sliderChanged(juce::var info);
        void dropdownCommit(juce::var info);
        void buttonClicked(juce::var info);
        void formSubmitted(juce::var info);
        void toggleGroupChanged(juce::var info);

        void testNativeFunction(const juce::Array<juce::var>& args, juce::WebBrowserComponent::NativeFunctionCompletion completion);

        juce::Array<juce::var> convertPresetNames(juce::StringArray& names);

        //Member Variable
        bool shouldNotDisplayKitty{ false };
        int currentDefaultSkin{ 0 };
        //Processor
        TremoKittyAudioProcessor& audioProcessor;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TremoKittyAudioProcessorEditor)
    };
} // nameSpace End
