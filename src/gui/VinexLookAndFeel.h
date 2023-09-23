#pragma once

#ifndef VINEX_VINEXLOOKANDFEEL_H
#define VINEX_VINEXLOOKANDFEEL_H

#include <JuceHeader.h>

class VinexLookAndFeel : public LookAndFeel_V4
{
public:
    enum class FontStyle { regular, semiBold };

    VinexLookAndFeel();

    static Typeface::Ptr getFontTypeface(FontStyle fontStyle);
    static Font getFont(FontStyle fontStyle);

    void drawRotarySlider(Graphics &graphics, int x, int y, int width, int height, float sliderPosProportional,
                          float rotaryStartAngle, float rotaryEndAngle, Slider &slider) override;
};


#endif //VINEX_VINEXLOOKANDFEEL_H
