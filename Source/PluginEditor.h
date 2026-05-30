/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"


#define modules TremoKittyAudioProcessor::modules

namespace kitty_editor {
    //Forward declare editor
    class TremoKittyAudioProcessorEditor;

    //Parameter Update timer helper class
    class ParameterUpdateTimer : public juce::Timer
    {
    public:
        ParameterUpdateTimer(TremoKittyAudioProcessorEditor& editorRef)
            : editor(editorRef) {
        }
        void timerCallback() override;
          
    private:
        TremoKittyAudioProcessorEditor& editor;
    };

    //Editor
    class TremoKittyAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer, private juce::AudioProcessorValueTreeState::Listener
    {
    public:
        TremoKittyAudioProcessorEditor(TremoKittyAudioProcessor&);
        ~TremoKittyAudioProcessorEditor() override;

        void resized() override;

        void processParamUpdates();

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
        void parameterChanged(const juce::String& parameterID, float newValue) override;
        void emitFrontendEvent(const juce::String& identifier, juce::var value);

        //UI Timer/listener updates stuff
        ParameterUpdateTimer parameterUpdateTimer;

        juce::CriticalSection pendingLock;
        juce::NamedValueSet pendingUpdates;

        //AUGHHH
        const std::vector<juce::String> parameterIDs{
       "GAIN",
       "MIX",
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
       "FILTERMODBP",
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
        bool frontendReady {false};
        bool shouldNotDisplayKitty{ false };
        int currentDefaultSkin{ 0 };
        //Processor
        TremoKittyAudioProcessor& audioProcessor;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TremoKittyAudioProcessorEditor)
    };

} // nameSpace End
