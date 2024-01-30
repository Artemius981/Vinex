#pragma once

#ifndef VINEX_CONSTANTS_H
#define VINEX_CONSTANTS_H

namespace constants
{
    constexpr float pluginHeight = 431;
    constexpr float pluginWidth = 721;

    constexpr float headerHeight = 67;

    constexpr int presetBrowserWidth = 223;
    constexpr int presetBrowserHeight = 57;
    constexpr int presetBrowserButtonWidth = 52;
    constexpr int presetBrowserButtonHeight = 21;
    constexpr int presetBrowserSelectorWidth = 223;
    constexpr int presetBrowserSelectorHeight = 33;

    constexpr float blockHeaderHeight = 24;
    constexpr float blockCornersRadius = 7;
    constexpr float blockTitleSizePt = 16;
    constexpr float blockContentPadding = 5;

    constexpr int oscSectionWidth = 356;
    constexpr int oscSectionHeight = 149;
    constexpr int oscSectionVisualizerWidth = 197;
    constexpr int oscSectionVisualizerHeight = 116;
    constexpr int oscSectionCurveHeight = 61;

    constexpr int envSectionWidth = 230;
    constexpr int envSectionHeight = 206;

    constexpr int filterSectionWidth = 249;
    constexpr int filterSectionHeight = 206;

    constexpr int lfoSectionWidth = 230;
    constexpr int lfoSectionHeight = 206;

    constexpr float knobRegularWidth = 41;
    constexpr float knobRegularHeight = 45;
    constexpr float knobSmallWidth = 41;
    constexpr float knobSmallHeight = 39;
    constexpr float knobIndent = -2;
    constexpr float knobLabelHeight = 17;

    constexpr int numOfVoices = 8;
    constexpr int fftOrder = 11;
    constexpr int wavetableSize = 1 << fftOrder;
    constexpr int numOfSubtables = 10;
    constexpr double defaultSampleRate = 44100.0;
    constexpr int oversamplingRatio = 2;
}

#endif //VINEX_CONSTANTS_H
