#include "HeaderPanel.h"
#include "VinexColours.h"
#include "../Constants.h"

HeaderPanel::HeaderPanel(service::PresetManager& presetManager) : presetBrowser(presetManager)
{
    addAndMakeVisible(presetBrowser);
}

void HeaderPanel::paint(Graphics& g)
{
    g.fillAll(vinex_colours::headerBackground);
}

void HeaderPanel::resized()
{
    presetBrowser.setBounds(6, 5, constants::presetBrowserWidth, constants::presetBrowserHeight);
}
