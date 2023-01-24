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
    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

public:
    Knob(const String& name, const String& paramId, KnobSize knobSize, juce::AudioProcessorValueTreeState& apvts) : Component(name), knobSize(knobSize), apvts(apvts)
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

        attachment = std::make_unique<SliderAttachment>(apvts, paramId, slider);
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

    juce::AudioProcessorValueTreeState& apvts;
    std::unique_ptr<SliderAttachment> attachment;
};


#endif //VINEX_KNOB_H
