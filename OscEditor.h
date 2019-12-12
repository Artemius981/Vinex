/*
  ==============================================================================

    OscEditor.h
    Created: 22 Aug 2019 12:41:10am
    Author:  amayn

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "CustomLnF.h"
//==============================================================================
/*
*/
class WaveVisualiser : public Component
{
public:
	WaveVisualiser(OsctestAudioProcessor& p, int oscId);
	~WaveVisualiser();

	void paint(Graphics&) override;
	void resized() override;

private:
	Path wave;
	int oscId;
	OsctestAudioProcessor& processor;

	Array<Colour> customColours;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveVisualiser)
};

class OscEditor    : public Component, ComboBox::Listener
{
public:
	OscEditor(OsctestAudioProcessor& p, int oscId);
    ~OscEditor();

    void paint (Graphics&) override;
    void resized() override;
	void comboBoxChanged(ComboBox* comboChanged);

	OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
	OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;
	OwnedArray<AudioProcessorValueTreeState::ComboBoxAttachment> comboBoxAttachments;
private:
	OsctestAudioProcessor& processor;

	Slider octKnob;
	Slider semiKnob;
	Slider panKnob;
	Slider gainKnob;
	Slider wtposKnob;
	Slider tableModKnob;
	Slider phaseKnob;
	ComboBox wavePicker;
	ToggleButton statusButton;

	Label oscLabel;
	lnf lf;
	
	std::string oscNum;
	int oscId;
	Array<Colour> customColours;

	WaveVisualiser tableWave;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscEditor)
};
