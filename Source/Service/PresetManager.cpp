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
        //Create a default preset Directory if it doesn't exist, also should create a default preset if it dont exist
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

    juce::File PresetManager::getPresetFile(const juce::String& presetPath) const
    {
        auto cleanedPath = presetPath.replaceCharacter('\\', '/');

        if (cleanedPath.endsWith("." + extension))
            cleanedPath = cleanedPath.dropLastCharacters(extension.length() + 1);

        return defaultDirectory.getChildFile(cleanedPath + "." + extension);
    }

    void Service::PresetManager::savePreset(const juce::String& presetName, const juce::String& categoryName)
    {
        if(presetName.isEmpty())
        {
            return;
        }

        const auto categoryDirectory = categoryName.isEmpty()
            ? defaultDirectory
            : defaultDirectory.getChildFile(categoryName);

        if (!categoryDirectory.exists())
        {
            const auto result = categoryDirectory.createDirectory();

            if (result.failed())
            {
                DBG("Could not create preset category directory: "
                    + categoryDirectory.getFullPathName()
                    + " - "
                    + result.getErrorMessage());

                jassertfalse;
                return;
            }
        }

        const auto xml = valueTreeState.copyState().createXml();

        const auto presetFile = categoryDirectory.getChildFile(presetName + "." + extension);

        if (!xml->writeTo(presetFile))
        {
            DBG("Could Not Create Preset File + " + presetFile.getFullPathName());
            jassertfalse;
        }

        const auto savedPresetPath = categoryName.isEmpty()
            ? presetName
            : categoryName + "/" + presetName;

        currentPreset.setValue(savedPresetPath);

        const auto presets = getAllPresets();
        updateAPVTS(presets.indexOf(savedPresetPath));
    }

    void Service::PresetManager::deletePreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
        {
            return;
        }

        const auto presetFile = getPresetFile(presetName);
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
    }

    void Service::PresetManager::loadPreset(const juce::String& presetName)
    {
        if (presetName.isEmpty())
        {
            return; 
        }

        const auto presetFile = getPresetFile(presetName);

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

    void Service::PresetManager::loadPreset(const int index)
    {
        const auto presetList = getAllPresets();

        if (index < 0 || index >= presetList.size())
            return;

        loadPreset(presetList[index]);
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
        auto* param = valueTreeState.getParameter("PRESETINDEX");

        if (param != nullptr)
        {
            param->beginChangeGesture();
            param->setValueNotifyingHost(param->convertTo0to1((float)index));
            param->endChangeGesture();
        }
    }

    void PresetManager::setCurrentPresetName(juce::String newName)
    {
        currentPreset.setValue(newName);
    }

    juce::StringArray Service::PresetManager::getAllPresets() const
    {
        juce::StringArray presets;

        const auto fileArray = defaultDirectory.findChildFiles(
            juce::File::findFiles,
            true,
            "*." + extension
        );

        for (const auto& file : fileArray)
        {
            auto relativePath = file.getRelativePathFrom(defaultDirectory)
                .replaceCharacter('\\', '/');

            relativePath = relativePath.upToLastOccurrenceOf(
                "." + extension,
                false,
                false
            );

            presets.add(relativePath);
        }

        presets.sort(true);
        return presets;
    }

    juce::String Service::PresetManager::getCurrentPreset() const
    {
        return currentPreset.toString();
    }

    int PresetManager::getCurrentPresetIndex() const
    {
        const auto presets = getAllPresets();
        if (presets.isEmpty())
            return -1;
        const auto currentIndex = presets.indexOf(currentPreset.toString());

        return currentIndex;
    }

    void PresetManager::valueTreeRedirected(juce::ValueTree& treeWhichHasBeenChanged)
    {
        treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr);
    }
}
