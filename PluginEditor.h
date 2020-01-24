/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Envelope.h"
#include "OscEditor.h"
#include "FilterEditor.h"
#include "EducationalSystem.h"

//==============================================================================
/**
*/

class OsctestAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    OsctestAudioProcessorEditor (OsctestAudioProcessor&);
    ~OsctestAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void addItemToContainer(int containerId, bool useComponent, Component* component);

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> masterGainAttachment;
	OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

private:
	void calculateObjectsCoordinates(Rectangle<int>& container, std::vector <std::pair<int, int>>& objectsSizes,
		int spaceFromBorders, std::vector<std::pair<int, int>>& outputVector);
	//void comboBoxChanged(ComboBox* box);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OsctestAudioProcessor& processor;

	//ComboBox oscTypeBox;
	Slider masterGainKnob;

	Envelope envGui;
	OscEditor osc1Gui;
	OscEditor osc2Gui;
	FilterEditor filterGui;
	EducationalSystem eduSystem;

	FlexBox middleContainer;
	FlexBox bottomContainer;

	int middleContainerWidth;
	int middleContainerHeight = 289;
	int middleContainerItemWidth = 290;
	int middleContainerItemHeight = 270;

	int bottomContainerWidth;
	int bottomContainerHeight = 234;
	int bottomContainerItemWidth = 462;
	int bottomContainerItemHeight = 215;

	Rectangle <int> middleContainerArea;
	Rectangle <int> bottomContainerArea;

	lnf lf;

	Label logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OsctestAudioProcessorEditor)
};
