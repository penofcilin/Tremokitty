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

        addAndMakeVisible(webView);

        #if TREMOKITTY_DEV_UI
            webView.goToURL("http://localhost:5173");
        #else
            webView.goToURL(webView.getResourceProviderRoot());
        #endif

            // CPP -> js 1: Webview Evaluate Javascript
            constexpr auto js = "console.log(\"hello from cpp!\")";
            webView.evaluateJavascript(js,
                                       [](juce::WebBrowserComponent::EvaluationResult result) {
                                           if (const auto* resultPtr = result.getResult())
                                               DBG("Evaluation result in cpp: " + resultPtr->toString());
                                           else 
                                               DBG("Evaluation failed");
                                       });

        setResizable(false,false);
        setSize(600, 580);
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

    void TremoKittyAudioProcessorEditor::sliderChanged(juce::var info)
    {
        const juce::String& sliderID =  info.getProperty("sliderID", 0).toString();
        float newValue = info.getProperty("newValue", -1); //if something goes wrong, hopefully this will crash it while in development. Could make it 0, but that will probably get confusing.


         audioProcessor.apvts.getRawParameterValue(sliderID)->store(newValue);
         float storedVal = audioProcessor.apvts.getRawParameterValue(sliderID)->load();
         juce::String output = juce::String("Your slider is called " + sliderID + " and it's new value stored in apvts is " + juce::String(storedVal));
         DBG(output);
    }

    void TremoKittyAudioProcessorEditor::dropdownCommit(juce::var info)
    {
        const juce::String& dropdownID = info.getProperty("dropdownID", "Null").toString();
        const int newValue = info.getProperty("newValue", -1); //Choices are stored as integers

        audioProcessor.apvts.getRawParameterValue(dropdownID)->store(newValue);

        float storedVal = audioProcessor.apvts.getRawParameterValue(dropdownID)->load();
        juce::String output = juce::String("Your dropdown is called " + dropdownID + " and it's new value stored in apvts is " + juce::String(storedVal));
        DBG(output);

    }

    //Handle each button on it's own
    void TremoKittyAudioProcessorEditor::buttonClicked(juce::var info)
    {
        const juce::String& buttonID = info.getProperty("buttonID", "null").toString();
        const int isCheckBox = info.getProperty("isCheckBox", -1);

        //If it's a togglebutton easy as shit, just store the inverse of the current parameter
        if (isCheckBox) {
            audioProcessor.apvts.getRawParameterValue(buttonID)->store(!audioProcessor.apvts.getRawParameterValue(buttonID)->load());
            DBG("Toggled " + buttonID + " new value: " + juce::String(audioProcessor.apvts.getRawParameterValue(buttonID)->load()));
        }
        DBG("clicked " + buttonID);

        //todo: write explicit handlers for each unique button
    }

    void TremoKittyAudioProcessorEditor::formSubmitted(juce::var info)
    {
        const auto& formID = info.getProperty("formID", "null").toString();
        juce::var dataVar = info.getProperty("data", "null");
        auto* dataObj = dataVar.getDynamicObject();


        if (formID == "savePresetForm") {
            for (const auto& entry : dataObj->getProperties())
            {
                juce::String key = entry.name.toString();
                juce::String value = entry.value.toString();

                DBG(key + " = " + value);
            }

        }
            
    }

    void TremoKittyAudioProcessorEditor::toggleGroupChanged(juce::var info)
    {
        const auto& groupID = info.getProperty("togglegroupID", "null").toString();
        juce::var newVal = info.getProperty("newValue", "null");
        DBG("Togled: " + groupID + juce::String(newVal.toString()));


        //Store value. In theory, pass an integer index reflecting choice. For instance click on sine in react -> 0 is passed, apvts gets the tremwave parameter, and sets it to the same index, which should be the same. Later on may need to change this, if miscellaneous togglegroups are incorporated (misc meaning the group does not reflect the state of some parameter).
        audioProcessor.apvts.getRawParameterValue(groupID)->store(newVal);

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

    }

    void TremoKittyAudioProcessorEditor::resized()
    {
        auto bounds = getLocalBounds();
        webView.setBounds(bounds);
    }
} //namespace
