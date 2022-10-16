/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
TremoKittyAudioProcessorEditor::TremoKittyAudioProcessorEditor (TremoKittyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), presetPanel(p.getPresetManager())
{
    
    juce::LookAndFeel::setDefaultLookAndFeel(&myLNF);

    //Getting the header rectangle image
    auto kittyImage = juce::ImageCache::getFromMemory(BinaryData::TremoKittyBanner_png, BinaryData::TremoKittyBanner_pngSize);
    if (!kittyImage.isNull())
        tremoKittyBanner.setImage(kittyImage, juce::RectanglePlacement::stretchToFit);
    else
        jassert(!kittyImage.isNull());
    addAndMakeVisible(tremoKittyBanner);

    auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::BG_png, BinaryData::BG_pngSize);
    if (!backgroundImage.isNull())
    {
        background.setImage(backgroundImage, juce::RectanglePlacement::stretchToFit);
        background.setAlpha(0.1);
    }
    else
        jassert(!backgroundImage.isNull());
    addAndMakeVisible(background);

    //Header Label
    createLabel("TremoKitty!", header);
    header.setFont(juce::Font("Calibri", 20.f, juce::Font::bold));
    header.setColour(juce::Label::ColourIds::textColourId, juce::Colours::ghostwhite);


    //PresetPanel
    addAndMakeVisible(presetPanel);

    //Got rid of the reset button cause you can just select the default preset
    //Got rid of master bypass cause kind of pointless
    
    setUpTremoloSection();
    setUpPannerSection();
    setUpFilterSection();
    setUpModSection();
  
    setSize (500, 550);
}

void TremoKittyAudioProcessorEditor::setUpTremoloSection()
{
    createLabel("Tremolo", tremSectionHeader);
    tremSectionHeader.setFont(juce::Font( "Calibri", 35, juce::Font::bold));


   //Trem Rate
    createSlider(tremRateSlider);
    tremRateSlider.setRange(0.f, 20.f, 10.f);
    tremRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREMRATE", tremRateSlider);
    createLabel("tremrate", tremRateLabel);

    //Trem Depth
    createSlider(tremDepthSlider);
    tremDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREMDEPTH", tremDepthSlider);
    createLabel("tremdepth", TremDepthLabel);

    tremWaveChoice.addItemList(audioProcessor.WaveTypes, 1);
    tremWaveChoice.addListener(this);


    TremWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "TREMWAVE", tremWaveChoice);
    addAndMakeVisible(tremWaveChoice);

    createToggleButton("Tremolo Bypass", TremBypass);
    TremBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "TREMBP", TremBypass);
}

void TremoKittyAudioProcessorEditor::setUpPannerSection()
{
    createLabel("Panner", panSectionHeader);
    panSectionHeader.setFont(juce::Font("Calibri", 35, juce::Font::bold));
    createSlider(PanRateSlider);
    PanRateSlider.setRange(0.f, 10.f, 10.f);
    panRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PANRATE", PanRateSlider);
    createLabel("Pan Rate", PanRateLabel);

    createSlider(PanDepthSlider);
    panDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PANDEPTH", PanDepthSlider);
    createLabel("Pan Depth", PanDepthLabel);

    //pan wave combobox
    PanWaveChoice.addItemList(audioProcessor.WaveTypes, 1);
    addAndMakeVisible(PanWaveChoice);
    PanWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "PANWAVE", PanWaveChoice);

    createToggleButton("Pan Bypass", PanBypass);
    PanBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "PANBP", PanBypass);

}

