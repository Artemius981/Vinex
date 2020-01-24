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

class FilterEditor : public Component, ComboBox::Listener
{
public:
	FilterEditor(OsctestAudioProcessor& p);
	~FilterEditor();

	void paint(Graphics&) override;
	void resized() override;
	void comboBoxChanged(ComboBox* comboChanged);

	OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
	OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;
	OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> comboBoxAttachments;
private:
	OsctestAudioProcessor& processor;
	lnf lf;

	Slider cutoffKnob;
	Slider resKnob;
	Slider driveKnob;
	Slider gainKnob;
	Slider routeValue;

	ComboBox typeMenu;

	ToggleButton routeOsc1;
	ToggleButton routeOsc2;
	ToggleButton routeMaster;
	// labels for route buttons
	Label routeOsc1Label;
	Label routeOsc2Label;
	Label routeMasterLabel;

	Array<Colour> customColours;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEditor)
};
