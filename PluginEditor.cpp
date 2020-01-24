/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Array<Colour> customColours{
	Colour(220, 220, 220),
	Colour(72, 61, 119)
};
//==============================================================================

OsctestAudioProcessorEditor::OsctestAudioProcessorEditor (OsctestAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), envGui(p), osc1Gui(p, 1), osc2Gui(p, 2), filterGui(p), eduSystem(p, &osc1Gui)
{
	setSize (998, 575);

	//addAndMakeVisible(&oscTypeBox);
	//oscTypeBox.addItem("Sine", 1);
	//oscTypeBox.addItem("Saw", 2);
	//oscTypeBox.addItem("Square", 3);
	//oscTypeBox.setSelectedId(1);
	//oscTypeBox.addListener(this);

	addAndMakeVisible(&masterGainKnob);
	masterGainKnob.setLookAndFeel(&lf);
	masterGainKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	masterGainKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	masterGainKnob.setRange(0.0f, 1.0f, 0.01f);
	masterGainKnob.setValue(1.0f);
	masterGainKnob.setPopupDisplayEnabled(true, false, this);

	addAndMakeVisible(&envGui);
	addAndMakeVisible(&osc1Gui);
	addAndMakeVisible(&osc2Gui);
	addAndMakeVisible(&filterGui);
	//addAndMakeVisible(&eduSystem);

	logo.setFont(lf.getAestheticsFont().withPointHeight(54.5));
	logo.setText("Vinex", dontSendNotification);
	logo.setColour(Label::textColourId, Colours::whitesmoke);
	addAndMakeVisible(logo);

	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "masterGain", masterGainKnob));
}

OsctestAudioProcessorEditor::~OsctestAudioProcessorEditor()
{
}
//==============================================================================
void OsctestAudioProcessorEditor::paint (Graphics& g)
{
	g.setFont(lf.getAestheticsFont().withPointHeight(13.5f));
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (customColours[1]);

    g.setColour (customColours[0]);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);

	//g.fillRect(13, 350, 462, 215);							//ENV
	g.fillRect(524, 350, 462, 215);							//LFO

	g.setColour(Colour(212, 212, 212));
	g.drawText("MASTER", 935, 39, 65, 19, Justification::left);
}

void OsctestAudioProcessorEditor::resized()
{
	masterGainKnob.setBounds(949, 6, 34, 34);
	//envGui.setBounds(13, 350, 462, 215);
	logo.setBounds(0, -5, 138, 73);

	middleContainerWidth = getWidth();
	middleContainerHeight = 289;
	middleContainerItemWidth = 290;
	middleContainerItemHeight = 270;

	middleContainer = FlexBox(
		FlexBox::Direction::row,
		FlexBox::Wrap::noWrap,
		FlexBox::AlignContent::flexStart,
		FlexBox::AlignItems::flexStart,
		FlexBox::JustifyContent::spaceAround
	);
	middleContainerArea = Rectangle <int> (0, 60, middleContainerWidth, middleContainerHeight);
	//middleContainer.items.add(FlexItem(middleContainerItemWidth, middleContainerItemHeight, osc1Gui));
	//middleContainer.items.add(FlexItem(middleContainerItemWidth, middleContainerItemHeight, filterGui));
	//middleContainer.items.add(FlexItem(middleContainerItemWidth, middleContainerItemHeight, osc2Gui));
	addItemToContainer(1, true, &osc1Gui);
	addItemToContainer(1, true, &filterGui);
	addItemToContainer(1, true, &osc2Gui);
	middleContainer.performLayout(middleContainerArea);

	bottomContainerWidth = getWidth();
	bottomContainerHeight = 234;
	bottomContainerItemWidth = 462;
	bottomContainerItemHeight = 215;

	bottomContainer = FlexBox(
		FlexBox::Direction::row,
		FlexBox::Wrap::noWrap,
		FlexBox::AlignContent::flexStart,
		FlexBox::AlignItems::flexStart,
		FlexBox::JustifyContent::spaceAround
	);
	bottomContainerArea = Rectangle <int>(0, 349, bottomContainerWidth, bottomContainerHeight);
	//bottomContainer.items.add(FlexItem(bottomContainerItemWidth, bottomContainerItemHeight, envGui));
	addItemToContainer(2, true, &envGui);
	bottomContainer.items.add(FlexItem(bottomContainerItemWidth, bottomContainerItemHeight));
	bottomContainer.performLayout(bottomContainerArea);
	osc1Gui.setVisible(false);
	osc2Gui.setVisible(false);
	filterGui.setVisible(false);
	envGui.setVisible(false);

	eduSystem.setBounds(getBounds());
}
//=============================================
void OsctestAudioProcessorEditor::addItemToContainer(int containerId, bool useComponent = false, Component* component = nullptr)
{
	if (containerId == 1) 
	{
		if(useComponent)
		{
			middleContainer.items.add(FlexItem(middleContainerItemWidth, middleContainerItemHeight, *component));
		}
		else
		{
			middleContainer.items.add(FlexItem(middleContainerItemWidth, middleContainerItemHeight));
		}
	}
	else if (containerId == 2) 
	{
		if (useComponent)
		{
			bottomContainer.items.add(FlexItem(bottomContainerItemWidth, bottomContainerItemHeight, *component));
		}
		else
		{
			bottomContainer.items.add(FlexItem(bottomContainerItemWidth, bottomContainerItemHeight));
		}
	}
}
