#include "../PluginProcessor.h"
#include "PluginEditor.h"
#include "VinexColours.h"
#include "../Constants.h"

//==============================================================================
VinexAudioProcessorEditor::VinexAudioProcessorEditor (VinexAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p), audioProcessor (p), apvts(apvts), header(p.getPresetManager()), oscSection(osc1, p.getWavetableManager(osc1), apvts)
{
    setSize (constants::pluginWidth, constants::pluginHeight);
    setLookAndFeel(&vinexLookAndFeel);

    addAndMakeVisible(&header);
    addAndMakeVisible(&oscSection);
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
    auto bounds = getLocalBounds();

    header.setBounds(bounds.removeFromTop(constants::headerHeight));
    // TODO: remove hardcoded constants
    oscSection.setBounds(3, 70, constants::oscSectionWidth, constants::oscSectionHeight);
}
