/*
  ==============================================================================

    FilterEditor.cpp
    Created: 12 Nov 2019 11:32:01pm
    Author:  amayn

  ==============================================================================
*/

#include "FilterEditor.h"

FilterEditor::FilterEditor(OsctestAudioProcessor& p) : processor(p)
{
	customColours = {
		Colour(183, 183, 183),	//wtback
		Colour(235, 235, 235),	//wt picker text
		Colour(54, 54, 54)		//status button
	};

	addAndMakeVisible(&cutoffKnob);
	cutoffKnob.setLookAndFeel(&lf);
	cutoffKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	cutoffKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	cutoffKnob.setPopupDisplayEnabled(true, false, this);
	cutoffKnob.setRange(8, 22050, 1);
	cutoffKnob.setValue(22050);

	addAndMakeVisible(&resKnob);
	resKnob.setLookAndFeel(&lf);
	resKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	resKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	resKnob.setPopupDisplayEnabled(true, false, this);
	resKnob.setRange(0, 100, 1);
	resKnob.setValue(10);

	addAndMakeVisible(&driveKnob);
	driveKnob.setLookAndFeel(&lf);
	driveKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	driveKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	driveKnob.setPopupDisplayEnabled(true, false, this);
	driveKnob.setRange(0, 100, 1);
	driveKnob.setValue(0);

	addAndMakeVisible(&gainKnob);
	gainKnob.setLookAndFeel(&lf);
	gainKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	gainKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	gainKnob.setPopupDisplayEnabled(true, false, this);
	gainKnob.setRange(0, 100, 1);
	gainKnob.setValue(100);
}

FilterEditor::~FilterEditor()
{
}
void FilterEditor::paint(Graphics& g)
{
	g.setFont(lf.getAestheticsFont().withPointHeight(13.5));

	g.drawText("FILTER", 118, 4, 54, 19, Justification::left);

	g.drawText("CUTOFF", 10, 173, 54, 19, Justification::left);
	g.drawText("RES", 93, 173, 28, 19, Justification::left);
	g.drawText("DRIVE", 158, 173, 41, 19, Justification::left);
	g.drawText("LEVEL", 231, 173, 43, 19, Justification::left);


	g.setColour(Colours::black);
	g.drawLine(0, 29, 303, 29);

	g.setColour(customColours[0]);
	g.fillRect(0, 30, 290, 99);

	//g.drawText("OSC" + oscNum, 6, 9, 32, 17, Justification::left);

	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/
}

void FilterEditor::resized()
{
	cutoffKnob.setBounds(20, 142, 34, 34);
	resKnob.setBounds(92, 142, 34, 34);
	driveKnob.setBounds(164, 142, 34, 34);
	gainKnob.setBounds(236, 142, 34, 34);
}
//======================================================================
