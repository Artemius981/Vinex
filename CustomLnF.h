/*
  ==============================================================================

    CustomLnF.h
    Created: 22 Aug 2019 11:10:41pm
    Author:  amayn

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class lnf : public LookAndFeel_V4 {
public:
	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) {

		const float radius = jmin(width / 2, height / 2);
		const float centreX = x + width * 0.5f;
		const float centreY = y + height * 0.5f;
		const float rx = centreX - radius;
		const float ry = centreY - radius;
		const float rw = radius * 2.0f;
		const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		g.setColour(Colours::dimgrey);
		g.fillEllipse(rx, ry, rw, rw);

		Path pointer;
		const float pointerLength = radius * 0.5f;
		const float pointerThickness = 3.0f;
		pointer.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
		pointer.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		//pointer.applyTransform(AffineTransform::translation(centreX, centreY));

		g.setColour(Colours::black);
		g.fillPath(pointer);
	}
	//void drawComboBox(Graphics&, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, ComboBox&) {
	//	
	//}

	Font getAestheticsFont() {
		Font Helve(Font(Typeface::createSystemTypefaceFor(BinaryData::_9207_otf, BinaryData::_9207_otfSize)));
		return Helve;
	}
};