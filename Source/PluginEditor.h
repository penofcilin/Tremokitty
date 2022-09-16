/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#define modules TremoKittyAudioProcessor::modules

//==============================================================================
/**
*/
class TremoKittyAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    TremoKittyAudioProcessorEditor (TremoKittyAudioProcessor&);
    ~TremoKittyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton header;
    juce::ToggleButton MasterBypass;

    //Tremolo Section
    juce::Slider tremRateSlider;
    juce::Label tremRateLabel;
    juce::Slider tremDepthSlider;
    juce::Label TremDepthLabel;
    juce::ComboBox tremWaveChoice;
    juce::ToggleButton TremBypass;
    juce::ToggleButton TremSync;

    //Panning Section
    juce::Slider PanRateSlider;
    juce::Label PanRateLabel;
    juce::Slider PanDepthSlider;
    juce::Label PanDepthLabel;
    juce::ComboBox PanWaveChoice;
    juce::ToggleButton PanBypass;
    juce::ToggleButton PanSync;

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
    juce::ToggleButton FIlterModSync;

    //Attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremDepthAttachment; 
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterModRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterModAmountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;


    //Member functions
    void createSlider(juce::Slider& slider);
    void createLabel(const juce::String& name, juce::Label& label);
    void toggleBypass(modules m);
    void changeWave(modules m);
    void changeFilterType(int index);
    void sliderValueChanged(juce::Slider* slider) override;

    //Member variables
    bool initializedGUI = false;

    
//Processor
    TremoKittyAudioProcessor& audioProcessor;



    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoKittyAudioProcessorEditor)
};
