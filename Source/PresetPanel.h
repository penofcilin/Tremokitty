/*
  ==============================================================================

    PresetPanel.h
    Created: 2 Oct 2022 10:57:45pm
    Author:  lmend

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MyLNF.h"
namespace Gui
{
    class PresetPanel : public juce::Component, juce::Button::Listener, juce::ComboBox::Listener
    {
    public:
        PresetPanel(Service::PresetManager& pm, juce::Gui::MyLNF* lnf) : presetManager(pm)
        {
            juce::LookAndFeel::setDefaultLookAndFeel(&LNF);
            
            //I decided to get rid of the delete button because its kind of a weird thing to have. You don't want your customer to work really hard on a preset only to nudge the delete button and lose all their progress. Or be on the default and accidentally delete it, etc. And if they really want to, they can go into the preset folder themself and delete the file.
            //configureButton(deleteButton, "Delete Preset"); //Might get rid of this/change at some point
            configureButton(previousPresetButton, "<");
            configureButton(nextPresetButton, ">");
            configureButton(saveButton, "Save Preset");
            configureButton(OpenPresetFolderButton, "Open Preset Folder");
            configureButton(defaultPresetButton, "R");

            presetList.setTextWhenNothingSelected("None");
            presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            addAndMakeVisible(presetList);
            presetList.addListener(this);

            loadPresetList();
            presetList.setText(" ", juce::NotificationType::dontSendNotification);
        }

        ~PresetPanel()
        {
            saveButton.removeListener(this);
            deleteButton.removeListener(this);
            previousPresetButton.removeListener(this);
            nextPresetButton.removeListener(this);
            presetList.removeListener(this);
        }

        void changePresetIndex(const int newIndex)
        {
            presetList.setSelectedItemIndex(newIndex, juce::NotificationType::dontSendNotification);
        }

    private:
        juce::TextButton saveButton, deleteButton, previousPresetButton, nextPresetButton, OpenPresetFolderButton, defaultPresetButton;
        juce::Gui::MyLNF LNF;
        juce::ComboBox presetList;
        Service::PresetManager& presetManager;
        std::unique_ptr<juce::FileChooser> fileChooser;

        void buttonClicked(juce::Button* button) override
        {
            if (button == &saveButton)
            {
                fileChooser = std::make_unique<juce::FileChooser>("Please enter a name for your preset...", Service::PresetManager::defaultDirectory, "*." + Service::PresetManager::extension );

                fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser)
                                         {
                                             const auto resultFile = chooser.getResult();
                                             presetManager.savePreset(resultFile.getFileNameWithoutExtension());
                                             loadPresetList();
                                         });
                

            }
            if (button == &previousPresetButton)
            {
                const auto index = presetManager.loadPreviousPreset();
                presetList.setSelectedItemIndex(index, juce::NotificationType::dontSendNotification);
            }
            if (button == &nextPresetButton)
            {
                const auto index = presetManager.loadNextPreset();
                presetList.setSelectedItemIndex(index, juce::NotificationType::dontSendNotification);
            }
            if (button == &OpenPresetFolderButton)
            {
                presetManager.defaultDirectory.getChildFile(presetManager.defaultPresetName + "." + presetManager.extension).revealToUser();
            }
            if (button == &defaultPresetButton)
            {
                presetManager.loadPreset(presetManager.defaultPresetName);
                const auto index = presetManager.getAllPresets().indexOf(presetManager.defaultPresetName);
                presetList.setSelectedItemIndex(index, juce::NotificationType::dontSendNotification);
            }
        }


        void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override
        {
            if (comboBoxThatHasChanged == &presetList)
            {
                DBG("GUI combobox changed");
                presetManager.loadPreset(presetList.getItemText(presetList.getSelectedItemIndex()));
            }
        }

        void loadPresetList()
        {
            presetList.clear(juce::dontSendNotification);
            const auto allPresets = presetManager.getAllPresets();
            const auto currentPreset = presetManager.getCurrentPreset();
            presetList.addItemList(allPresets, 1);
            presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset),juce::NotificationType::dontSendNotification);
        }

        void resized() override
        {
            const auto container = getLocalBounds().reduced(4);
            auto bounds = container;

            saveButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.14f)));
            previousPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.07f)).reduced(1));
            presetList.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.5f)).reduced(1));
            nextPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.07f)).reduced(1));
            defaultPresetButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.05f)).reduced(1));
            OpenPresetFolderButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.17)).reduced(1));
        }
        void configureButton(juce::Button& button, const juce::String& buttonText)
        {
            button.setButtonText(buttonText);
            button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            addAndMakeVisible(button);
            button.addListener(this);
        }


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel);
    };
}