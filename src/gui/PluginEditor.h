#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "VinexLookAndFeel.h"
#include "Knob.h"
#include "OscSection.h"
#include "HeaderPanel.h"

//==============================================================================
/**
*/
class VinexAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VinexAudioProcessorEditor (VinexAudioProcessor&, juce::AudioProcessorValueTreeState& apvts);
    ~VinexAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    VinexAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& apvts;

    VinexLookAndFeel vinexLookAndFeel;

    HeaderPanel header;
    OscSection oscSection;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VinexAudioProcessorEditor)
};
