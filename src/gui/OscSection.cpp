#include "OscSection.h"

OscSection::OscSection(const int id, VinexAudioProcessor& processor, juce::AudioProcessorValueTreeState& apvts) : Section("Oscillator"), processor(processor), apvts(apvts), prefix(std::string("osc") + std::to_string(id))
, octaveKnob("Octave", prefix + "Oct", KnobSize::regular, apvts)
, phaseKnob("Phase", prefix + "Phase", KnobSize::regular, apvts)
, panKnob("Pan", prefix + "Pan", KnobSize::regular, apvts)
, levelKnob("Level", prefix + "Lvl", KnobSize::regular, apvts)
{
    addAndMakeVisible(&octaveKnob);
    addAndMakeVisible(&phaseKnob);
    addAndMakeVisible(&levelKnob);
    addAndMakeVisible(&panKnob);

    waveSelector.addItemList({"Sine", "Sawtooth", "Square"}, 1);
    waveSelector.onChange = [this] {changeWaveform();};
    waveSelector.setSelectedId(1);
    addAndMakeVisible(&waveSelector);

    waveSelectorAttachment = std::make_unique<ComboBoxAttachment>(apvts, prefix + "Wave", waveSelector);
}

void OscSection::paint(Graphics &g)
{
    Section::paint(g);
}

void OscSection::resized()
{
    Section::resized();
    octaveKnob.setPos(0, 0);
    phaseKnob.setPos(0, 0);
    panKnob.setPos(0, 0);
    levelKnob.setPos(0, 0);

    waveSelector.setBounds(72, 2, 88, 14);

    auto bounds = getLocalBounds().toFloat();
    bounds.removeFromTop(constants::blockHeaderHeight);
    bounds.reduce(3, 3);

    // TODO: Visualiser setBounds(bounds.removeFromRight(constants::oscSectionVisualWidth))
    bounds.removeFromRight(constants::oscSectionVisualWidth + constants::blockContentPadding);

    FlexBox knobsFB;
    knobsFB.flexDirection = FlexBox::Direction::row;
    knobsFB.flexWrap = FlexBox::Wrap::wrap;
    knobsFB.justifyContent = FlexBox::JustifyContent::spaceAround;
    knobsFB.alignContent = FlexBox::AlignContent::spaceAround;

    knobsFB.items.add(FlexItem(octaveKnob).withMinWidth(octaveKnob.getWidth()).withMinHeight(octaveKnob.getHeight()));
    knobsFB.items.add(FlexItem(phaseKnob).withMinWidth(phaseKnob.getWidth()).withMinHeight(phaseKnob.getHeight()));
    knobsFB.items.add(FlexItem(panKnob).withMinWidth(panKnob.getWidth()).withMinHeight(panKnob.getHeight()));
    knobsFB.items.add(FlexItem(levelKnob).withMinWidth(levelKnob.getWidth()).withMinHeight(levelKnob.getHeight()));

    knobsFB.performLayout(bounds);
}

void OscSection::changeWaveform()
{
}

