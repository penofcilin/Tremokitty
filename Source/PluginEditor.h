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
    class TremoKittyAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::ComboBox::Listener, public juce::Button::Listener
    {
    public:
        TremoKittyAudioProcessorEditor(TremoKittyAudioProcessor&);
        ~TremoKittyAudioProcessorEditor() override;


        void resized() override;

    private:
        using Resource = juce::WebBrowserComponent::Resource;
        std::optional<Resource> getResource(const juce::String& url);
        //webviews Testing section
        juce::WebBrowserComponent webView;

        juce::TextButton runJavaScriptButton{ "Run some JS" };

        
        //Frontend Events
        void sliderChanged(juce::var info);

        void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
        void sliderValueChanged(juce::Slider* slider) override;
        void buttonClicked(juce::Button* button) override;
        void changeLabelColours();

        //Set up functions
        void loadInitialState();

        void testNativeFunction(const juce::Array<juce::var>& args, juce::WebBrowserComponent::NativeFunctionCompletion completion);

        //Member Variable
        bool shouldNotDisplayKitty{ false };
        int currentDefaultSkin{ 0 };
        //Processor
        TremoKittyAudioProcessor& audioProcessor;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TremoKittyAudioProcessorEditor)
    };
} // nameSpace End
