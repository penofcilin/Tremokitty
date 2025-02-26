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
    using Resource = juce::WebBrowserComponent::Resource;
    std::optional<Resource> getResource(const juce::String& url);
    //webviews Testing section
    juce::WebBrowserComponent webView;

    //General Elements
    juce::Gui::MyLNF myLNF;
    juce::ImageComponent tremoKittyBanner;
    juce::ImageComponent background;
    juce::Label header;
    juce::ToggleButton MasterBypass;
    Gui::PresetPanel presetPanel;
    juce::ImageButton defaultSkinButton, halloweenSkinButton, christmasSkinButton, spaceSkinButton, displayKittyButton, setDefaultSkinButton;
    
    //Tremolo Section
    juce::Label tremSectionHeader;
    juce::Slider tremRateSlider;
    juce::ComboBox tremSyncChoice;
    juce::Label tremRateLabel;
    juce::Slider tremDepthSlider;
    juce::Label TremDepthLabel;
    juce::ComboBox tremWaveChoice;
    juce::ToggleButton TremBypass;
    juce::ToggleButton TremSyncButton;

    //Panning Section
    juce::Label panSectionHeader;
    juce::Slider PanRateSlider;
    juce::Label PanRateLabel;
    juce::Slider PanDepthSlider;
    juce::Label PanDepthLabel;
    juce::ComboBox PanWaveChoice;
    juce::ComboBox PanSyncChoice;
    juce::ToggleButton PanBypass;
    juce::ToggleButton PanSyncButton;

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
    juce::ComboBox FilterSyncChoice;
    juce::ComboBox FilterType;
    juce::ToggleButton FilterBypass;
    juce::ToggleButton FilterSyncButton;

    //ModLFO section
    juce::Label modSectionHeader;
    juce::Slider ModLFORateSlider;
    juce::Label ModLFORateLabel;
    juce::Slider ModLFODepthSlider;
    juce::Label ModLFODepthLabel;
    juce::ComboBox ModLFOWaveType;
    juce::ComboBox ModLFOModOptions;
    juce::ComboBox ModSyncChoice;
    juce::ToggleButton ModBypass;
    juce::ToggleButton ModSyncButton;

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

    //Syncing
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> TremSyncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> PanSyncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> FilterSyncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> ModSyncAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> TremSyncChoiceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> PanSyncChoiceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> FilterSyncChoiceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ModSyncChoiceAttachment;

    

    //Member functions
    void createSlider(juce::Slider& slider);
    void createLabel(const juce::String& name, juce::Label& label);
    void createToggleButton(const juce::String& text, juce::ToggleButton& button);
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    void syncButtonClicked(juce::ToggleButton* button);
    void changeLabelColours();

    //Set up functions
    void loadInitialState();
    void setUpTremoloSection();
    void setUpPannerSection();
    void setUpFilterSection();
    void setUpModSection();
    void setUpSkinButtons();

    //Member Variable
    bool shouldNotDisplayKitty{ false };
    int currentDefaultSkin{ 0 };
    //Processor
    TremoKittyAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoKittyAudioProcessorEditor)
};
