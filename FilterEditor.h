/*
  ==============================================================================

    FilterEditor.h
    Created: 12 Nov 2019 11:32:01pm
    Author:  amayn

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "CustomLnF.h"

class FilterEditor : public Component
{
public:
	FilterEditor(OsctestAudioProcessor& p);
	~FilterEditor();

	void paint(Graphics&) override;
	void resized() override;
	//void comboBoxChanged(ComboBox* comboChanged);

	OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
	OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;
private:
	OsctestAudioProcessor& processor;
	lnf lf;

	Slider cutoffKnob;
	Slider resKnob;
	Slider driveKnob;
	Slider gainKnob;

	Array<Colour> customColours;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEditor)
};
