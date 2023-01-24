#include "../PluginProcessor.h"
#include "PluginEditor.h"
#include "VinexColours.h"
#include "../Constants.h"

//==============================================================================
VinexAudioProcessorEditor::VinexAudioProcessorEditor (VinexAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (constants::pluginWidth, constants::pluginHeight);
    setLookAndFeel(&vinexLookAndFeel);
}

VinexAudioProcessorEditor::~VinexAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void VinexAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (vinex_colours::background);
}

void VinexAudioProcessorEditor::resized()
{
}
