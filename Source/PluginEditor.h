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

        
        //Attachments
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremRateAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremDepthAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panRateAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panDepthAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterModRateAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterModAmountAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ModLFORateAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ModLFODepthAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> MasterBypassAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> TremBypassAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> PanBypassAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> FilterBypassAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ModBypassAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> TremWaveAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> PanWaveAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> FilterWaveAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> FilterTypeAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ModLFOWaveTypeAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ModLFOModdedParameterAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> TremSyncAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> PanSyncAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> FilterSyncAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ModSyncAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> TremSyncChoiceAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> PanSyncChoiceAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> FilterSyncChoiceAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ModSyncChoiceAttachment;

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
