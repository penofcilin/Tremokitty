/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TremoKittyAudioProcessorEditor::TremoKittyAudioProcessorEditor (TremoKittyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    header.setColour(0, juce::Colours::chartreuse);
    header.setButtonText("TremoKitty!");
    addAndMakeVisible(header);

    createSlider(tremRateSlider);
    tremRateSlider.setRange(0.f, 20.f, 10.f);
    tremRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREMRATE", tremRateSlider);
    createLabel("tremrate", tremRateLabel);


    tremWaveChoice.addItem("Sine", 1);
    tremWaveChoice.addItem("Saw", 2);
    tremWaveChoice.addItem("Square", 3);
    tremWaveChoice.onChange = [&] {changeTremWave(); };
    addAndMakeVisible(tremWaveChoice);
    tremWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "TREMWAVE", tremWaveChoice);

    createSlider(tremDepthSlider);
    tremDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREMDEPTH", tremDepthSlider);
    createLabel("tremdepth", TremDepthLabel);
    

    setSize (500, 500);
}

void TremoKittyAudioProcessorEditor::changeTremWave()
{
    audioProcessor.changeTremWave(tremWaveChoice.getSelectedItemIndex());
}

void TremoKittyAudioProcessorEditor::createLabel(const juce::String& name, juce::Label& label)
{
    label.setText(name, juce::NotificationType::dontSendNotification);
    label.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(label);
}

void TremoKittyAudioProcessorEditor::createSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setDoubleClickReturnValue(true, 0.5);
    slider.setRange(0.f, 1.f, 0.01f);
    slider.setValue(0.5);
    addAndMakeVisible(slider);
}

TremoKittyAudioProcessorEditor::~TremoKittyAudioProcessorEditor()
{
}

//==============================================================================
void TremoKittyAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::beige);
    getTopLevelComponent()->setName("TremoKitty!");
}

void TremoKittyAudioProcessorEditor::resized()
{

    juce::FlexBox Tremfb;
    juce::FlexBox Panfb;
    juce::FlexBox Filterfb;

    Tremfb.flexDirection = juce::FlexBox::Direction::column;
    Tremfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Tremfb.alignContent = juce::FlexBox::AlignContent::stretch;

    Tremfb.items.add(juce::FlexItem(50, 100, tremRateSlider));
    Tremfb.items.add(juce::FlexItem(50, 100, tremRateLabel));
    Tremfb.items.add(juce::FlexItem(50, 100, tremWaveChoice));
    Tremfb.items.add(juce::FlexItem(50, 100, tremDepthSlider));
    Tremfb.items.add(juce::FlexItem(50, 100, TremDepthLabel));


    Tremfb.items.add(juce::FlexItem(50, 100, header));


    auto area = getLocalBounds();
    auto TremArea = juce::Rectangle<int>(area.getX() / 2, area.getY() * 0.75);
    auto PanArea = juce::Rectangle<int>(area.getX() / 2, area.getY() * 0.75);

    auto headerHeight = 36;
    auto sliderWidth = 45;
    //header.setBounds(area.removeFromTop(headerHeight));


    //TremRate Stuff
    //tremRateSlider.setBounds(area.removeFromLeft(sliderWidth));
    //tremRateLabel.setBounds(area.removeFromLeft(sliderWidth));
    juce::Rectangle<int> RBounds = tremRateLabel.getBounds();
    //tremRateLabel.setBounds(RBounds.getX(), RBounds.getY() + 25, RBounds.getWidth() + 30, RBounds.getHeight());
    

    //TremDepth Stuff
    //tremDepthSlider.setBounds(area.removeFromRight(sliderWidth));

    //TremDepthLabel.setBounds(area.removeFromRight(sliderWidth));
    juce::Rectangle<int> TBounds = TremDepthLabel.getBounds();
    //TremDepthLabel.setBounds(TBounds.getX(), TBounds.getY() + 25, TBounds.getWidth()+30, TBounds.getHeight());
    //tremWaveChoice.setBounds(area.removeFromBottom(headerHeight));
    Tremfb.performLayout(area.removeFromLeft(250));
}
