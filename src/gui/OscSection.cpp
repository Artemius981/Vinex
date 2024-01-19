#include "OscSection.h"
#include "VinexColours.h"

OscSection::OscSection(const int id, VinexAudioProcessor& processor, juce::AudioProcessorValueTreeState& apvts) : Section("Oscillator"), processor(processor), apvts(apvts), prefix(std::string("osc") + std::to_string(id))
{
    basicKnobs.add(new Knob("Phase", prefix + "Phase", KnobSize::regular, apvts));
    basicKnobs.add(new Knob("Pan", prefix + "Pan", KnobSize::regular, apvts));
    basicKnobs.add(new Knob("Level", prefix + "Lvl", KnobSize::regular, apvts));

    unisonKnobs.add(new Knob("Detune", prefix + "Detune", KnobSize::regular, apvts));
    unisonKnobs.add(new Knob("Blend", prefix + "Blend", KnobSize::regular, apvts));

    for (auto knob : basicKnobs)
        addAndMakeVisible(knob);

    for (auto knob : unisonKnobs)
        addAndMakeVisible(knob);

    waveSelector.addItemList({"Sine", "Sawtooth", "Square"}, 1);
    waveSelector.onChange = [this] {changeWaveform();};
    waveSelector.setSelectedId(1);
    waveSelector.setColour(ComboBox::ColourIds::backgroundColourId, vinex_colours::comboBoxBackground);
    waveSelector.setColour(ComboBox::ColourIds::outlineColourId, vinex_colours::comboBoxBackground);
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

    waveSelector.setBounds(90, 2, 88, 20);

    auto bounds = getLocalBounds().toFloat();
    bounds.removeFromTop(constants::blockHeaderHeight);
    bounds.reduce(constants::blockContentPadding, constants::blockContentPadding);

    // TODO: Visualiser setBounds(bounds.removeFromRight(constants::oscSectionVisualWidth))
    bounds.removeFromRight(constants::oscSectionVisualizerWidth + constants::blockContentPadding);
    performKnobLayout(bounds);
}

void OscSection::performKnobLayout(Rectangle<float> bounds) const
{
    auto knobLineWidth = bounds.getWidth();
    auto knobLineHeight = constants::knobRegularHeight;

    FlexBox basicKnobsFlex;
    basicKnobsFlex.flexDirection = FlexBox::Direction::row;
    basicKnobsFlex.justifyContent = FlexBox::JustifyContent::spaceBetween;
    basicKnobsFlex.alignItems = FlexBox::AlignItems::flexStart;

    for (auto knob : basicKnobs)
    {
        float knobWidth = 0.0f, knobHeight = 0.0f;
        switch(knob->getKnobSize())
        {
            case KnobSize::regular:
                knobWidth = constants::knobRegularWidth;
                knobHeight = constants::knobRegularHeight;
                break;
            case KnobSize::small:
                knobWidth = constants::knobSmallWidth;
                knobHeight = constants::knobSmallHeight;
                break;
        }

        basicKnobsFlex.items.add(FlexItem(*knob)
                                            .withMinWidth(knobWidth)
                                            .withMinHeight(knobHeight));
    }

    FlexBox unisonKnobsFlex;
    unisonKnobsFlex.flexDirection = FlexBox::Direction::row;
    unisonKnobsFlex.justifyContent = FlexBox::JustifyContent::spaceBetween;
    unisonKnobsFlex.alignItems = FlexBox::AlignItems::flexEnd;

    for (auto knob : unisonKnobs)
    {
        float knobWidth = 0.0f, knobHeight = 0.0f;
        switch(knob->getKnobSize())
        {
            case KnobSize::regular:
                knobWidth = constants::knobRegularWidth;
                knobHeight = constants::knobRegularHeight;
                break;
            case KnobSize::small:
                knobWidth = constants::knobSmallWidth;
                knobHeight = constants::knobSmallHeight;
                break;
        }

        unisonKnobsFlex.items.add(FlexItem(*knob)
                                          .withMinWidth(knobWidth)
                                          .withMinHeight(knobHeight));
    }

    FlexBox flex;
    flex.flexDirection = FlexBox::Direction::column;
    flex.alignContent = FlexBox::AlignContent::spaceBetween;
    flex.alignItems = FlexBox::AlignItems::flexEnd;

    flex.items.add(FlexItem(basicKnobsFlex)
                           .withMinWidth(knobLineWidth)
                           .withMinHeight(knobLineHeight));

    flex.items.add(FlexItem(unisonKnobsFlex)
                           .withMinWidth(knobLineWidth)
                           .withMinHeight(knobLineHeight));

    flex.performLayout(bounds);
}

void OscSection::changeWaveform()
{
    // temporary solution
    processor.setWavetable(waveSelector.getSelectedId() - 1);
}