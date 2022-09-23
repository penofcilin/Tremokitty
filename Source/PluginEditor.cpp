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
    header.setColour(juce::TextButton::buttonColourId, juce::Colours::beige);
    header.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    header.setButtonText("TremoKitty!");
    addAndMakeVisible(header);

    //Reset Button
    ResetButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    ResetButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    ResetButton.setButtonText("Reset Everything");
    ResetButton.onClick = [&] {resetEverything(); };
    addAndMakeVisible(ResetButton);

    //Master BP
    createToggleButton("Master Bypass", MasterBypass);
    MasterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MASTERBP", MasterBypass);
    

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
    addAndMakeVisible(tremWaveChoice);
    TremWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "TREMWAVE", tremWaveChoice);
    createSyncBox(TremSyncChoice, false);
    createSyncBox(TremSyncChoiceMod, true);

    createToggleButton("Tremolo Bypass", TremBypass);
    TremBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "TREMBP", TremBypass);

    

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
    PanWaveChoice.addItem("Square", 3);
    addAndMakeVisible(PanWaveChoice);
    PanWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "PANWAVE", PanWaveChoice);

    createSyncBox(PanSyncChoice, false);
    createSyncBox(PanSyncChoiceMod, true);

    createToggleButton("Pan Bypass", PanBypass);
    PanBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "PANBP", PanBypass);
    
    

    /*Panning ends*/

    /*Filter starts*/
    //Filter cutoff
    createSlider(FilterCutoffSlider);
    FilterCutoffSlider.setRange(0.f, 1.f, 0.00001f);
    FilterCutoffSlider.addListener(this);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERCUTOFF", FilterCutoffSlider);
    createLabel("Cutoff", FilterCutoffLabel);
    float startingCutoff = juce::jmap(audioProcessor.apvts.getRawParameterValue("FILTERCUTOFF")->load(), 20.f, 20000.f);
    FilterCutoffLabel.setText(std::to_string(startingCutoff), juce::NotificationType::dontSendNotification);
    FilterCutoffLabel.attachToComponent(&FilterCutoffSlider, false);

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
    FilterResonanceSlider.setRange(0.07, 1, 0.1f);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERRES", FilterResonanceSlider);
    createLabel("Filter Resonance", FilterResonanceLabel);
    //Filter Wave Combobox
    FilterWaveChoice.addItem("Sine", 1);
    FilterWaveChoice.addItem("Saw", 2);
    FilterWaveChoice.addItem("square", 3);
    FilterWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTERWAVE", FilterWaveChoice);
    createSyncBox(FilterSyncChoice, false);
    createSyncBox(FilterSyncChoiceMod, true);
    addAndMakeVisible(FilterWaveChoice);
    

    //Filter type Combobox
    FilterType.addItem("Low Pass", 1);
    FilterType.addItem("High Pass", 2);
    FilterType.addItem("Band Pass", 3);
    FilterTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTERTYPE", FilterType);
    addAndMakeVisible(FilterType);

    createToggleButton("Filter Bypass", FilterBypass);
    FilterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "FILTERBP", FilterBypass);
    /*Filter ends*/

    /*Mod Section Starts*/
    createSlider(ModLFORateSlider);
    ModLFORateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MODLFORATE", ModLFORateSlider);
    createLabel("Mod LFO Rate", ModLFORateLabel);
    for (int i = 0; i < audioProcessor.modableParams.size(); i++)
    {
        ModLFOModOptions.addItem(audioProcessor.modableParams[i], i+1);
    }
    createSlider(ModLFODepthSlider);
    createLabel("Mod LFO Depth", ModLFODepthLabel);
    ModLFODepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MODLFODEPTH", ModLFODepthSlider);
    ModLFOWaveType.addItem("Sine", 1);
    ModLFOWaveType.addItem("Saw", 2);
    ModLFOWaveType.addItem("Square", 3);
    ModLFOWaveTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "LFOWAVETYPE", ModLFOWaveType);
    //Fils the combobox with the straight notes
    createSyncBox(ModLFOSyncChoice, false);
    createSyncBox(ModLFOSyncChoiceMod, true);
    addAndMakeVisible(ModLFOSyncChoice);
    addAndMakeVisible(ModLFOSyncChoiceMod);
    addAndMakeVisible(ModLFOWaveType);
    addAndMakeVisible(ModLFOModOptions);
       
    /*Mod Section Ends*/
    
    
    

    setSize (500, 500);
    initializedGUI = true;
}

void TremoKittyAudioProcessorEditor::createSyncBox(juce::ComboBox& box, bool ModBox)
{
    int i = 1;
    if (!ModBox)
    {
        for (juce::String s : audioProcessor.noteDuration.getStraightNoteTypes())
        {
            box.addItem(s, i++);
        }
    }
    else
    {
        box.addItem("Straight", 1);
        box.addItem("Dotted", 2);
        box.addItem("Triplet", 3);
    }
    addAndMakeVisible(box);
}

void TremoKittyAudioProcessorEditor::resetEverything()
{
    audioProcessor.resetEverything();
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
        if (initializedGUI)
        {
            float newValue = slider->getValue();
            FilterCutoffLabel.setText((std::to_string((int)juce::jmap(newValue, 20.f, 20000.f))) + " HZ", juce::NotificationType::dontSendNotification);
        }
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
    g.setGradientFill(juce::ColourGradient::vertical(juce::Colour::fromRGB(245, 256, 220), getHeight(), juce::Colour::fromRGB(255, 182, 193), getHeight()));
    g.fillRect(getLocalBounds());
    g.setColour(juce::Colours::whitesmoke.withAlpha(0.25f));
    g.fillRect(getLocalBounds());
    getTopLevelComponent()->setName("TremoKitty!");
}



