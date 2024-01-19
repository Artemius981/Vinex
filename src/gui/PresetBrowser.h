#ifndef VINEX_PRESETBROWSER_H
#define VINEX_PRESETBROWSER_H

#include <JuceHeader.h>
#include "../service/PresetManager.h"

class PresetBrowser : public Component, Button::Listener, ComboBox::Listener
{
public:
    explicit PresetBrowser(service::PresetManager& presetManager);

    void resized() override;

private:
    enum buttonIds
    {
        saveButton,
        deleteButton,
        previousPresetButton,
        nextPresetButton
    };

    void buttonClicked(Button* button) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void loadPresetList();

    service::PresetManager& presetManager;
    std::unique_ptr<FileChooser> fileChooser;

    OwnedArray<Button> buttons;

    ComboBox presetSelector;
};

#endif //VINEX_PRESETBROWSER_H
