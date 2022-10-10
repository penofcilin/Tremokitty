/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PresetPanel.h"
#define modules TremoKittyAudioProcessor::modules

//==============================================================================
/**
*/
class TremoKittyAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::ComboBox::Listener
{
public:
    TremoKittyAudioProcessorEditor (TremoKittyAudioProcessor&);
    ~TremoKittyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    //General Elements
    juce::TextButton header;
    juce::ToggleButton MasterBypass;
    juce::TextButton ResetButton;
    Gui::PresetPanel presetPanel;

    //Tremolo Section
    juce::Slider tremRateSlider;
    juce::Label tremRateLabel;
    juce::Slider tremDepthSlider;
    juce::Label TremDepthLabel;
    juce::ComboBox tremWaveChoice;
    juce::ToggleButton TremBypass;

    //Panning Section
    juce::Slider PanRateSlider;
    juce::Label PanRateLabel;
    juce::Slider PanDepthSlider;
    juce::Label PanDepthLabel;
    juce::ComboBox PanWaveChoice;
    juce::ToggleButton PanBypass;

    //Filter Section
    juce::Slider FilterCutoffSlider;
    juce::Label FilterCutoffLabel;
    juce::Slider FilterModRate;
    juce::Label FilterModLabel;
    juce::Slider FilterModAmount;
    juce::Label FilterModAmountLabel;
    juce::Slider FilterResonanceSlider;
    juce::Label FilterResonanceLabel;
    juce::ComboBox FilterWaveChoice;
    juce::ComboBox FilterType;
    juce::ToggleButton FilterBypass;

    //ModLFO section
    juce::Slider ModLFORateSlider;
    juce::Label ModLFORateLabel;
    juce::Slider ModLFODepthSlider;
    juce::Label ModLFODepthLabel;
    juce::ComboBox ModLFOWaveType;
    juce::ComboBox ModLFOModOptions;
    juce::ToggleButton ModBypass;

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
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>ModLFOModdedParameterAttachment;


    //Member functions
    void createSlider(juce::Slider& slider);
    void createLabel(const juce::String& name, juce::Label& label);
    void createToggleButton(const juce::String& text, juce::ToggleButton& button);
    void sliderValueChanged(juce::Slider* slider) override;
    void resetEverything();

    //Processor
    TremoKittyAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoKittyAudioProcessorEditor)
};
