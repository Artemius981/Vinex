/*
  ==============================================================================

    OscEditor.cpp
    Created: 22 Aug 2019 12:41:10am
    Author:  amayn

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OscEditor.h"

//======================================================================
WaveVisualiser::WaveVisualiser(OsctestAudioProcessor& p, int oscId) : processor(p), oscId(oscId) {
	customColours = {
		Colour(129, 34, 106)	//wave
	};
}
void WaveVisualiser::paint(Graphics& g) {
	wave.clear();
	auto samples = processor.wavetables[processor.currentTables[oscId - 1]];
	int waveSize = samples.size() - 1;
	float tableDelta = waveSize / 290.0f;
	float tableIndex = 0;
	int centreY = getHeight() / 2;
	

	wave.startNewSubPath(0, centreY);
	for (int x = 0; x <= 290; x++) {
		wave.lineTo(x, centreY - (centreY - 15) * samples[(int)tableIndex]);
		if ((tableIndex += tableDelta) >= waveSize) tableIndex -= waveSize;
	}
	g.setColour(customColours[0]);
	g.strokePath(wave, PathStrokeType(2.0f));
}
void WaveVisualiser::resized() {

}
WaveVisualiser::~WaveVisualiser() {

}
//==============================================================================
void OscEditor::comboBoxChanged(ComboBox* comboChanged) {
	Logger::outputDebugString(std::to_string(wavePicker.getSelectedId() - 1));
	if (processor.currentTables[oscId - 1] != (wavePicker.getSelectedId() - 1)) {
		processor.setVoiceWavetable(wavePicker.getSelectedId() - 1, oscId);
		processor.currentTables[oscId - 1] = wavePicker.getSelectedId() - 1;
		tableWave.repaint();
	}
}
OscEditor::OscEditor(OsctestAudioProcessor& p, int oscId) : processor(p), oscNum(std::to_string(oscId)), tableWave(p, oscId), oscId(oscId)
{
	customColours = {
		Colour(183, 183, 183),	//wtback
		Colour(235, 235, 235),	//wt picker text
		Colour(54, 54, 54)		//status button
	};
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	addAndMakeVisible(octKnob);
	octKnob.setLookAndFeel(&lf);
	octKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	octKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	octKnob.setPopupDisplayEnabled(true, false, this);
	octKnob.setRange(-4, 4, 1);
	octKnob.setValue(0);

	addAndMakeVisible(semiKnob);
	semiKnob.setLookAndFeel(&lf);
	semiKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	semiKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	semiKnob.setPopupDisplayEnabled(true, false, this);
	semiKnob.setRange(-12, 12, 1);
	semiKnob.setValue(0);

	addAndMakeVisible(panKnob);
	panKnob.setLookAndFeel(&lf);
	panKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	panKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	panKnob.setPopupDisplayEnabled(true, false, this);
	panKnob.setRange(-1.0f, 1.0f, 0.01f);
	panKnob.setValue(0);

	addAndMakeVisible(gainKnob);
	gainKnob.setLookAndFeel(&lf);
	gainKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	gainKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	gainKnob.setPopupDisplayEnabled(true, false, this);
	gainKnob.setRange(0.0f, 1.0f, 0.01f);
	gainKnob.setValue(1.0f);

	addAndMakeVisible(wtposKnob);
	wtposKnob.setLookAndFeel(&lf);
	wtposKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	wtposKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	wtposKnob.setPopupDisplayEnabled(true, false, this);
	wtposKnob.setRange(0, 1024, 1);
	wtposKnob.setValue(0);

	addAndMakeVisible(tableModKnob);
	tableModKnob.setLookAndFeel(&lf);
	tableModKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	tableModKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	tableModKnob.setPopupDisplayEnabled(true, false, this);
	tableModKnob.setRange(-1.0f, 1.0f, 0.1f);
	tableModKnob.setValue(0);

	addAndMakeVisible(phaseKnob);
	phaseKnob.setLookAndFeel(&lf);
	phaseKnob.setSliderStyle(Slider::RotaryVerticalDrag);
	phaseKnob.setTextBoxStyle(Slider::NoTextBox, 1, 0, 0);
	phaseKnob.setPopupDisplayEnabled(true, false, this);
	phaseKnob.setRange(0, 1024, 1);
	phaseKnob.setValue(0);

	oscLabel.setFont(lf.getAestheticsFont().withPointHeight(13.0f));
	oscLabel.setColour(Label::textColourId, Colours::black);
	oscLabel.setText("OSC" + oscNum, dontSendNotification);
	addAndMakeVisible(oscLabel);

	wavePicker.addItemList({ "Sine", "Sawtooth", "Square", "Custom" }, 1);
	wavePicker.setColour(wavePicker.backgroundColourId, customColours[0]);
	wavePicker.setColour(wavePicker.textColourId, customColours[1]);
	wavePicker.setSelectedId(1);
	wavePicker.addListener(this);
	addAndMakeVisible(wavePicker);

	addAndMakeVisible(statusButton);
	statusButton.setColour(statusButton.textColourId, customColours[2]);

	addAndMakeVisible(&tableWave);

	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "osc" + oscNum + "Oct", octKnob));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "osc" + oscNum + "Semi", semiKnob));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "osc" + oscNum + "Pan", panKnob));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "osc" + oscNum + "Gain", gainKnob));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "osc" + oscNum + "WTPos", wtposKnob));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "osc" + oscNum + "TableMod", tableModKnob));
	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, "osc" + oscNum + "Phase", phaseKnob));

	buttonAttachments.add(new AudioProcessorValueTreeState::ButtonAttachment(processor.treeState, "osc" + oscNum + "Status", statusButton));

	comboBoxAttachments.add(new AudioProcessorValueTreeState::ComboBoxAttachment(processor.treeState, "osc" + oscNum + "Wavetable", wavePicker));
}

OscEditor::~OscEditor()
{
	attachments.clear();
}
void OscEditor::paint(Graphics& g)
{
	g.setFont(lf.getAestheticsFont().withPointHeight(13.5));

	g.drawText("OCT", 20, 173, 31, 19, Justification::left);
	g.drawText("SEMI", 90, 173, 34, 19, Justification::left);
	g.drawText("PAN", 165, 173, 26, 19, Justification::left);
	g.drawText("LEVEL", 230, 173, 43, 19, Justification::left);

	g.drawText("WT POS", 42, 238, 53, 19, Justification::left);
	g.drawText("none", 128, 238, 33, 19, Justification::left);
	g.drawText("PHASE", 194, 238, 47, 19, Justification::left);

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

void OscEditor::resized()
{
	statusButton.setBounds(268, 7, 14, 14);

	octKnob.setBounds(20, 142, 34, 34);
	semiKnob.setBounds(92, 142, 34, 34);
	panKnob.setBounds(164, 142, 34, 34);
	gainKnob.setBounds(236, 142, 34, 34);

	wtposKnob.setBounds(54, 204, 34, 34);
	tableModKnob.setBounds(128, 204, 34, 34);
	phaseKnob.setBounds(202, 204, 34, 34);

	oscLabel.setBounds(3, 4, 42, 20);

	wavePicker.setBounds((getWidth() - 140) / 2, 5, 140, 20);
	tableWave.setBounds(0, 30, 290, 99);
    // This method is where you should set the bounds of any child
    // components that your component contains..
}
//======================================================================