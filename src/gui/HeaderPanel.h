#ifndef VINEX_HEADERPANEL_H
#define VINEX_HEADERPANEL_H

#include <JuceHeader.h>
#include "../service/PresetManager.h"
#include "PresetBrowser.h"

class HeaderPanel : public Component
{
public:
    explicit HeaderPanel(service::PresetManager& presetManager);
    HeaderPanel() = delete;

    void paint(Graphics& g) override;
    void resized() override;

private:
    PresetBrowser presetBrowser;
};

#endif //VINEX_HEADERPANEL_H
