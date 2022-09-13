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
    //Header
    header.setColour(1, juce::Colours::chartreuse);
    header.setButtonText("TremoKitty!");
    addAndMakeVisible(header);

    /*Tremolo start*/
    //Trem Rate
    createSlider(tremRateSlider);
    tremRateSlider.setRange(0.f, 20.f, 10.f);
    tremRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREMRATE", tremRateSlider);
    createLabel("tremrate", tremRateLabel);

    //Trem Depth
    createSlider(tremDepthSlider);
    tremDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREMDEPTH", tremDepthSlider);
    createLabel("tremdepth", TremDepthLabel);

    //Trem Wave Combobox
    tremWaveChoice.addItem("Sine", 1);
    tremWaveChoice.addItem("Saw", 2);
    tremWaveChoice.addItem("Square", 3);
    tremWaveChoice.onChange = [&] {changeWave(modules::tremolo); };
    addAndMakeVisible(tremWaveChoice);
    tremWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "TREMWAVE", tremWaveChoice);


    /*Tremolo end*/

    /*Panning starts*/
    createSlider(PanRateSlider);
    PanRateSlider.setRange(0.f, 20.f, 10.f);
    panRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PANRATE", PanRateSlider);
    createLabel("Pan Rate", PanRateLabel);

    createSlider(PanDepthSlider);
    panDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PANDEPTH", PanDepthSlider);
    createLabel("Pan Depth", PanDepthLabel);

    //pan wave combobox
    PanWaveChoice.addItem("Sine", 1);
    PanWaveChoice.addItem("Saw", 2);
    PanWaveChoice.addItem("square", 3);
    PanWaveChoice.onChange = [&] {changeWave(modules::pan); };
    addAndMakeVisible(PanWaveChoice);

    /*Panning ends*/

    /*Filter starts*/
    createSlider(FilterModRate);
    FilterModRate.setRange(0.f, 20.f, 10.f);
    




    

    setSize (500, 500);
}

void TremoKittyAudioProcessorEditor::changeWave(modules m)
{
    switch (m)
    {
    case(modules::tremolo):
        audioProcessor.changeWave(tremWaveChoice.getSelectedItemIndex(), modules::tremolo);
        break;
    case(modules::pan):
        audioProcessor.changeWave(PanWaveChoice.getSelectedItemIndex(), modules::pan);
        break;
    case(modules::filter):
        audioProcessor.changeWave(FilterWaveChoice.getSelectedItemIndex(), modules::filter);
        break;
    default:
        DBG("Failed to send change wave command to processor");
    }

}

void TremoKittyAudioProcessorEditor::createLabel(const juce::String& name, juce::Label& label)
{
    label.setText(name, juce::NotificationType::dontSendNotification);
    label.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(label);
}

void TremoKittyAudioProcessorEditor::createSlider(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
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
    juce::FlexBox ArchFB;
    juce::FlexBox Tremfb;
    juce::FlexBox Panfb;
    juce::FlexBox Filterfb;

    auto area = getLocalBounds();
    auto TremArea = juce::Rectangle<int>(area.getX() / 2, area.getY() * 0.63);
    auto PanArea = juce::Rectangle<int>(area.getX() / 2, area.getY() * 0.63);
    auto FilterArea = juce::Rectangle<int>(area.getX(), area.getY() * 0.33);

    auto headerHeight = 36;
    auto sliderWidth = 45;

    //Misc
    ArchFB.flexDirection = juce::FlexBox::Direction::column;
    ArchFB.flexWrap = juce::FlexBox::Wrap::wrap;
    ArchFB.alignContent = juce::FlexBox::AlignContent::stretch;

    ArchFB.items.add(juce::FlexItem(100, 35, header));

    //Tremolo
    Tremfb.flexDirection = juce::FlexBox::Direction::column;
    Tremfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Tremfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Tremfb.justifyContent = juce::FlexBox::JustifyContent::center;

    Tremfb.items.add(juce::FlexItem(50, 100, tremRateSlider));
    Tremfb.items.add(juce::FlexItem(50, 100, tremRateLabel));
    Tremfb.items.add(juce::FlexItem(50, 100, tremDepthSlider));
    Tremfb.items.add(juce::FlexItem(50, 100, TremDepthLabel));
    Tremfb.items.add(juce::FlexItem(50, 100, tremWaveChoice));

    //Pan Section
    Panfb.flexDirection = juce::FlexBox::Direction::column;
    Panfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Panfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Panfb.justifyContent = juce::FlexBox::JustifyContent::center;

    Panfb.items.add(juce::FlexItem(50, 100, PanRateSlider));
    Panfb.items.add(juce::FlexItem(50, 100, PanRateLabel));
    Panfb.items.add(juce::FlexItem(50, 100, PanDepthSlider));
    Panfb.items.add(juce::FlexItem(50, 100, PanDepthLabel));
    Panfb.items.add(juce::FlexItem(50, 100, PanWaveChoice));

    //Filter Section

    Tremfb.performLayout(area.removeFromLeft(TremArea.getX()));
    Panfb.performLayout(area.removeFromRight(area.getX()+50));
    ArchFB.performLayout(area.removeFromTop(35));
}
