#include "../PluginProcessor.h"
#include "PluginEditor.h"
#include "VinexColours.h"
#include "../Constants.h"

//==============================================================================
VinexAudioProcessorEditor::VinexAudioProcessorEditor (VinexAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p), audioProcessor (p), apvts(apvts)
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
