#pragma once

#ifndef VINEX_CONSTANTS_H
#define VINEX_CONSTANTS_H

namespace constants
{
    constexpr float pluginHeight = 365;
    constexpr float pluginWidth = 700;

    constexpr float headerHeight = 70;

    constexpr float blockHeaderHeight = 18;
    constexpr float blockCornersRadius = 7;
    constexpr float blockTitleSizePt = 14;
    constexpr float blockContentPadding = 3;

    constexpr int oscSectionVisualWidth = 175;

    constexpr float knobRegularWidth = 50;
    constexpr float knobRegularHeight = 45;
    constexpr float knobSmallWidth = 47;
    constexpr float knobSmallHeight = 39;
    constexpr float knobIndent = -2;
    constexpr float knobLabelHeight = 14;

    constexpr int numOfVoices = 8;
    constexpr int fftOrder = 11;
    constexpr int wavetableSize = 1 << fftOrder;
    constexpr int numOfSubtables = 10;
    constexpr double defaultSampleRate = 44100.0;
    constexpr int oversamplingRatio = 2;
}

#endif //VINEX_CONSTANTS_H
