#include "VinexLookAndFeel.h"
#include "VinexColours.h"
#include "BinaryData.h"

VinexLookAndFeel::VinexLookAndFeel()
{
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(getFontTypeface(FontStyle::regular));
}

Typeface::Ptr VinexLookAndFeel::getFontTypeface(FontStyle fontStyle)
{
    static const auto regularTypeface = Typeface::createSystemTypefaceFor(BinaryData::HankenGroteskRegular_ttf, BinaryData::HankenGroteskRegular_ttfSize);
    static const auto semiBoldTypeface = Typeface::createSystemTypefaceFor(BinaryData::HankenGroteskSemiBold_ttf, BinaryData::HankenGroteskSemiBold_ttfSize);

    switch (fontStyle)
    {
        case FontStyle::regular:
            return regularTypeface;
        case FontStyle::semiBold:
            return semiBoldTypeface;
        default:
            return regularTypeface;
    }
}

Font VinexLookAndFeel::getFont(FontStyle fontStyle)
{
    return {getFontTypeface(fontStyle)};
}

void
VinexLookAndFeel::drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPosProportional,
                                   float rotaryStartAngle, float rotaryEndAngle, Slider &slider)
{
//    auto outline = slider.findColour (Slider::rotarySliderOutlineColourId);
//    auto fill    = slider.findColour (Slider::rotarySliderFillColourId);

    auto bounds = Rectangle<int> (x, y, width, height).toFloat();

    auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto radiusKnob = radius - 3;
    auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = 2;
    auto arcRadius = radius - lineW * 0.5f;

    Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);

    g.setColour (vinex_colours::knobRangeBackground);
    g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::butt));

    if (slider.isEnabled())
    {
        Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);

        g.setColour (vinex_colours::knobRangeValue);
        g.strokePath (valueArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::butt));
    }

    g.setColour(vinex_colours::knob);
    g.fillEllipse(bounds.getCentreX() - radiusKnob, bounds.getCentreY() - radiusKnob, 2 * radiusKnob, 2 * radiusKnob);

    juce::Path p;
    auto pointerLength = radiusKnob * 0.7f;
    auto pointerThickness = 2.0f;
    p.addRectangle (-pointerThickness * 0.5f, -radiusKnob, pointerThickness, pointerLength);
    p.applyTransform (juce::AffineTransform::rotation (toAngle).translated (bounds.getCentreX(), bounds.getCentreY()));

    g.setColour(vinex_colours::knobPointer);
    g.fillPath(p);
}