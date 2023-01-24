#pragma once

#ifndef VINEX_KNOB_H
#define VINEX_KNOB_H

#include <JuceHeader.h>
#include "../Constants.h"

enum class KnobSize {
    small = 27,
    regular = 33
};

class Knob : public Component
{
public:
    Knob(const String& name, const String& paramId, KnobSize knobSize) : Component(name), knobSize(knobSize)
    {
        slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        slider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        slider.setPopupDisplayEnabled(true, false, nullptr);
        addAndMakeVisible(&slider);

        label.setFont(constants::knobLabelHeight);
        label.setText(name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setMinimumHorizontalScale(1.0f);
        addAndMakeVisible(&label);
    }

    void paint(Graphics& g) override;
    void resized() override;
    void setPosWithKnobSize(int x, int y, KnobSize size);
    void setPos(int x, int y);

    void setRange(double newMinimum, double newMaximum, double newInterval = 0);
    void setValue(double newValue, NotificationType notification=sendNotificationAsync);

    void setKnobSize(KnobSize knobSize);

private:
    Slider slider;
    Label label;
    KnobSize knobSize;
};


#endif //VINEX_KNOB_H