void TremoKittyAudioProcessorEditor::setUpFilterSection()
{
    createLabel("Filter", filterSectionHeader);
    filterSectionHeader.setFont(juce::Font("Calibri", 35, juce::Font::bold));
    //Filter cutoff
    createSlider(FilterCutoffSlider);
    FilterCutoffSlider.setRange(0.f, 1.f, 0.00001f);
    FilterCutoffSlider.addListener(this);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERCUTOFF", FilterCutoffSlider);
    createLabel("Cutoff", FilterCutoffLabel);
    float startingCutoff = juce::jmap(audioProcessor.apvts.getRawParameterValue("FILTERCUTOFF")->load(), 20.f, 20000.f);
    FilterCutoffLabel.setText(std::to_string(startingCutoff), juce::NotificationType::dontSendNotification);

    //Filter mod rate
    createSlider(FilterModRate);
    FilterModRate.setRange(0.f, 10.f, 10.f);
    filterModRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERRATE", FilterModRate);
    createLabel("Mod Rate", FilterModLabel);

    //Filter mod level
    createSlider(FilterModAmount);
    FilterModAmount.setRange(0.f, 1.f, 0.f);
    filterModAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERMODLEVEL", FilterModAmount);
    createLabel("Mod Level", FilterModAmountLabel);

    //Filter resonance
    createSlider(FilterResonanceSlider);
    FilterResonanceSlider.setRange(0.07, 1, 0.1f);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERRES", FilterResonanceSlider);
    createLabel("Filter Resonance", FilterResonanceLabel);
    //Filter Wave Combobox
    FilterWaveChoice.addItemList(audioProcessor.WaveTypes, 1);
    FilterWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTERWAVE", FilterWaveChoice);
    addAndMakeVisible(FilterWaveChoice);


    //Filter type Combobox
    FilterType.addItem("Low Pass", 1);
    FilterType.addItem("High Pass", 2);
    FilterType.addItem("Band Pass", 3);
    FilterTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTERTYPE", FilterType);
    addAndMakeVisible(FilterType);

    createToggleButton("Filter Bypass", FilterBypass);
    FilterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "FILTERBP", FilterBypass);
}

void TremoKittyAudioProcessorEditor::setUpModSection()
{
    createLabel("Mod", modSectionHeader);
    modSectionHeader.setFont(juce::Font("Calibri", 25, juce::Font::bold));
    createSlider(ModLFORateSlider);
    ModLFORateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MODLFORATE", ModLFORateSlider);
    createLabel("Mod LFO Rate", ModLFORateLabel);
    ModLFOModOptions.addItemList(audioProcessor.ModParams, 1);
    ModLFOModdedParameterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "MODCHOICE", ModLFOModOptions);
    createSlider(ModLFODepthSlider);
    createLabel("Mod LFO Depth", ModLFODepthLabel);
    ModLFODepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MODLFODEPTH", ModLFODepthSlider);
    ModLFOWaveType.addItemList(audioProcessor.WaveTypes, 1);
    ModLFOWaveTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "MODWAVETYPE", ModLFOWaveType);
    addAndMakeVisible(ModLFOWaveType);
    addAndMakeVisible(ModLFOModOptions);
    createToggleButton("Mod LFO Bypass", ModBypass);
    ModBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MODBP", ModBypass);
}

//DISGUSTING, ABSOLUTELY DISGUSTING, might have to do this for the rest of the modules as well if its' still broken
void TremoKittyAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    float index = comboBoxThatHasChanged->getSelectedItemIndex();
    if (index == 3)
        audioProcessor.changeTremWaveManually();
}

void TremoKittyAudioProcessorEditor::createToggleButton(const juce::String& text, juce::ToggleButton& button)
{
    button.setButtonText(text);
    button.setClickingTogglesState(true);
    button.setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::black);
    button.setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::red);
    button.setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::black);
    addAndMakeVisible(button);
}

void TremoKittyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &FilterCutoffSlider)
    { 
        float newValue = slider->getValue();
        FilterCutoffLabel.setText("Cutoff = " + (std::to_string((int)juce::jmap(newValue, 20.f, 20000.f))) + " HZ", juce::NotificationType::dontSendNotification);
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
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void TremoKittyAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    g.fillRect(bounds);
    g.setColour(juce::Colours::lightgoldenrodyellow);
    g.fillRect(bounds);
    getTopLevelComponent()->setName("TremoKitty!");
}

