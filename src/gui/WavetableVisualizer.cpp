#include "WavetableVisualizer.h"
#include "VinexColours.h"

WavetableVisualizer::WavetableVisualizer(service::WavetableManager& wavetableManager)
    : wavetableManager(wavetableManager)
{}

void WavetableVisualizer::paint(Graphics& g)
{
    auto bounds = getLocalBounds();

    g.setColour(vinex_colours::comboBoxBackground);
    g.fillRoundedRectangle(bounds.toFloat(), 4);

    Path wavetableCurve;
    auto wavetable = wavetableManager.getCurrentWavetableData();
    if (!wavetable.empty())
    {
        int pointsCount = bounds.getWidth();
        auto stepSize = static_cast<float>(wavetable.size()) / pointsCount;

        for (int i = 0; i < pointsCount; ++i)
            wavetableCurve.lineTo(i, wavetable[stepSize * i]);
        wavetableCurve.lineTo(bounds.getWidth(), 0.0f);
    }
    else
    {
        wavetableCurve.lineTo(bounds.getWidth(), 0.0f);
    }
    auto curveYScale = static_cast<float>(constants::oscSectionCurveHeight) / 2;
    wavetableCurve.applyTransform(AffineTransform::scale(1.0f, curveYScale));
    wavetableCurve.applyTransform(AffineTransform::translation(0, static_cast<float>(bounds.getHeight()) / 2));

    g.setColour(vinex_colours::wavetableCurve);
    g.strokePath(wavetableCurve, PathStrokeType(2.0f));
}
