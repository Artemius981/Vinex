#include "Knob.h"

void Knob::paint(Graphics &g)
{
    //g.fillAll(juce::Colours::black);
}

void Knob::resized()
{
    auto bounds = getLocalBounds();

    label.setBounds(bounds.removeFromBottom(constants::knobLabelHeight));
    bounds.setHeight(bounds.getHeight() - constants::knobIndent);
    slider.setBounds(bounds);
}

void Knob::setKnobSize(KnobSize size)
{
    Knob::knobSize = size;
}

void Knob::setPosWithKnobSize(int x, int y, KnobSize size)
{
    switch (size)
    {
        case KnobSize::small:
            setBounds(x, y, constants::knobSmallWidth, constants::knobSmallHeight);
            break;
        case KnobSize::regular:
            setBounds(x, y, constants::knobRegularWidth, constants::knobRegularHeight);
            break;
    }
}

void Knob::setPos(int x, int y)
{
    setPosWithKnobSize(x, y, knobSize);
}

void Knob::setRange(double newMinimum, double newMaximum, double newInterval)
{
    slider.setRange(newMinimum, newMaximum, newInterval);
}

void Knob::setValue(double newValue, NotificationType notification)
{
    slider.setValue(newValue, notification);
}