void TremoKittyAudioProcessorEditor::resized()
{
    juce::FlexBox ArchFB;
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

    auto headerHeight = 36;
    auto sliderWidth = 45;

    //Misc
    ArchFB.flexDirection = juce::FlexBox::Direction::column;
    ArchFB.flexWrap = juce::FlexBox::Wrap::wrap;
    ArchFB.alignContent = juce::FlexBox::AlignContent::stretch;

    ArchFB.items.add(juce::FlexItem(100, 35, header));
    ArchFB.items.add(juce::FlexItem(50, 50, MasterBypass));
    ArchFB.items.add(juce::FlexItem(40, 40, ResetButton));

    //Tremolo
    Tremfb.flexDirection = juce::FlexBox::Direction::column;
    Tremfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Tremfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Tremfb.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    Tremfb.items.add(juce::FlexItem(75, 45, tremRateSlider));
    Tremfb.items.add(juce::FlexItem(75, 45, tremRateLabel));
    Tremfb.items.add(juce::FlexItem(75, 45, tremDepthSlider));
    Tremfb.items.add(juce::FlexItem(75, 45, TremDepthLabel));
    Tremfb.items.add(juce::FlexItem(75, 45, tremWaveChoice));
    Tremfb.items.add(juce::FlexItem(45, 45, TremBypass));
    Tremfb.items.add(juce::FlexItem(45, 45, TremSyncChoice));
    Tremfb.items.add(juce::FlexItem(45, 45, TremSyncChoiceMod));

    //Pan Section
    Panfb.flexDirection = juce::FlexBox::Direction::column;
    Panfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Panfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Panfb.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    Panfb.items.add(juce::FlexItem(75, 45, PanRateSlider));
    Panfb.items.add(juce::FlexItem(75, 45, PanRateLabel));
    Panfb.items.add(juce::FlexItem(75, 45, PanDepthSlider));
    Panfb.items.add(juce::FlexItem(75, 45, PanDepthLabel));
    Panfb.items.add(juce::FlexItem(75, 45, PanWaveChoice));
    Panfb.items.add(juce::FlexItem(45, 45, PanBypass));
    Panfb.items.add(juce::FlexItem(45, 45, PanSyncChoice));
    Panfb.items.add(juce::FlexItem(45, 45, PanSyncChoiceMod));

    //Filter Section
    Filterfb.flexDirection = juce::FlexBox::Direction::column;
    Filterfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Filterfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Filterfb.justifyContent = juce::FlexBox::JustifyContent::center;

    Filterfb.items.add(juce::FlexItem(100, 45, FilterCutoffSlider));
    Filterfb.items.add(juce::FlexItem(25, 25, FilterCutoffLabel));
    Filterfb.items.add(juce::FlexItem(75, 45, FilterModRate));
    Filterfb.items.add(juce::FlexItem(25, 25, FilterModLabel));
    Filterfb.items.add(juce::FlexItem(75, 45, FilterModAmount));
    Filterfb.items.add(juce::FlexItem(25, 25, FilterModAmountLabel));
    Filterfb.items.add(juce::FlexItem(75, 45, FilterResonanceSlider));
    Filterfb.items.add(juce::FlexItem(25, 25, FilterResonanceLabel));
    Filterfb.items.add(juce::FlexItem(75, 45, FilterType));
    Filterfb.items.add(juce::FlexItem(75, 45, FilterWaveChoice));
    Filterfb.items.add(juce::FlexItem(45, 45, FilterBypass));
    Filterfb.items.add(juce::FlexItem(45, 45, FilterSyncChoice));
    Filterfb.items.add(juce::FlexItem(45, 45, FilterSyncChoiceMod));

    //Mod Section
    Modfb.flexDirection = juce::FlexBox::Direction::column;
    Modfb.flexWrap = juce::FlexBox::Wrap::wrap;
    Modfb.alignContent = juce::FlexBox::AlignContent::stretch;
    Modfb.justifyContent = juce::FlexBox::JustifyContent::center;

    Modfb.items.add(juce::FlexItem(35, 35, ModLFORateSlider));
    Modfb.items.add(juce::FlexItem(35, 35, ModLFORateLabel));
    Modfb.items.add(juce::FlexItem(35, 35, ModLFODepthSlider));
    Modfb.items.add(juce::FlexItem(35, 35, ModLFODepthLabel));
    Modfb.items.add(juce::FlexItem(35, 35, ModLFOWaveType));
    Modfb.items.add(juce::FlexItem(35, 35, ModLFOSyncChoice));
    Modfb.items.add(juce::FlexItem(35, 35, ModLFOSyncChoiceMod));
    Modfb.items.add(juce::FlexItem(35, 35, ModLFOModOptions));

    //Performing Layout
    ArchFB.performLayout(threeQuarterArea.removeFromTop(50));
    Tremfb.performLayout(threeQuarterArea.removeFromLeft(200));
    Panfb.performLayout(threeQuarterArea.removeFromRight(200));
    Modfb.performLayout(threeQuarterArea.removeFromTop(150));
    Filterfb.performLayout(area.removeFromBottom(135));
    
}
