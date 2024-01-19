#include "PresetManager.h"

namespace service {

    const File PresetManager::defaultDirectory = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory)
            .getChildFile(ProjectInfo::companyName)
            .getChildFile(ProjectInfo::projectName);
    const String PresetManager::extension = "vnx";
    const String PresetManager::presetNameProperty = "presetName";

    PresetManager::PresetManager(AudioProcessorValueTreeState& apvts) : apvts(apvts)
    {
        if (!defaultDirectory.exists())
        {
            const auto result = defaultDirectory.createDirectory();
            if (result.failed())
            {
                DBG("Could not create preset directory: " + result.getErrorMessage());
                jassertfalse;
            }
        }

        refreshPresets();

        apvts.state.addListener(this);
        currentPreset.referTo(apvts.state.getPropertyAsValue(presetNameProperty, nullptr));
    }

    void PresetManager::savePreset(const String& presetName)
    {
        if (presetName.isEmpty())
            return;
        currentPreset.setValue(presetName);

        const auto stateXml = apvts.copyState().createXml();
        const auto presetFile = defaultDirectory.getChildFile(presetName + '.' + extension);

        if (!stateXml->writeTo(presetFile))
        {
            DBG("Could not create preset file: " + presetFile.getFullPathName());
            jassertfalse;
        }
    }

    void PresetManager::deletePreset(const String& presetName)
    {
        if (presetName.isEmpty())
            return;

        const auto presetFile = defaultDirectory.getChildFile(presetName + '.' + extension);
        if (!presetFile.existsAsFile() || !presetFile.deleteFile())
            return;

        currentPreset.setValue("");
    }

    void PresetManager::loadPreset(const String& presetName)
    {
        if (presetName.isEmpty())
            return;

        const auto presetFile = defaultDirectory.getChildFile(presetName + '.' + extension);
        if (!presetFile.existsAsFile())
            return;

        XmlDocument xml(presetFile);
        const auto presetValueTree = ValueTree::fromXml(*xml.getDocumentElement());

        apvts.replaceState(presetValueTree);
        currentPreset.setValue(presetName);
    }

    int PresetManager::loadNextPreset()
    {
        if (presets.isEmpty())
            return -1;

        const auto currentPresetIndex = presets.indexOf(currentPreset.toString());
        const auto nextPresetIndex = (currentPresetIndex + 1 > presets.size() - 1) ? 0 : currentPresetIndex + 1;
        loadPreset(presets.getReference(nextPresetIndex));
        return nextPresetIndex;
    }

    int PresetManager::loadPreviousPreset()
    {
        if (presets.isEmpty())
            return -1;

        const auto currentPresetIndex = presets.indexOf(currentPreset.toString());
        const auto previousPresetIndex = (currentPresetIndex - 1 < 0) ? presets.size() - 1 : currentPresetIndex - 1;
        loadPreset(presets.getReference(previousPresetIndex));
        return previousPresetIndex;
    }

    const StringArray* PresetManager::getPresets(bool refreshNeeded)
    {
        if (!refreshNeeded)
            return &presets;
        refreshPresets();
        return &presets;
    }

    void PresetManager::refreshPresets()
    {
        presets.clearQuick();
        const auto presetFiles = defaultDirectory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*." + extension);

        for (const auto& file : presetFiles)
        {
            presets.add(file.getFileNameWithoutExtension());
        }
    }

    String PresetManager::getCurrentPreset() const
    {
        return currentPreset.toString();
    }

    void PresetManager::valueTreeRedirected(ValueTree &treeWhichHasBeenChanged)
    {
        currentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));
    }
}