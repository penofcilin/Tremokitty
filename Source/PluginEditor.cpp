/*
  ==============================================================================

    This file contains the editor baby 

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define TREMOKITTY_DEV_UI 1

namespace kitty_editor
{
    namespace {
        auto streamToVector(juce::InputStream& stream)
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
        : AudioProcessorEditor(&p),
        audioProcessor(p),
        webView(juce::WebBrowserComponent::Options{}
                .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
                .withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}
                                        .withUserDataFolder(juce::File::getSpecialLocation(juce::File::tempDirectory))
                                        .withBackgroundColour(juce::Colours::white))
                .withResourceProvider([this](const auto& url) {return getResource(url); })
                .withNativeIntegrationEnabled()
                .withInitialisationData("Presets", convertPresetNames(p.PresetNames))
                .withNativeFunction(
                    juce::Identifier{ "testNativeFunction" },
                    [this](const juce::Array<juce::var>& args,
                           juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                               testNativeFunction(args, std::move(completion)); }
                )
                .withNativeFunction("ProvideState", [this](const juce::Array<juce::var>& args, juce::WebBrowserComponent::NativeFunctionCompletion completion) {
                    prepareAPVTSState(std::move(completion)); }
                )
                .withEventListener("SliderChanged",
                                   [this](juce::var info) {
                                       sliderChanged(info);
                                   })
                .withEventListener("DropdownCommit",
                                    [this](juce::var info) {
                                        dropdownCommit(info);
                                    })
                .withEventListener("ButtonClicked",
                                    [this](juce::var info) {
                                        buttonClicked(info);
                                    })
                .withEventListener("FormSubmitted",
                                    [this](juce::var info) {
                                        formSubmitted(info);
                                   })
                .withEventListener("TogglegroupChanged",
                                    [this](juce::var info) {
                                        toggleGroupChanged(info);
                                    })
                            )
    {
        juce::ignoreUnused(audioProcessor);

        startTimer(5);
        addAndMakeVisible(webView);
       


        #if TREMOKITTY_DEV_UI
            webView.goToURL("http://localhost:5173");
        #else
            webView.goToURL(webView.getResourceProviderRoot());
        #endif

        setResizable(false,false);
        setSize(730, 600);
    }

    //Webview changes

    auto TremoKittyAudioProcessorEditor::getResource(const juce::String& url) -> std::optional<Resource>
    {
        juce::String dir;

        #if JUCE_WINDOWS
            dir = R"(C:\Users\Levi\Documents\JUCE projects\TremoKitty\Tremokitty\Source\ui\public)";
        #elif JUCE_MAC
            dir = "/Users/levi/Tremokitty/Source/ui/public";
        #else
            DBG("Unsupported platform");
        #endif

            
        static const auto resourceFileRoot = juce::File{dir};

        auto resourceToRetrieve = url;

        // Strip scheme (e.g. juce-resource://root)
        resourceToRetrieve = resourceToRetrieve.fromFirstOccurrenceOf("://", false, false);

        // Strip leading slash
        resourceToRetrieve = resourceToRetrieve.fromFirstOccurrenceOf("/", false, false);

        // Default document
        if (resourceToRetrieve.isEmpty())
            resourceToRetrieve = "index.html";


        const auto resource = resourceFileRoot.getChildFile(resourceToRetrieve).createInputStream();
        DBG("WebView URL: " + url);
        DBG("Resolved resource: " + resourceToRetrieve);

        if (resource)
        {
            const auto extension = resourceToRetrieve.fromLastOccurrenceOf(".", false, false);
            return Resource{ streamToVector(*resource), getMimeForExtension(extension) };
        }

        return std::nullopt;
    }

    void TremoKittyAudioProcessorEditor::timerCallback()
    {
        float filterLFOValue = audioProcessor.filterLFOCurrentPosition.load();
        float tremLFOValue = audioProcessor.tremLFOCurrentPosition.load();
        float panLFOValue = audioProcessor.panLFOCurrentPosition.load();
        float modLFOValue = audioProcessor.modLFOCurrentPosition.load();

        emitFrontendEvent("FilterLFOUpdate", juce::var(filterLFOValue));
        emitFrontendEvent("TremLFOUpdate", juce::var(tremLFOValue));
        emitFrontendEvent("PanLFOUpdate", juce::var(panLFOValue));
        emitFrontendEvent("ModLFOUpdate", juce::var(modLFOValue));

        auto paramUpdates = audioProcessor.getChangedParameters();
        if (paramUpdates.empty())
            return;

        juce::DynamicObject::Ptr obj = new juce::DynamicObject();

        for (const auto& u : paramUpdates)
            obj->setProperty(u.id, u.value);
    }

    void TremoKittyAudioProcessorEditor::emitFrontendEvent(const juce::String& identifier, juce::var value)
    {
        webView.emitEventIfBrowserIsVisible(juce::Identifier(identifier), value);
    }

    //This function is called by the GUI.
    void TremoKittyAudioProcessorEditor::testNativeFunction(const juce::Array<juce::var>& args, juce::WebBrowserComponent::NativeFunctionCompletion completion)
    {
        juce::String concatenatedArgs;
        for (const auto& arg : args) {
            concatenatedArgs += arg.toString();
        }

        // CPP -> JS 2: emit event
        webView.emitEventIfBrowserIsVisible("ExampleEvent", 42);

        DBG("Pressed a javascript button bro: " + concatenatedArgs);
        completion("Native function callback: OK!");
    }

    juce::Array<juce::var> TremoKittyAudioProcessorEditor::convertPresetNames(juce::StringArray& names)
    {
        juce::Array<juce::var> presetVars;

        for (const auto& name : names)
            presetVars.add(name);

        return presetVars;
    }

    //Called from the initializer list
    void TremoKittyAudioProcessorEditor::prepareAPVTSState(
        juce::WebBrowserComponent::NativeFunctionCompletion completion)
    {
        auto* obj = new juce::DynamicObject();

        DBG("Providing initial state");

        audioProcessor.getPresetManager().loadPreset("Default"); //Set preset to default

        for (const auto& id : parameterIDs)
        {
            if (auto* param = audioProcessor.apvts.getParameter(id))
            {
                // ALWAYS normalized [0..1]
                obj->setProperty(id, param->getValue());
            }
        }

        completion(juce::var(obj));
    }

    //Called from cpp
    juce::var TremoKittyAudioProcessorEditor::prepareAPVTSState()
    {
        auto* obj = new juce::DynamicObject();

        for (const auto& id : parameterIDs)
        {
            obj->setProperty(id, audioProcessor.apvts.getRawParameterValue(id)->load());
        }
        return obj;
    }

    void TremoKittyAudioProcessorEditor::updateUI()
    {
        const auto& state = prepareAPVTSState();

        emitFrontendEvent("UpdateUI", state);
    }

    void TremoKittyAudioProcessorEditor::sliderChanged(juce::var info)
    {
        const juce::String sliderID =
            info.getProperty("sliderID", "Null").toString();

        const float incoming =
            (float)info.getProperty("newValue", 0.0f);

        if (auto* param = audioProcessor.apvts.getParameter(sliderID))
        {
            // CHOICE PARAM (slider acting as discrete selector)
            if (auto* choice =
                dynamic_cast<juce::AudioParameterChoice*>(param))
            {
                const int index =
                    juce::jlimit(0,
                                 choice->choices.size() - 1,
                                 (int)incoming);

                const float normalized =
                    choice->convertTo0to1(index);

                DBG("CHANGED A CHOICE PARAM, VALUE " << normalized);

                choice->setValueNotifyingHost(normalized);
            }

            // BOOL PARAM (slider acting as toggle)
            else if (auto* boolean = dynamic_cast<juce::AudioParameterBool*>(param))
            {
                const bool next = incoming > 0.5f;
                boolean->setValueNotifyingHost(next ? 1.0f : 0.0f);
            }

            // INT PARAM
            else if (auto* intParam =
                     dynamic_cast<juce::AudioParameterInt*>(param))
            {
                const int value =
                    juce::jlimit(intParam->getRange().getStart(),
                                 intParam->getRange().getEnd(),
                                 (int)std::round(incoming));

                const float normalized =
                    intParam->convertTo0to1(value);

                intParam->setValueNotifyingHost(normalized);

                DBG("Slider->Int " << sliderID
                    << " value=" << value);
            }

            // FLOAT PARAM (normal case)
            else
            {
                const float normalized =
                    juce::jlimit(0.0f, 1.0f, incoming);

                param->setValueNotifyingHost(normalized);

                DBG("Slider->Float " << sliderID
                    << " value=" << normalized);
            }
        }
        else
        {
            DBG("ERROR: sliderCommit param not found: " << sliderID);
        }
    }

    void TremoKittyAudioProcessorEditor::dropdownCommit(juce::var info)
    {
        const juce::String dropdownID =
            info.getProperty("dropdownID", "Null").toString();

        const int newIndex =
            (int)info.getProperty("newValue", -1);

        DBG("Selected from dropdown " << dropdownID << " New value (index) = " << newIndex);

        if (auto* param = audioProcessor.apvts.getParameter(dropdownID))
        {
            if (auto* choice =
                dynamic_cast<juce::AudioParameterChoice*>(param))
            {
                const float normalized =
                    choice->convertTo0to1(newIndex);

                choice->setValueNotifyingHost(normalized);

                DBG("Dropdown " << dropdownID
                    << " set to index " << newIndex
                    << " (normalized " << normalized << ")");
            }
            else
            {
                DBG("ERROR: " << dropdownID << " is not an AudioParameterChoice");
            }
        }
        else // If it's not a specific parameter dropdown ie. preset menu
        {
            if (dropdownID == "presetDropdown")
            {
                audioProcessor.getPresetManager().loadPreset(newIndex);
                updateUI();
            }
            else
            {
                DBG("ERROR: Parameter not found: " << dropdownID);
            }
        }
    }

    //Handle each button on it's own
    void TremoKittyAudioProcessorEditor::buttonClicked(juce::var info)
    {
        const juce::String buttonID =
            info.getProperty("buttonID", "null").toString();

        const bool isCheckBox =
            (bool)info.getProperty("isCheckBox", false);

        if (isCheckBox)
        {
            if (auto* param = audioProcessor.apvts.getParameter(buttonID))
            {
                if (auto* boolParam =
                    dynamic_cast<juce::AudioParameterBool*>(param))
                {
                    const bool current =
                        boolParam->get();

                    const bool next = !current;

                    // Bool params are already normalized (0 or 1)
                    boolParam->setValueNotifyingHost(next ? 1.0f : 0.0f);

                    DBG("Toggled " << buttonID
                        << " new value: " << (next ? "true" : "false"));
                }
                else
                {
                    DBG("ERROR: " << buttonID << " is not an AudioParameterBool");
                }
            }
            else
            {
                DBG("ERROR: Parameter not found: " << buttonID);
            }
        }
        else
        {
            DBG("clicked " + buttonID);
            if (buttonID == "RESETPRESETBUTTON") {
                audioProcessor.getPresetManager().loadPreset("Default");
                updateUI();
            }
            else if (buttonID == "NEXTPRESETBUTTON") {
                audioProcessor.getPresetManager().loadNextPreset();
                updateUI();
            }
            else if (buttonID == "PREVIOUSPRESETBUTTON") {
                audioProcessor.getPresetManager().loadPreviousPreset();
                updateUI();
            }
        }
    }

    void TremoKittyAudioProcessorEditor::formSubmitted(juce::var info)
    {
        const auto& formID = info.getProperty("formID", "null").toString();
        juce::var dataVar = info.getProperty("data", "null");
        auto* dataObj = dataVar.getDynamicObject();

        DBG("Submitted a form: " << formID);
       


        if (formID == "SAVEPRESETBUTTON") {
            for (const auto& entry : dataObj->getProperties())
            {
                juce::String key = entry.name.toString();
                juce::String value = entry.value.toString();

                if(key == "PresetName")
                    audioProcessor.getPresetManager().savePreset(value);
           
                DBG(key + " = " + value);
            }
        }
    }

    void TremoKittyAudioProcessorEditor::toggleGroupChanged(juce::var info)
    {
        const auto& groupID = info.getProperty("togglegroupID", "null").toString();
        juce::var newVal = info.getProperty("newValue", "null");
        DBG("Togled: " + groupID + ", new value: " + juce::String(newVal.toString()));


        //Store value. In theory, pass an integer index reflecting choice. For instance click on sine in react -> 0 is passed, apvts gets the tremwave parameter, and sets it to the same index, which should be the same. Later on may need to change this, if miscellaneous togglegroups are incorporated (misc meaning the group does not reflect the state of some parameter).
        audioProcessor.apvts.getRawParameterValue(groupID)->store(newVal);
        audioProcessor.parameterChanged(groupID, newVal);
       DBG("Currenrtly stored in " + groupID + " " + juce::String(audioProcessor.apvts.getRawParameterValue(groupID)->load()));
    }

    //DISGUSTING, ABSOLUTELY DISGUSTING, might have to do this for the rest of the modules as well if its' still broken
    /*void TremoKittyAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
    {
        float index = comboBoxThatHasChanged->getSelectedItemIndex();
        if (index == 5 || index == 6)
            audioProcessor.changeTremWaveManually(index);
    }*/


    TremoKittyAudioProcessorEditor::~TremoKittyAudioProcessorEditor()
    {
        stopTimer();
    }

    void TremoKittyAudioProcessorEditor::resized()
    {
        auto bounds = getLocalBounds();
        webView.setBounds(bounds);
    }
} //namespace
