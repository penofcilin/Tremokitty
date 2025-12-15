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
                .withInitialisationData("pluginName", "DevKitty101")
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

    void TremoKittyAudioProcessorEditor::sliderChanged(juce::var info)
    {
        const int sliderID = (int) info.getProperty("sliderID", 0);
        float newValue = info.getProperty("newValue", -1); //if something goes wrong, hopefully this will crash it while in development. Could make it 0, but that will probably get confusing.

        if (sliderID > PARAMETER_COUNT) {
            DBG("Invalid param!");
            return;
        }

        juce::String sliderIDString = parameterIDStrings[sliderID];

        DBG("You just changed the: " + juce::String(parameterIDStrings[sliderID]));
        DBG("The new value should be: " + juce::String(newValue));

         audioProcessor.apvts.getRawParameterValue(parameterIDStrings[sliderID])->store(newValue);
         float newstored = audioProcessor.apvts.getRawParameterValue(parameterIDStrings[sliderID])->load();
         DBG("The parameter within APVTS is now: " + juce::String(newstored));
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
        webView.setBounds(bounds);
    }
} //namespace
