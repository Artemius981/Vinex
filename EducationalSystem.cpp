/*
  ==============================================================================

    EducationalSystem.cpp
    Created: 18 Jan 2020 11:43:15pm
    Author:  amayn

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "EducationalSystem.h"

//==============================================================================
/*
   _____ _______ ______ _____    __
  / ____|__   __|  ____|  __ \  /_ |
 | (___    | |  | |__  | |__) |  | |
  \___ \   | |  |  __| |  ___/   | |
  ____) |  | |  | |____| |       | |
 |_____/   |_|  |______|_|       |_|
*/
//==============================================================================
Step1::Step1(OscEditor* oscGui, Label* textGui, Label* descGui): tasksBlock(textGui), oscBlock(oscGui), descriptionBlock(descGui), tasks(2), descriptions(3)
{
	//tasksBlock = &textGui;
	//descriptionBlock = &descGui;
	//oscBlock = &oscGui;
	addAndMakeVisible(oscBlock);

	Font font;
	font = lf.getAestheticsFont().withPointHeight(13.5);
	//setOpaque(true);

	tasks[0] = "1. Choose any type of waveform(sine, sawtooth, square).";
	tasks[1] = "2. Configure oscillator using controls(OCT, SEMI, LEVEL...).";
	//tasks[2] = "3. Play any note/s.";
	tasks.push_back("1. Choose any type of waveform(sine, sawtooth, square).");
	descriptions[0] = "1. On the left side you can see the first oscillator. On the right side you can see frequency spectrum visualiser. It shows frequencies of real time signal.";
	descriptions[1] = "2. test";
	descriptions[2] = "3. test";

	tasksBlock->setText("", dontSendNotification);
	tasksBlock->setFont(font);
	tasksBlock->setColour(Label::textColourId, Colours::whitesmoke);
	tasksBlock->setMinimumHorizontalScale(1.f);
	tasksBlock->setJustificationType(Justification::topLeft);
	addAndMakeVisible(tasksBlock);

	descriptionBlock->setFont(font);
	descriptionBlock->setText("", dontSendNotification);
	descriptionBlock->setColour(Label::textColourId, Colours::whitesmoke);
	descriptionBlock->setMinimumHorizontalScale(1.f);
	descriptionBlock->setJustificationType(Justification::topLeft);

	addAndMakeVisible(descriptionBlock);

	addAndMakeVisible(&nextButton);
	nextButton.setButtonText("NEXT");
	nextButton.addListener(this);
}

Step1::~Step1()
{
}
void Step1::buttonClicked(Button* button)
{
	if (textIndex > textMaxIndex || (textIndex += 1) > textMaxIndex) return;
	DBG(tasks[textIndex]);
	tasksBlock->setText(tasksBlock->getText() + tasks[textIndex] + "\n", dontSendNotification);
	descriptionBlock->setText(descriptionBlock->getText() + descriptions[textIndex] + "\n", dontSendNotification);
}
void Step1::paint(Graphics & g)
{
	g.setColour(Colour(213, 213, 213));
	g.drawRect(13, 359, 460, 194);
	g.drawRect(522, 359, 460, 194);
}

void Step1::resized()
{
	oscBlock->setVisible(true);
	//oscBlock->setBounds(13, 60, 290, 270);
	tasksBlock->setBounds(15, 360, 447, 183);
	descriptionBlock->setBounds(522, 360, 447, 183);
	nextButton.setBounds(13, 340, 43, 15);
}

//==============================================================================
EducationalSystem::EducationalSystem(OsctestAudioProcessor& p, OscEditor* osc1Gui): processor(p), osc1Gui(osc1Gui), step1(osc1Gui, &tasksBlock, &descriptionBlock)
{
	addAndMakeVisible(&step1);
}

EducationalSystem::~EducationalSystem()
{
}

void EducationalSystem::paint (Graphics& g)
{
	//g.fillAll(Colours::aquamarine);
}

void EducationalSystem::resized()
{
	step1.setBounds(getLocalBounds());
}
