/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PresetPanel.h"
#include "MyLNF.h"
#define modules TremoKittyAudioProcessor::modules

//==============================================================================
/**
*/
class TremoKittyAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::ComboBox::Listener, public juce::Button::Listener
{
public:
    TremoKittyAudioProcessorEditor (TremoKittyAudioProcessor&);
    ~TremoKittyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //General Elements
    juce::Gui::MyLNF myLNF;
    juce::ImageComponent tremoKittyBanner;
    juce::ImageComponent background;
    juce::Label header;
    juce::ToggleButton MasterBypass;
    Gui::PresetPanel presetPanel;
    juce::ImageButton defaultSkinButton, halloweenSkinButton, christmasSkinButton, spaceSkinButton, displayKittyButton;
    
    //Tremolo Section
    juce::Label tremSectionHeader;
    juce::Slider tremRateSlider;
    juce::Label tremRateLabel;
    juce::Slider tremDepthSlider;
    juce::Label TremDepthLabel;
    juce::ComboBox tremWaveChoice;
    juce::ToggleButton TremBypass;

    //Panning Section
    juce::Label panSectionHeader;
    juce::Slider PanRateSlider;
    juce::Label PanRateLabel;
    juce::Slider PanDepthSlider;
    juce::Label PanDepthLabel;
    juce::ComboBox PanWaveChoice;
    juce::ToggleButton PanBypass;

    //Filter Section
    juce::Label filterSectionHeader;
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
    juce::Label modSectionHeader;
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
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ModLFOModdedParameterAttachment;


    //Member functions
    void createSlider(juce::Slider& slider);
    void createLabel(const juce::String& name, juce::Label& label);
    void createToggleButton(const juce::String& text, juce::ToggleButton& button);
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    void changeLabelColours();

    //Set up functions
    void loadInitialState();
    void setUpTremoloSection();
    void setUpPannerSection();
    void setUpFilterSection();
    void setUpModSection();
    void setUpSkinButtons();

    //Member Variable
    bool shouldDisplayKitty{ true };
    //Processor
    TremoKittyAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoKittyAudioProcessorEditor)
};
