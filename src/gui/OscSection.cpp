#include "OscSection.h"
#include "VinexColours.h"

OscSection::OscSection(const int id, service::WavetableManager& wavetableManager, juce::AudioProcessorValueTreeState& apvts)
    : Section("Oscillator"), wavetableManager(wavetableManager), apvts(apvts),
      prefix(std::string("osc") + std::to_string(id)), visualizer(wavetableManager)
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

    loadWavetables();
    waveSelector.onChange = [&] {
        wavetableManager.loadWavetable(waveSelector.getItemText(waveSelector.getSelectedItemIndex()));
        visualizer.repaint();
    };
    waveSelector.setColour(ComboBox::ColourIds::backgroundColourId, vinex_colours::comboBoxBackground);
    waveSelector.setColour(ComboBox::ColourIds::outlineColourId, vinex_colours::comboBoxBackground);
    addAndMakeVisible(&waveSelector);
    addAndMakeVisible(&visualizer);

    apvts.state.addListener(this);
}

OscSection::~OscSection()
{
    apvts.state.removeListener(this);
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

    visualizer.setBounds(bounds.removeFromRight(constants::oscSectionVisualizerWidth).toNearestInt());
    bounds.removeFromRight(constants::blockContentPadding);
    performKnobLayout(bounds);
}

void OscSection::valueTreeRedirected(ValueTree& treeWhichHasBeenChanged)
{
    const auto wavetablesTree = apvts.state.getChildWithName("wavetables");
    const auto newWavetableName = wavetablesTree.getProperty(prefix, String()).toString();
    const auto wavetables = wavetableManager.getWavetables(false);
    waveSelector.setSelectedItemIndex(wavetables->indexOf(newWavetableName), dontSendNotification);
    visualizer.repaint();
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

void OscSection::loadWavetables()
{
    waveSelector.clear(dontSendNotification);
    const auto wavetables = wavetableManager.getWavetables(true);
    const auto currentWavetable = wavetableManager.getCurrentWavetableName();
    waveSelector.addItemList(*wavetables, 1);
    waveSelector.setSelectedItemIndex(wavetables->indexOf(currentWavetable), dontSendNotification);
}