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
    class TremoKittyAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
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

        //Events from frontend
        void sliderChanged(juce::var info);
        void dropdownCommit(juce::var info);
        void buttonClicked(juce::var info);
        void formSubmitted(juce::var info);
        void toggleGroupChanged(juce::var info);

        //Events to frontend
        void timerCallback() override;
        void emitFrontendEvent(const juce::String& identifier, juce::var value);

        //AUGHHH
        const std::vector<juce::String> parameterIDs{
       "GAIN",
       "MASTERBP",
       "PRESETINDEX",

       "TREMRATE",
       "TREMDEPTH",
       "TREMWAVE",
       "TREMSYNCCHOICE",
       "TREMBP",
       "TREMSYNC",

       "PANRATE",
       "PANDEPTH",
       "PANWAVE",
       "PANSYNCCHOICE",
       "PANBP",
       "PANSYNC",

       "FILTERRATE",
       "FILTERMODLEVEL",
       "FILTERCUTOFF",
       "FILTERRES",
       "FILTERWAVE",
       "FILTERTYPE",
       "FILTERSYNCCHOICE",
       "FILTERBP",
       "FILTERSYNC",

       "MODLFORATE",
       "MODLFODEPTH",
       "MODWAVETYPE",
       "MODCHOICE",
       "LASTMODDEDPARAM",
       "MODSYNCCHOICE",
       "MODPARAMPRIORVALUE",
       "MODRESETSWITCH",
       "MODBP",
       "MODSYNC"
        };
      
        void testNativeFunction(const juce::Array<juce::var>& args, juce::WebBrowserComponent::NativeFunctionCompletion completion);

        juce::Array<juce::var> convertPresetNames(juce::StringArray& names);
        void prepareAPVTSState(juce::WebBrowserComponent::NativeFunctionCompletion completion);
        juce::var prepareAPVTSState();

        void updateUI();

        //Member Variable
        bool shouldNotDisplayKitty{ false };
        int currentDefaultSkin{ 0 };
        //Processor
        TremoKittyAudioProcessor& audioProcessor;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TremoKittyAudioProcessorEditor)
    };
} // nameSpace End
