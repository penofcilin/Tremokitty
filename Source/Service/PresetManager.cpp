/*
  ==============================================================================

    PresetManager.cpp
    Created: 3 Oct 2022 10:41:22pm
    Author:  lmend

  ==============================================================================
*/

#include "PresetManager.h"
namespace Service
{
    const juce::File PresetManager::defaultDirectory{ juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getChildFile(ProjectInfo::projectName).getChildFile("Presets")};
    const juce::String PresetManager::extension{ "kty" };
    const juce::String PresetManager::defaultPresetName{ "Default" };
    const juce::String PresetManager::presetNameProperty{ "presetName" };

    //Constructah
    Service::PresetManager::PresetManager(juce::AudioProcessorValueTreeState& valueTreeState) : valueTreeState(valueTreeState)
    {
        //Create a default preset Directory if it doesn't exist
        if (!defaultDirectory.exists())
        {
            const auto result = defaultDirectory.createDirectory();
            if (result.failed())
            {
                DBG("Could not create preset directory! " + result.getErrorMessage());
                jassertfalse;
            }
        }
        valueTreeState.state.addListener(this);
    }

    void Service::PresetManager::savePreset(const juce::String& presetName)
    {
        if(presetName.isEmpty())
        {
            return;
        }
        currentPreset.setValue(presetName);
        const auto xml = valueTreeState.copyState().createXml();
        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!xml->writeTo(presetFile))
        {
            DBG("Could Not Create Preset File + " + presetFile.getFullPathName());
            jassertfalse;
        }
    }

    void Service::PresetManager::deletePreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
        {
            return;
        }

        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!presetFile.existsAsFile())
        {
            DBG("Preset file " + presetFile.getFullPathName() + " does not exist.");
            jassertfalse;
            return;
        }
        
        if (!presetFile.deleteFile())
        {
            DBG("Failed to delete file: " + presetFile.getFullPathName());
            jassertfalse;
            return;
        }

        try
        {
            loadPreset("Default");
            updateAPVTS(0);
        }
        catch (const std::exception& e)
        {
            DBG("Failed to load default preset. Could be that there is no preset named 'Default'.");
            currentPreset.setValue("");
        }
    }

    void Service::PresetManager::loadPreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
        {
            return; 
        }

        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);

        if (!presetFile.existsAsFile())
        {
            DBG("Preset file " + presetFile.getFullPathName() + " does not exist.");
            jassertfalse;
            return;
        }

        juce::XmlDocument xmlDocument{ presetFile };
        const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDocument.getDocumentElement());

        valueTreeState.replaceState(valueTreeToLoad);
        currentPreset.setValue(presetName);
        const auto presets = getAllPresets();
        updateAPVTS(presets.indexOf(presetName));

    }

    int Service::PresetManager::loadNextPreset()
    {
        const auto presets = getAllPresets();
        if (presets.isEmpty())
            return -1;
        const auto currentIndex = presets.indexOf(currentPreset.toString());

        const auto nextIndex = currentIndex + 1 > (presets.size()-1) ? 0 : currentIndex+1;
        loadPreset(presets.getReference(nextIndex));
        currentPreset.setValue(presets[nextIndex]);
        updateAPVTS(nextIndex);
        return nextIndex;
    }

    int Service::PresetManager::loadPreviousPreset()
    {
        const auto presets = getAllPresets();
        if (presets.isEmpty())
            return -1;
        const auto currentIndex = presets.indexOf(currentPreset.toString());
        const auto previousIndex = currentIndex - 1 < 0 ? presets.size() - 1 : currentIndex - 1;
        loadPreset(presets.getReference(previousIndex));
        currentPreset.setValue(presets[previousIndex]);
        updateAPVTS(previousIndex);
        return previousIndex;
    }

    void PresetManager::updateAPVTS(const int index)
    {
        valueTreeState.getRawParameterValue("PRESETINDEX")->store(index);
    }

    juce::StringArray Service::PresetManager::getAllPresets() const
    {
        juce::StringArray presets;
        const auto fileArray = defaultDirectory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*." + extension);
        for (const auto& file : fileArray)
        {
            presets.add(file.getFileNameWithoutExtension());
        }

        return presets;
    }

    juce::String Service::PresetManager::getCurrentPreset() const
    {
        return currentPreset.toString();
    }

    void PresetManager::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
    {
        treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr);
    }
}