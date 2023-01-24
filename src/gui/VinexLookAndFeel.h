#pragma once

#ifndef VINEX_VINEXLOOKANDFEEL_H
#define VINEX_VINEXLOOKANDFEEL_H

#include <JuceHeader.h>

class VinexLookAndFeel : public LookAndFeel_V4
{
public:
    VinexLookAndFeel();

    Font getFont();

    void drawRotarySlider(Graphics &graphics, int x, int y, int width, int height, float sliderPosProportional,
                          float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override;

    void
    drawComboBox(Graphics &graphics, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW,
                 int buttonH, ComboBox &box) override;
};


#endif //VINEX_VINEXLOOKANDFEEL_H
