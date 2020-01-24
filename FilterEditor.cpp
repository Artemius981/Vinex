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
		Colour(54, 54, 54),		//status button
		Colour(220, 220, 220)	//background color
	};

	enum radioGroups
	{
		routeButtons = 1001
	};

	addAndMakeVisible(&cutoffKnob);
	cutoffKnob.setLookAndFeel(&lf);
	cutoffKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	cutoffKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	cutoffKnob.setPopupDisplayEnabled(true, false, this);
	cutoffKnob.setRange(10, 22000, 1);
	cutoffKnob.setSkewFactorFromMidPoint(500);
	cutoffKnob.setValue(22000);

	addAndMakeVisible(&resKnob);
	resKnob.setLookAndFeel(&lf);
	resKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	resKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	resKnob.setPopupDisplayEnabled(true, false, this);
	resKnob.setRange(0.0, 3.0, 0.01);
	resKnob.setValue(0.5);

	addAndMakeVisible(&driveKnob);
	driveKnob.setLookAndFeel(&lf);
	driveKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	driveKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	driveKnob.setPopupDisplayEnabled(true, false, this);
	driveKnob.setRange(0.0, 1.0, 0.001);
	driveKnob.setValue(0.0);

	addAndMakeVisible(&gainKnob);
	gainKnob.setLookAndFeel(&lf);
	gainKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	gainKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	gainKnob.setPopupDisplayEnabled(true, false, this);
	gainKnob.setRange(0.0, 1.0, 0.001);
	gainKnob.setValue(1.0);

	typeMenu.addItemList({ "Lowpass", "Bandpass", "HighPass"}, 1);
	typeMenu.setColour(typeMenu.backgroundColourId, customColours[0]);
	typeMenu.setColour(typeMenu.textColourId, customColours[1]);
	typeMenu.setSelectedId(1);
	typeMenu.addListener(this);
	addAndMakeVisible(typeMenu);

	routeOsc1.setLookAndFeel(&lf);
	routeOsc2.setLookAndFeel(&lf);
	routeMaster.setLookAndFeel(&lf);
	routeOsc1.setRadioGroupId(routeButtons);
	routeOsc2.setRadioGroupId(routeButtons);
	routeMaster.setRadioGroupId(routeButtons);
	routeOsc1.triggerClick();
	routeOsc2.setToggleState(false, sendNotification);
	routeMaster.setToggleState(false, sendNotification);
	addAndMakeVisible(&routeOsc1);
	addAndMakeVisible(&routeOsc2);
	addAndMakeVisible(&routeMaster);

	routeOsc1Label.setFont(lf.getAestheticsFont().withPointHeight(12));
	routeOsc2Label.setFont(lf.getAestheticsFont().withPointHeight(12));
	routeMasterLabel.setFont(lf.getAestheticsFont().withPointHeight(12));
	routeOsc1Label.setText("O1", dontSendNotification);
	routeOsc2Label.setText("O2", dontSendNotification);
	routeMasterLabel.setText("M", dontSendNotification);
	routeOsc1Label.setColour(Label::textColourId, Colours::black);
	routeOsc2Label.setColour(Label::textColourId, Colours::black);
	routeMasterLabel.setColour(Label::textColourId, Colours::black);
	routeOsc1Label.setMinimumHorizontalScale(1.0f);
	routeOsc2Label.setMinimumHorizontalScale(1.0f);
	routeMasterLabel.setMinimumHorizontalScale(1.0f);
	addAndMakeVisible(&routeOsc1Label);
	addAndMakeVisible(&routeOsc2Label);
	addAndMakeVisible(&routeMasterLabel);
	//slider to attach to valuetree

	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "filterCutoff", cutoffKnob));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "filterRes", resKnob));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "filterDrive", driveKnob));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "filterGain", gainKnob));

	comboBoxAttachments.add(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.treeState, "filterType", typeMenu));

	buttonAttachments.add(new AudioProcessorValueTreeState::ButtonAttachment(processor.treeState, "filterRouteOsc1", routeOsc1));
	buttonAttachments.add(new AudioProcessorValueTreeState::ButtonAttachment(processor.treeState, "filterRouteOsc2", routeOsc2));
	buttonAttachments.add(new AudioProcessorValueTreeState::ButtonAttachment(processor.treeState, "filterRouteMaster", routeMaster));
}

FilterEditor::~FilterEditor()
{
}
void FilterEditor::paint(Graphics& g)
{
	g.fillAll(customColours[3]);

	g.setFont(lf.getAestheticsFont().withPointHeight(13.5));
	g.setColour(Colours::black);

	g.drawText("FILTER", 8, 4, 54, 19, Justification::left);

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
	typeMenu.setBounds((getWidth() - 140) / 2, 5, 140, 20);

	cutoffKnob.setBounds(20, 142, 34, 34);
	resKnob.setBounds(92, 142, 34, 34);
	driveKnob.setBounds(164, 142, 34, 34);
	gainKnob.setBounds(236, 142, 34, 34);

	routeOsc1.setBounds(10, 200, 14, 14);
	routeOsc2.setBounds(10, 218, 14, 14);
	routeMaster.setBounds(10, 236, 14, 14);

	routeOsc1Label.setBounds(22, 198, lf.getAestheticsFont().withPointHeight(12).getStringWidthFloat(routeOsc1Label.getText()) + 50, lf.getAestheticsFont().withPointHeight(14.85f).getHeight());
	routeOsc2Label.setBounds(22, 216, lf.getAestheticsFont().withPointHeight(12).getStringWidthFloat(routeOsc2Label.getText()) + 50, lf.getAestheticsFont().withPointHeight(14.85f).getHeight());
	routeMasterLabel.setBounds(22, 234, lf.getAestheticsFont().withPointHeight(12).getStringWidthFloat(routeMasterLabel.getText()) + 50, lf.getAestheticsFont().withPointHeight(14.85f).getHeight());
}
//======================================================================
void FilterEditor::comboBoxChanged(ComboBox* comboChanged) {
	processor.setFilterType(typeMenu.getSelectedId());
}