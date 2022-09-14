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
    //Filter cutoff
    createSlider(FilterCutoffSlider);
    FilterCutoffSlider.setRange(0.f, 20.f, 18.f);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERCUTOFF", FilterCutoffSlider);
    createLabel("Cutoff", FilterCutoffLabel);

    //Filter mod rate
    createSlider(FilterModRate);
    FilterModRate.setRange(0.f, 20.f, 10.f);
    filterModRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERRATE", FilterModRate);
    createLabel("Mod Rate", FilterModLabel);

    //Filter mod level
    createSlider(FilterModAmount);
    FilterModAmount.setRange(0.f, 1.f, 0.f);
    filterModAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERMODLEVEL", FilterModAmount);
    createLabel("Mod Level", FilterModAmountLabel);

    //Filter resonance
    createSlider(FilterResonanceSlider);
    FilterResonanceSlider.setRange(0.f, 1.f, 0.f);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERRES", FilterResonanceSlider);
    createLabel("Filter Resonance", FilterResonanceLabel);
    //Filter Wave Combobox
    FilterWaveChoice.addItem("Sine", 1);
    FilterWaveChoice.addItem("Saw", 2);
    FilterWaveChoice.addItem("square", 3);
    FilterWaveChoice.onChange = [&] {changeWave(modules::filter); };
    addAndMakeVisible(FilterWaveChoice);
    

    //Filter type Combobox
    FilterType.addItem("Low Pass", 1);
    FilterType.addItem("High Pass", 2);
    FilterType.addItem("Band Pass", 3);
    FilterType.onChange = [&] {changeFilterType(FilterType.getSelectedItemIndex()); };
    addAndMakeVisible(FilterType);
    /*Filter ends*/
    

    setSize (500, 500);
}

void TremoKittyAudioProcessorEditor::changeFilterType(int index)
{
    audioProcessor.changeFilterType(index);
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

    Tremfb.items.add(juce::FlexItem(75, 75, tremRateSlider));
    Tremfb.items.add(juce::FlexItem(75, 75, tremRateLabel));
    Tremfb.items.add(juce::FlexItem(75, 75, tremDepthSlider));
    Tremfb.items.add(juce::FlexItem(75, 75, TremDepthLabel));
    Tremfb.items.add(juce::FlexItem(75, 75, tremWaveChoice));

    //Pan Section
    Panfb.flexDirection = juce::FlexBox::Direction::column;
    Panfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Panfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Panfb.justifyContent = juce::FlexBox::JustifyContent::center;

    Panfb.items.add(juce::FlexItem(75, 75, PanRateSlider));
    Panfb.items.add(juce::FlexItem(75, 75, PanRateLabel));
    Panfb.items.add(juce::FlexItem(75, 75, PanDepthSlider));
    Panfb.items.add(juce::FlexItem(75, 75, PanDepthLabel));
    Panfb.items.add(juce::FlexItem(75, 75, PanWaveChoice));

    //Filter Section
    Filterfb.flexDirection = juce::FlexBox::Direction::column;
    Filterfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Filterfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Filterfb.justifyContent = juce::FlexBox::JustifyContent::center;

    Filterfb.items.add(juce::FlexItem(75, 75, FilterCutoffSlider));
    Filterfb.items.add(juce::FlexItem(75, 75, FilterCutoffLabel));
    Filterfb.items.add(juce::FlexItem(75, 75, FilterModRate));
    Filterfb.items.add(juce::FlexItem(75, 75, FilterModLabel));
    Filterfb.items.add(juce::FlexItem(75, 75, FilterModAmount));
    Filterfb.items.add(juce::FlexItem(75, 75, FilterModAmountLabel));
    Filterfb.items.add(juce::FlexItem(75, 75, FilterResonanceSlider));
    Filterfb.items.add(juce::FlexItem(75, 75, FilterResonanceLabel));
    Filterfb.items.add(juce::FlexItem(75, 75, FilterType));
    Filterfb.items.add(juce::FlexItem(75, 75, FilterWaveChoice));

    //Performing Layout
    Tremfb.performLayout(area.removeFromLeft(TremArea.getX()));
    Panfb.performLayout(area.removeFromRight(area.getX()+50));
    Filterfb.performLayout(area.removeFromBottom(area.getX()));
    ArchFB.performLayout(area.removeFromTop(35));
}
