/*
  ==============================================================================

    Envelope.h
    Created: 28 Jul 2019 12:03:08am
    Author:  amayn

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "CustomLnF.h"

//==============================================================================
class EnvVisualiser : public Component, public Timer
{
public:
	EnvVisualiser(OsctestAudioProcessor& p);
	~EnvVisualiser();

	void paint(Graphics&) override;
	void resized() override;
	void timerCallback();

private:
	Path wave;
	Path lines;
	OsctestAudioProcessor& processor;

	float* attack;
	float* decay;
	float* sustain;
	float* release;
	float* adsrValue;
	bool* adsrStatus;

	int sustainX;
	float realtimeLineX = 20.f;
	float realtimeLineXInc = 1.0;
	bool endingRelease = false;
	float value = 0;
	float releaseInc;

	Array<Colour> customColours;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvVisualiser)
};
class Envelope    : public Component, Slider::Listener
{
public:
    Envelope(OsctestAudioProcessor&);
    ~Envelope();

    void paint (Graphics&) override;
	void sliderValueChanged(Slider* slider);
    void resized() override;

	OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

private:
	OsctestAudioProcessor& processor;

	Slider attackSlider;
	Slider decaySlider;
	Slider sustainSlider;
	Slider releaseSlider;

	lnf lf;
	Array<Colour> customColours;

	EnvVisualiser envVisual;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
