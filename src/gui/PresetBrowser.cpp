#include "PresetBrowser.h"
#include "../Constants.h"
#include "VinexColours.h"

PresetBrowser::PresetBrowser(service::PresetManager& presetManager) : presetManager(presetManager)
{
    buttons.add(new TextButton("Save"));
    buttons.add(new TextButton("Delete"));
    buttons.add(new TextButton("<"));
    buttons.add(new TextButton(">"));

    for (auto button: buttons)
    {
        button->setColour(TextButton::ColourIds::buttonColourId, vinex_colours::buttonBackground);
        button->setColour(ComboBox::ColourIds::outlineColourId, vinex_colours::buttonOutline);
        button->addListener(this);
        addAndMakeVisible(*button);
    }

    presetSelector.setColour(ComboBox::ColourIds::backgroundColourId, vinex_colours::comboBoxBackground);
    presetSelector.setColour(ComboBox::ColourIds::outlineColourId, vinex_colours::comboBoxOutline);
    presetSelector.addListener(this);
    addAndMakeVisible(presetSelector);

    loadPresetList();
}

void PresetBrowser::resized()
{
    auto bounds = getLocalBounds();

    FlexBox buttonsFlex;
    buttonsFlex.flexDirection = FlexBox::Direction::row;
    buttonsFlex.justifyContent = FlexBox::JustifyContent::spaceBetween;
    buttonsFlex.alignItems = FlexBox::AlignItems::flexStart;

    for (auto button: buttons)
    {
        buttonsFlex.items.add(FlexItem(*button)
                                      .withMinWidth(constants::presetBrowserButtonWidth)
                                      .withMinHeight(constants::presetBrowserButtonHeight));
    }

    FlexBox flex;
    flex.flexDirection = FlexBox::Direction::column;
    flex.justifyContent = FlexBox::JustifyContent::spaceBetween;

    flex.items.add(FlexItem(buttonsFlex)
                           .withMinWidth(bounds.getWidth())
                           .withMinHeight(constants::presetBrowserButtonHeight));
    flex.items.add(FlexItem(presetSelector)
                           .withMinWidth(bounds.getWidth())
                           .withMinHeight(constants::presetBrowserSelectorHeight));
    flex.performLayout(bounds);
}

void PresetBrowser::buttonClicked(Button* button)
{
    if (button == buttons[saveButton])
    {
        fileChooser = std::make_unique<FileChooser>("Save preset as",
                                                    service::PresetManager::defaultDirectory,
                                                    "*." + service::PresetManager::extension);
        fileChooser->launchAsync(FileBrowserComponent::saveMode, [&](const FileChooser &fileChooser)
        {
            const auto presetFile = fileChooser.getResult();
            presetManager.savePreset(presetFile.getFileNameWithoutExtension());
            loadPresetList();
        });
    } else if (button == buttons[deleteButton])
    {
        presetManager.deletePreset(presetManager.getCurrentPreset());
        loadPresetList();
    } else if (button == buttons[previousPresetButton])
    {
        presetSelector.setSelectedItemIndex(presetManager.loadPreviousPreset(), dontSendNotification);
    } else if (button == buttons[nextPresetButton])
        presetSelector.setSelectedItemIndex(presetManager.loadNextPreset(), dontSendNotification);
}

void PresetBrowser::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &presetSelector)
    {
        presetManager.loadPreset(presetSelector.getItemText(presetSelector.getSelectedItemIndex()));
    }
}

void PresetBrowser::loadPresetList()
{
    presetSelector.clear(dontSendNotification);
    const auto presets = presetManager.getPresets(true);
    const auto currentPreset = presetManager.getCurrentPreset();
    presetSelector.addItemList(*presets, 1);
    presetSelector.setSelectedItemIndex(presets->indexOf(currentPreset), dontSendNotification);
}
