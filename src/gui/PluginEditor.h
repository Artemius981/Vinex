#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "VinexLookAndFeel.h"
#include "Knob.h"

//==============================================================================
/**
*/
class VinexAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VinexAudioProcessorEditor (VinexAudioProcessor&);
    ~VinexAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    VinexAudioProcessor& audioProcessor;

    VinexLookAndFeel vinexLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VinexAudioProcessorEditor)
};