void TremoKittyAudioProcessorEditor::resized()
{
    juce::FlexBox Tremfb;
    juce::FlexBox Panfb;
    juce::FlexBox Filterfb;
    juce::FlexBox Modfb;

    auto area = getLocalBounds();
    auto threeQuarterArea = juce::Rectangle<int>(500, 365);
    auto x = area.getX();
    auto y = area.getY();

    auto TremArea = juce::Rectangle<int>(area.getX() / 2, area.getY() * 0.63);
    auto PanArea = juce::Rectangle<int>(area.getX() / 2, area.getY() * 0.63);
    auto FilterArea = juce::Rectangle<int>(area.getX(), area.getY() * 0.33);
    auto ModArea = juce::Rectangle<int>(area.getX()/4, area.getY()/4);
    

    //Tremolo
    Tremfb.flexDirection = juce::FlexBox::Direction::column;
    Tremfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Tremfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Tremfb.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    Tremfb.items.add(juce::FlexItem(100, 25, tremSectionHeader));
    Tremfb.items.add(juce::FlexItem(75, 25, tremRateSlider));
    Tremfb.items.add(juce::FlexItem(75, 25, tremRateLabel));
    Tremfb.items.add(juce::FlexItem(75, 25, tremDepthSlider));
    Tremfb.items.add(juce::FlexItem(75, 25, TremDepthLabel));
    Tremfb.items.add(juce::FlexItem(45, 25, tremWaveChoice));
    Tremfb.items.add(juce::FlexItem(45, 45, TremBypass));

    //Pan Section
    Panfb.flexDirection = juce::FlexBox::Direction::column;
    Panfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Panfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Panfb.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    Panfb.items.add(juce::FlexItem(100, 25, panSectionHeader));
    Panfb.items.add(juce::FlexItem(75, 25, PanRateSlider));
    Panfb.items.add(juce::FlexItem(75, 25, PanRateLabel));
    Panfb.items.add(juce::FlexItem(75, 25, PanDepthSlider));
    Panfb.items.add(juce::FlexItem(75, 25, PanDepthLabel));
    Panfb.items.add(juce::FlexItem(45, 25, PanWaveChoice));
    Panfb.items.add(juce::FlexItem(45, 45, PanBypass));

    //Filter Section
    Filterfb.flexDirection = juce::FlexBox::Direction::column;
    Filterfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Filterfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Filterfb.justifyContent = juce::FlexBox::JustifyContent::center;

    Filterfb.items.add(juce::FlexItem(100, 25, filterSectionHeader));
    Filterfb.items.add(juce::FlexItem(45, 25, FilterCutoffSlider));
    Filterfb.items.add(juce::FlexItem(25, 15, FilterCutoffLabel));
    Filterfb.items.add(juce::FlexItem(45, 25, FilterModRate));
    Filterfb.items.add(juce::FlexItem(25, 25, FilterModLabel));
    Filterfb.items.add(juce::FlexItem(45, 25, FilterModAmount));
    Filterfb.items.add(juce::FlexItem(25, 25, FilterModAmountLabel));
    Filterfb.items.add(juce::FlexItem(45, 25, FilterResonanceSlider));
    Filterfb.items.add(juce::FlexItem(25, 25, FilterResonanceLabel));
    Filterfb.items.add(juce::FlexItem(45, 25, FilterType));
    Filterfb.items.add(juce::FlexItem(45, 25, FilterWaveChoice));
    Filterfb.items.add(juce::FlexItem(25, 25, FilterBypass));

    //Mod Section
    Modfb.flexDirection = juce::FlexBox::Direction::column;
    Modfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Modfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Modfb.justifyContent = juce::FlexBox::JustifyContent::center;

    Modfb.items.add(juce::FlexItem(75, 25, modSectionHeader));
    Modfb.items.add(juce::FlexItem(35, 25, ModLFORateSlider));
    Modfb.items.add(juce::FlexItem(35, 25, ModLFORateLabel));
    Modfb.items.add(juce::FlexItem(35, 25, ModLFODepthSlider));
    Modfb.items.add(juce::FlexItem(35, 25, ModLFODepthLabel));
    Modfb.items.add(juce::FlexItem(35, 25, ModLFOModOptions));
    Modfb.items.add(juce::FlexItem(35, 25, ModLFOWaveType));
    Modfb.items.add(juce::FlexItem(15, 15, ModBypass));

    //Performing Layout
    background.setBounds(getLocalBounds());
    tremoKittyBanner.setBounds(threeQuarterArea.removeFromTop(30));
    presetPanel.setBounds(threeQuarterArea.removeFromTop(area.proportionOfHeight(0.07f)));
    Tremfb.performLayout(threeQuarterArea.removeFromLeft(threeQuarterArea.proportionOfWidth(0.5)).reduced(4));
    Panfb.performLayout(threeQuarterArea.removeFromRight(threeQuarterArea.getWidth()).reduced(4));
    Filterfb.performLayout(area.removeFromBottom(area.proportionOfHeight(0.3f)));
    Modfb.performLayout(area.removeFromBottom(area.proportionOfHeight(0.3f)));

    
    
    //Painting the header
    auto newRect = getLocalBounds();
    auto bannerBounds = newRect.removeFromTop(30);
    header.setSize(147, 50);
    header.setBoundsToFit(bannerBounds, juce::Justification::centred, false);
}
