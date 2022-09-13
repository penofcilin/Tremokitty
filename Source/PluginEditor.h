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
class TremoKittyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TremoKittyAudioProcessorEditor (TremoKittyAudioProcessor&);
    ~TremoKittyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton header;
    //Tremolo Section
    juce::Slider tremRateSlider;
    juce::Label tremRateLabel;
    juce::Slider tremDepthSlider;
    juce::Label TremDepthLabel;
    juce::ComboBox tremWaveChoice;

    //Panning Section
    juce::Slider PanRateSlider;
    juce::Label PanRateLabel;
    juce::Slider PanDepthSlider;
    juce::Label PanDepthLabel;
    juce::ComboBox PanWaveChoice;

    //Filter Section
    juce::Slider FilterCutoffSlider;
    juce::Label FilterCutoffLabel;
    juce::Slider FilterModRate;
    juce::Label FilterModLabel;
    juce::Slider FilterModAmount;
    juce::Label FilterModAmountLabel;
    juce::Slider FilterResonanceSlider;
    juce::ComboBox FilterWaveChoice;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tremDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> tremWaveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterModRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterModAmountAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    void createSlider(juce::Slider& slider);
    void createLabel(const juce::String& name, juce::Label& label);

    


    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TremoKittyAudioProcessor& audioProcessor;



    void changeWave(modules m);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoKittyAudioProcessorEditor)
};
