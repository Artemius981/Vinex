#include "VinexLookAndFeel.h"
#include "VinexColours.h"

VinexLookAndFeel::VinexLookAndFeel()
{
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Arial");
}

Font VinexLookAndFeel::getFont()
{
    return Font("Arial", 16, Font::plain);
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

void
VinexLookAndFeel::drawComboBox(Graphics &g, int width, int height, bool isButtonDown, int buttonX, int buttonY,
                               int buttonW, int buttonH, ComboBox &box)
{
    auto cornerSize = 7;
    Rectangle<int> boxBounds (0, 0, width, height);

    g.setColour (vinex_colours::comboBoxBackground);
    g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

    Rectangle<int> arrowZone (width - 30, 0, 20, height);
    Path path;
    path.startNewSubPath ((float) arrowZone.getX() + 3.0f, (float) arrowZone.getCentreY() - 2.0f);
    path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getCentreY() + 3.0f);
    path.lineTo ((float) arrowZone.getRight() - 3.0f, (float) arrowZone.getCentreY() - 2.0f);

    g.setColour (box.findColour (ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath (path, PathStrokeType (2.0f));
}
