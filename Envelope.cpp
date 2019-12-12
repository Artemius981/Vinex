/*
  ==============================================================================

    Envelope.cpp
    Created: 28 Jul 2019 12:03:08am
    Author:  amayn

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"
#include "CustomLnF.h"

//==============================================================================
EnvVisualiser::EnvVisualiser(OsctestAudioProcessor& p) : processor(p) {
	customColours = {
		Colour(129, 34, 106),	//wave
		Colour(110, 110, 110)	//ADSR Lines
	};
}
void EnvVisualiser::paint(Graphics& g) {
	wave.clear();
	lines.clear();
	float* attack = processor.treeState.getRawParameterValue("env1Attack");
	float* decay = processor.treeState.getRawParameterValue("env1Decay");
	float* sustain = processor.treeState.getRawParameterValue("env1Sustain");
	float* release = processor.treeState.getRawParameterValue("env1Release");

	int height = getHeight();
	int startY = getHeight() - 20;
	int startX = 20;
	int coef = 20;

	wave.startNewSubPath(startX, startY);
	wave.lineTo(startX + *attack * coef, 20);
	lines.addLineSegment(Line<float>(startX + *attack * coef, 20, startX + *attack * coef, height - 20), 1.0f);
	startY = 20;
	startX += *attack * coef;
	wave.lineTo(startX + *decay * coef, 20 + 114* (1 - *sustain));
	lines.addLineSegment(Line<float>(startX + *decay * coef, 20 + 114 * (1 - *sustain), startX + *decay * coef, height - 20), 1.0f);
	startX += *decay * coef;
	startY += 114 * (1 - *sustain);
	wave.lineTo(startX + *release * coef, getHeight() - 20);

	g.setColour(customColours[1]);
	g.drawLine(0, height - 20, getWidth(), height - 20);
	g.strokePath(lines, PathStrokeType(1.0f));
	g.setColour(customColours[0]);
	g.strokePath(wave, PathStrokeType(2.0f, PathStrokeType::JointStyle::mitered, PathStrokeType::EndCapStyle::square));
}
void EnvVisualiser::resized() {

}
EnvVisualiser::~EnvVisualiser() {

}

Envelope::Envelope(OsctestAudioProcessor& p) : processor(p), envVisual(p)
{
	customColours = {
		Colour(187, 187, 187)
	};
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

	attackSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	attackSlider.setRange(0.01f, 5.0f, 0.01f);
	attackSlider.setValue(0.01f);
	attackSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&attackSlider);

	decaySlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	decaySlider.setRange(0.1f, 2.0f, 0.01f);
	decaySlider.setValue(0.1f);
	decaySlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&decaySlider);

	sustainSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	sustainSlider.setRange(0.0f, 1.0f, 0.01f);
	sustainSlider.setValue(1.0f);
	sustainSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&sustainSlider);

	releaseSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	releaseSlider.setRange(0.01f, 5.0f, 0.01f);
	releaseSlider.setValue(0.01f);
	releaseSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
	addAndMakeVisible(&releaseSlider);

	attackSlider.addListener(this);
	decaySlider.addListener(this);
	sustainSlider.addListener(this);
	releaseSlider.addListener(this);

	addAndMakeVisible(&envVisual);

	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "env1Attack", attackSlider));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "env1Decay", decaySlider));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "env1Sustain", sustainSlider));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "env1Release", releaseSlider));
}

Envelope::~Envelope()
{
}
void Envelope::sliderValueChanged(Slider* sliderChanged) {
	envVisual.repaint();
}

void Envelope::paint (Graphics& g)
{
	g.setColour(customColours[0]);
	g.fillRect(202, 33, 246, 154);

	g.setColour(Colours::black);
	g.setFont(lf.getAestheticsFont().withPointHeight(16.5));
	g.drawText("ENVELOPE", 284, 9, 90, 20, Justification::centred);

    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
	g.drawText("A", 18, 190, 11, 22, Justification::centred);
	g.drawText("D", 52, 190, 12, 22, Justification::centred);
	g.drawText("S", 89, 190, 11, 22, Justification::centred);
	g.drawText("R", 122, 190, 12, 22, Justification::centred);
}

void Envelope::resized()
{
	attackSlider.setBounds(14, 20, 22, 167);	//.., 31, .., 156
	decaySlider.setBounds(49, 20, 22, 167);
	sustainSlider.setBounds(84, 20, 22, 167);
	releaseSlider.setBounds(119, 20, 22, 167);

	envVisual.setBounds(202, 33, 246, 154);
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
