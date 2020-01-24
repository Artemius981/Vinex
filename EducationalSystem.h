/*
  ==============================================================================

    EducationalSystem.h
    Created: 18 Jan 2020 11:43:15pm
    Author:  amayn

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Envelope.h"
#include "OscEditor.h"
#include "FilterEditor.h"
#include "CustomLnF.h"

class Step1 : public Component, Button::Listener
{
public:
	Step1(OscEditor* oscGui, Label* textGui, Label* descGui);
	~Step1();

	void paint(Graphics&) override;
	void resized() override;
	void buttonClicked(Button*);

private:
	lnf lf;

	SafePointer<OscEditor> oscBlock;
	SafePointer<Label> tasksBlock;
	SafePointer<Label> descriptionBlock;
	//OscEditor* oscBlock;
	//Label* tasksBlock;
	//Label* descriptionBlock;
	ToggleButton aliasingSwitch;
	TextButton nextButton;

	std::vector<std::string> tasks;
	std::vector<std::string> descriptions;

	int textIndex = -1;
	int textMaxIndex = 2;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Step1)
};
//==============================================================================
class EducationalSystem    : public Component
{
public:
    EducationalSystem(OsctestAudioProcessor& p, OscEditor* osc1Gui);
    ~EducationalSystem();

    void paint (Graphics&) override;
    void resized() override;

private:
	OscEditor* osc1Gui;
	Label tasksBlock;
	Label descriptionBlock;
	OsctestAudioProcessor& processor;
	Step1 step1;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EducationalSystem)
};
//==============================================================================
