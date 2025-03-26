/*
  ==============================================================================

    This file contains the editor baby 

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


namespace kitty_editor
{
<<<<<<< HEAD
    namespace {
        auto streamToVector(juce::InputStream& stream)
=======
    background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
    background.setAlpha(0.1);
    addAndMakeVisible(background);

    //Getting the header rectangle image
    auto kittyImage = juce::ImageCache::getFromMemory(BinaryData::TremoKittyBanner_png, BinaryData::TremoKittyBanner_pngSize);
    if (!kittyImage.isNull())
        tremoKittyBanner.setImage(kittyImage, juce::RectanglePlacement::stretchToFit);
    else
        jassert(!kittyImage.isNull());
    addAndMakeVisible(tremoKittyBanner);

    setUpSkinButtons();
    
    //Header Label
    createLabel("Tremokitty!", header);
    header.setFont(juce::Font("Calibri", 20.f, juce::Font::bold));
    header.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);

    //PresetPanel
    addAndMakeVisible(presetPanel);

    //Got rid of master bypass cause kind of pointless
    setUpTremoloSection();
    setUpPannerSection();
    setUpFilterSection();
    setUpModSection();
    changeLabelColours();
    
    setSize (500, 550);
    loadInitialState();
    setLookAndFeel(&myLNF);
}


void TremoKittyAudioProcessorEditor::loadInitialState()
{
    auto settings = audioProcessor.globalProperties.getUserSettings();
    auto shouldNotDisplay = settings->getBoolValue("DONTDISPLAYKITTY");
    auto skinToLoad = settings->getIntValue("DEFAULTSKIN", 0);
    DBG("TOLOAD = " + std::to_string(skinToLoad));
    switch (skinToLoad)
    {
    case(0):
        myLNF.changeSkin(juce::Gui::MyLNF::skins::sDefault);
        background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
        changeLabelColours();
        break;
    case(1):
        myLNF.changeSkin(juce::Gui::MyLNF::skins::sHalloween);
        background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
        changeLabelColours();
        break;
    case(2):
        myLNF.changeSkin(juce::Gui::MyLNF::skins::sChristmas);
        background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
        changeLabelColours();
        break;
    case(3):
        myLNF.changeSkin(juce::Gui::MyLNF::skins::sSpace);
        background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
        changeLabelColours();
        break;
    default:
        DBG("Failed to assign skin.");
        break;
    }
    if (!shouldNotDisplay == 0)
    {
        background.setVisible(false);
    }
    else
    {
        background.setVisible(true);
    }
    repaint();
}

void TremoKittyAudioProcessorEditor::setUpTremoloSection()
{
    //Create section header
    createLabel("Tremolo", tremSectionHeader);
    tremSectionHeader.setFont(juce::Font(myLNF.typeFace, 35, juce::Font::bold));
    tremSectionHeader.setColour(juce::Label::ColourIds::textColourId, myLNF.textColour);

   //Trem Rate
    createSlider(tremRateSlider);
    tremRateSlider.setRange(0.f, 20.f, 10.f);
    tremRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREMRATE", tremRateSlider);
    createLabel("Rate", tremRateLabel);
    //tremRateLabel.setColour(juce::Label::ColourIds::textColourId, myLNF.textColour);

    //Trem Depth
    createSlider(tremDepthSlider);
    tremDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TREMDEPTH", tremDepthSlider);
    createLabel("Depth", TremDepthLabel);
    //TremDepthLabel.setColour(juce::Label::ColourIds::textColourId, myLNF.textColour);

    //Set up comboboxes
    tremWaveChoice.addItemList(audioProcessor.WaveTypes, 1);
    tremWaveChoice.addListener(this);
    TremWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "TREMWAVE", tremWaveChoice);
    addAndMakeVisible(tremWaveChoice);

    //Sync button + combobox stuff
    createToggleButton("Tempo Sync", TremSyncButton);
    TremSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "TREMSYNC", TremSyncButton);
    TremSyncButton.addListener(this);

    tremSyncChoice.addItemList(KOTempo::getNoteTypesAlternative(), 1);
    TremSyncChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "TREMSYNCCHOICE", tremSyncChoice);
    addAndMakeVisible(tremSyncChoice);

    createToggleButton("Tremolo Bypass", TremBypass);
    TremBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "TREMBP", TremBypass);
}

void TremoKittyAudioProcessorEditor::setUpPannerSection()
{
    createLabel("Panner", panSectionHeader);
    panSectionHeader.setFont(juce::Font(myLNF.typeFace, 35, juce::Font::bold));
    panSectionHeader.setColour(juce::Label::ColourIds::textColourId, myLNF.textColour);

    createSlider(PanRateSlider);
    PanRateSlider.setRange(0.f, 10.f, 10.f);
    panRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PANRATE", PanRateSlider);
    createLabel("Rate", PanRateLabel);
    PanRateLabel.setColour(juce::Label::ColourIds::textColourId, myLNF.textColour);

    createSlider(PanDepthSlider);
    panDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PANDEPTH", PanDepthSlider);
    createLabel("Depth", PanDepthLabel);
    PanDepthLabel.setColour(juce::Label::ColourIds::textColourId, myLNF.textColour);

    //pan wave combobox
    PanWaveChoice.addItemList(audioProcessor.WaveTypes, 1);
    addAndMakeVisible(PanWaveChoice);
    PanWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "PANWAVE", PanWaveChoice);

    //Sync button + combobox stuff
    createToggleButton("Tempo Sync", PanSyncButton);
    PanSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "PANSYNC", PanSyncButton);
    PanSyncButton.addListener(this);

    PanSyncChoice.addItemList(KOTempo::getNoteTypesAlternative(), 1);
    PanSyncChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "PANSYNCCHOICE", PanSyncChoice);
    addAndMakeVisible(PanSyncChoice);

    createToggleButton("Pan Bypass", PanBypass);
    PanBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "PANBP", PanBypass);
    PanBypass.setColour(juce::Label::ColourIds::textColourId, myLNF.textColour);
}

void TremoKittyAudioProcessorEditor::setUpFilterSection()
{
    createLabel("Filter", filterSectionHeader);
    filterSectionHeader.setFont(juce::Font(myLNF.typeFace, 35, juce::Font::bold));
    filterSectionHeader.setColour(juce::Label::ColourIds::textColourId, myLNF.textColour);
    //Filter cutoff
    createSlider(FilterCutoffSlider);
    FilterCutoffSlider.setRange(0.f, 1.f, 0.00001f);
    FilterCutoffSlider.addListener(this);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERCUTOFF", FilterCutoffSlider);
    createLabel("Cutoff", FilterCutoffLabel);
    float startingCutoff = juce::jmap(audioProcessor.apvts.getRawParameterValue("FILTERCUTOFF")->load(), 20.f, 20000.f);
    FilterCutoffLabel.setText("Cutoff = " + std::to_string((int)startingCutoff) + " HZ", juce::NotificationType::dontSendNotification);
    addAndMakeVisible(FilterCutoffLabel);

    //Filter mod rate
    createSlider(FilterModRate);
    FilterModRate.setRange(0.f, 10.f, 10.f);
    filterModRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERRATE", FilterModRate);
    createLabel("Filter Mod Rate", FilterModLabel);

    //Filter mod level
    createSlider(FilterModAmount);
    FilterModAmount.setRange(0.f, 1.f, 0.f);
    filterModAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERMODLEVEL", FilterModAmount);
    createLabel("Filter Mod Depth", FilterModAmountLabel);

    //Filter resonance
    createSlider(FilterResonanceSlider);
    FilterResonanceSlider.setRange(0.07, 1, 0.1f);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTERRES", FilterResonanceSlider);
    createLabel("Filter Resonance", FilterResonanceLabel);
    //Filter Wave Combobox
    FilterWaveChoice.addItemList(audioProcessor.WaveTypes, 1);
    FilterWaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTERWAVE", FilterWaveChoice);
    addAndMakeVisible(FilterWaveChoice);

    //Sync button + combobox stuff
    createToggleButton("Tempo Sync", FilterSyncButton);
    FilterSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "FILTERSYNC", FilterSyncButton);
    FilterSyncButton.addListener(this);

    FilterSyncChoice.addItemList(KOTempo::getNoteTypesAlternative(), 1);
    FilterSyncChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "FILTERSYNCCHOICE", FilterSyncChoice);
    addAndMakeVisible(FilterSyncChoice);


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
    modSectionHeader.setFont(juce::Font(myLNF.typeFace, 35, juce::Font::bold));
    modSectionHeader.setColour(juce::Label::ColourIds::textColourId, myLNF.textColour);

    createSlider(ModLFORateSlider);
    ModLFORateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MODLFORATE", ModLFORateSlider);

    createLabel("Mod LFO Rate", ModLFORateLabel);
    ModLFOModOptions.addItemList(audioProcessor.ModParams, 1);
    ModLFOModdedParameterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "MODCHOICE", ModLFOModOptions);

    if (audioProcessor.apvts.getRawParameterValue("TREMSYNC")->load())
    {
        int id = audioProcessor.ModParams.indexOf("TREMRATE") + 1;
        ModLFOModOptions.setItemEnabled(id, false);
    }
    if (audioProcessor.apvts.getRawParameterValue("PANSYNC")->load())
    {
        int id = audioProcessor.ModParams.indexOf("PANRATE") + 1;
        ModLFOModOptions.setItemEnabled(id, false);
    }
    if (audioProcessor.apvts.getRawParameterValue("FILTERSYNC")->load())
    {
        int id = audioProcessor.ModParams.indexOf("FILTERRATE") + 1;
        ModLFOModOptions.setItemEnabled(id, false);
    }

    createSlider(ModLFODepthSlider);
    createLabel("Mod LFO Depth", ModLFODepthLabel);
    ModLFODepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MODLFODEPTH", ModLFODepthSlider);
    ModLFOWaveType.addItemList(audioProcessor.WaveTypes, 1);
    ModLFOWaveTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "MODWAVETYPE", ModLFOWaveType);

    addAndMakeVisible(ModLFOWaveType);
    addAndMakeVisible(ModLFOModOptions);
    createToggleButton("Mod LFO Bypass", ModBypass);
    ModBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MODBP", ModBypass);

    //Sync button + combobox stuff
    createToggleButton("Tempo Sync", ModSyncButton);
    ModSyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "MODSYNC", ModSyncButton);
    ModSyncButton.addListener(this);

    ModSyncChoice.addItemList(KOTempo::getNoteTypesAlternative(), 1);
    ModSyncChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "MODSYNCCHOICE", ModSyncChoice);
    addAndMakeVisible(ModSyncChoice);
}

void TremoKittyAudioProcessorEditor::setUpSkinButtons()
{
    auto sdsbi = juce::ImageCache::getFromMemory(BinaryData::setDefaultButtonImage_png, BinaryData::setDefaultButtonImage_pngSize);
    setDefaultSkinButton.setImages(false, true, true, sdsbi, 1.f, juce::Colours::transparentWhite, sdsbi, 0.5f, juce::Colours::transparentWhite, sdsbi, 0.2f, juce::Colours::transparentBlack);
    addAndMakeVisible(setDefaultSkinButton);
    setDefaultSkinButton.addListener(this);

    auto dsbi = juce::ImageCache::getFromMemory(BinaryData::defaultSkinButton_png, BinaryData::defaultSkinButton_pngSize);
    defaultSkinButton.setImages(false, true, true, dsbi, 1.f, juce::Colours::transparentWhite, dsbi, 0.5f, juce::Colours::transparentWhite, dsbi, 0.2f, juce::Colours::transparentBlack);
    addAndMakeVisible(defaultSkinButton);
    defaultSkinButton.addListener(this);

    auto hsbi = juce::ImageCache::getFromMemory(BinaryData::halloweenSkinButton_png, BinaryData::halloweenSkinButton_pngSize);
    halloweenSkinButton.setImages(false, true, true, hsbi, 1.f, juce::Colours::transparentWhite, hsbi, 0.5f, juce::Colours::transparentWhite, hsbi, 0.2f, juce::Colours::transparentBlack);
    addAndMakeVisible(halloweenSkinButton);
    halloweenSkinButton.addListener(this);

    auto csbi = juce::ImageCache::getFromMemory(BinaryData::christmasSkinButton_png, BinaryData::christmasSkinButton_pngSize);
    christmasSkinButton.setImages(false, true, true, csbi, 1.f, juce::Colours::transparentWhite, csbi, 0.5f, juce::Colours::transparentWhite, csbi, 0.2f, juce::Colours::transparentBlack);
    addAndMakeVisible(christmasSkinButton);
    christmasSkinButton.addListener(this);

    auto ssbi = juce::ImageCache::getFromMemory(BinaryData::spaceSkinButton_png, BinaryData::spaceSkinButton_pngSize);
    spaceSkinButton.setImages(false, true, true, ssbi, 1.f, juce::Colours::transparentWhite, ssbi, 0.5f, juce::Colours::transparentWhite, ssbi, 0.2f, juce::Colours::transparentBlack);
    addAndMakeVisible(spaceSkinButton);
    spaceSkinButton.addListener(this);

    auto kittyImage = juce::ImageCache::getFromMemory(BinaryData::ChristmasBG_png, BinaryData::ChristmasBG_pngSize);
    displayKittyButton.setImages(false, true, true, kittyImage, 1.f, juce::Colours::transparentWhite, kittyImage, 0.5f, juce::Colours::transparentBlack, kittyImage, 0.25f, juce::Colours::transparentBlack);
    addAndMakeVisible(displayKittyButton);
    displayKittyButton.addListener(this);
}

//DISGUSTING, ABSOLUTELY DISGUSTING, might have to do this for the rest of the modules as well if its' still broken
void TremoKittyAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    float index = comboBoxThatHasChanged->getSelectedItemIndex();
    if (index == 5 || index == 6)
        audioProcessor.changeTremWaveManually(index);
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

void TremoKittyAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    auto settings = audioProcessor.globalProperties.getUserSettings();

    //Do for all sync buttons
    if (button == &TremSyncButton)
        syncButtonClicked(&TremSyncButton);
    else if (button == &PanSyncButton)
        syncButtonClicked(&PanSyncButton);
    else if (button == &FilterSyncButton)
        syncButtonClicked(&FilterSyncButton);
    else if (button == &ModSyncButton)
        syncButtonClicked(&ModSyncButton);
    else if (button == &defaultSkinButton)
    {
        myLNF.changeSkin(juce::Gui::MyLNF::skins::sDefault);
        background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
        changeLabelColours();
    }
    else if (button == &halloweenSkinButton)
    {
        myLNF.changeSkin(juce::Gui::MyLNF::skins::sHalloween);
        background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
        changeLabelColours();
    }
    else if (button == &christmasSkinButton)
    {
        myLNF.changeSkin(juce::Gui::MyLNF::skins::sChristmas);
        background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
        changeLabelColours();
    }
    else if (button == &spaceSkinButton)
    {
        myLNF.changeSkin(juce::Gui::MyLNF::skins::sSpace);
        background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
        changeLabelColours();
    }
    else if (button == &displayKittyButton)
    {
        shouldNotDisplayKitty = !settings->getBoolValue("DONTDISPLAYKITTY");
        settings->setValue("DONTDISPLAYKITTY", shouldNotDisplayKitty);
        background.setVisible(!shouldNotDisplayKitty);
    }
    else if (button == &setDefaultSkinButton)
    {
        
        switch (myLNF.currentSkin)
>>>>>>> Master
        {
            using namespace juce;
            std::vector<std::byte> result((size_t)stream.getTotalLength());
            stream.setPosition(0);
            [[maybe_unused]] const auto bytesRead = stream.read(result.data(), result.size());
            jassert(bytesRead == (ssize_t)result.size());
            return result;
        }

        const char* getMimeForExtension(const juce::String& extension)
        {
            using namespace juce;
            static const std::unordered_map<String, const char*> mimeMap =
            {
                { { "htm"   },  "text/html"                },
                { { "html"  },  "text/html"                },
                { { "txt"   },  "text/plain"               },
                { { "jpg"   },  "image/jpeg"               },
                { { "jpeg"  },  "image/jpeg"               },
                { { "svg"   },  "image/svg+xml"            },
                { { "ico"   },  "image/vnd.microsoft.icon" },
                { { "json"  },  "application/json"         },
                { { "png"   },  "image/png"                },
                { { "css"   },  "text/css"                 },
                { { "map"   },  "application/json"         },
                { { "js"    },  "text/javascript"          },
                { { "woff2" },  "font/woff2"               }
            };

            if (const auto it = mimeMap.find(extension.toLowerCase()); it != mimeMap.end())
                return it->second;

            jassertfalse;
            return "";
        }
    }
    
    TremoKittyAudioProcessorEditor::TremoKittyAudioProcessorEditor(TremoKittyAudioProcessor& p)
        : AudioProcessorEditor(&p), audioProcessor(p),
          webView(juce::WebBrowserComponent::Options{}.withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
                        .withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}
                        .withUserDataFolder(juce::File::getSpecialLocation(juce::File::tempDirectory))
                        .withBackgroundColour(juce::Colours::white))
                        .withResourceProvider([this](const auto& url) {return getResource(url); })
                        .withNativeIntegrationEnabled()
        )
    {
        juce::ignoreUnused(audioProcessor);

        addAndMakeVisible(webView);

        webView.goToURL(webView.getResourceProviderRoot());

        addAndMakeVisible(runJavaScriptButton);

        runJavaScriptButton.onClick = [this]() {
            constexpr auto JAVASCRIPT_TO_RUN{ "console.log(\"Hello from C++!\")" };
            webView.evaluateJavascript(
                JAVASCRIPT_TO_RUN,
                [](juce::WebBrowserComponent::EvaluationResult result)
                {
                    if (const auto* resultPtr = result.getResult())
                    {
                        std::cout << "javascript evaluation result: " << resultPtr->toString() << std::endl;
                    }
                }
            );
            };

        setResizable(true, true);
        setSize(400, 300);

        //background.setImage(myLNF.currentBgImage, juce::RectanglePlacement::stretchToFit);
        //background.setAlpha(0.1);
        //addAndMakeVisible(background);

        ////Getting the header rectangle image
        //auto kittyImage = juce::ImageCache::getFromMemory(BinaryData::TremoKittyBanner_png, BinaryData::TremoKittyBanner_pngSize);
        //if (!kittyImage.isNull())
        //    tremoKittyBanner.setImage(kittyImage, juce::RectanglePlacement::stretchToFit);
        //else
        //    jassert(!kittyImage.isNull());
        //addAndMakeVisible(tremoKittyBanner);

        //setUpSkinButtons();

        ////Header Label
        //createLabel("TremoKitty!", header);
        //header.setFont(juce::Font("Calibri", 20.f, juce::Font::bold));
        //header.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);

        ////PresetPanel
        //addAndMakeVisible(presetPanel);

        ////Got rid of master bypass cause kind of pointless
        //setUpTremoloSection();
        //setUpPannerSection();
        //setUpFilterSection();
        //setUpModSection();
        //changeLabelColours();

        //setSize(500, 550);
        //loadInitialState();
        //setLookAndFeel(&myLNF);
    }

    //Webview changes
    auto TremoKittyAudioProcessorEditor::getResource(const juce::String& url) -> std::optional<Resource>
    {
        static const auto resourceFileRoot = juce::File{ R"(C:\Users\Levi\Documents\JUCE projects\TremoKitty\Tremokitty\Source\ui\public)" };

        const auto resourceToRetrieve = url == "/" ? "index.html" : url.fromFirstOccurrenceOf("/", false, false);

        const auto resource = resourceFileRoot.getChildFile(resourceToRetrieve).createInputStream();

        if (resource)
        {
            const auto extension = resourceToRetrieve.fromLastOccurrenceOf(".", false, false);
            return Resource{ streamToVector(*resource), getMimeForExtension(extension) };
        }

        return std::nullopt;
    }

    void TremoKittyAudioProcessorEditor::loadInitialState()
    {
        auto settings = audioProcessor.globalProperties.getUserSettings();
        auto shouldNotDisplay = settings->getBoolValue("DONTDISPLAYKITTY");
        auto skinToLoad = settings->getIntValue("DEFAULTSKIN", 0);
        DBG("TOLOAD = " + std::to_string(skinToLoad));
        switch (skinToLoad)
        {
        case(0):
            break;
        case(1):
            break;
        case(2):
            break;
        case(3):
            break;
        default:
            DBG("Failed to assign skin.");
            break;
        }
        if (!shouldNotDisplay == 0)
        {
        }
        else
        {
        }
    }

    //DISGUSTING, ABSOLUTELY DISGUSTING, might have to do this for the rest of the modules as well if its' still broken
    void TremoKittyAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
    {
        float index = comboBoxThatHasChanged->getSelectedItemIndex();
        if (index == 5 || index == 6)
            audioProcessor.changeTremWaveManually(index);
    }

    void TremoKittyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
    {
       /* if (slider == &FilterCutoffSlider)
        {
            float newValue = slider->getValue();
            FilterCutoffLabel.setText("Cutoff = " + (std::to_string((int)juce::jmap(newValue, 20.f, 20000.f))) + " HZ", juce::NotificationType::dontSendNotification);
        }*/
    }

    void TremoKittyAudioProcessorEditor::buttonClicked(juce::Button* button)
    {
        auto settings = audioProcessor.globalProperties.getUserSettings();

    }

    void TremoKittyAudioProcessorEditor::changeLabelColours()
    {
       
    }

    TremoKittyAudioProcessorEditor::~TremoKittyAudioProcessorEditor()
    {

    }

    void TremoKittyAudioProcessorEditor::resized()
    {
        auto bounds = getLocalBounds();
        webView.setBounds(bounds.removeFromRight(getWidth() / 2));
        runJavaScriptButton.setBounds(bounds.removeFromTop(50).reduced(5));
    }
} //namespace
