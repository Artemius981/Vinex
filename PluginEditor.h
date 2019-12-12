/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Envelope.h"
#include "OscEditor.h"
#include "FilterEditor.h"

//==============================================================================
/**
*/

class OsctestAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    OsctestAudioProcessorEditor (OsctestAudioProcessor&);
    ~OsctestAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> masterGainAttachment;

private:
	//void comboBoxChanged(ComboBox* box);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OsctestAudioProcessor& processor;

	//ComboBox oscTypeBox;
	Slider masterGainKnob;

	Envelope envGui;
	OscEditor osc1Gui;
	OscEditor osc2Gui;
	FilterEditor filterGui;

	lnf lf;

	Label logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OsctestAudioProcessorEditor)
};
