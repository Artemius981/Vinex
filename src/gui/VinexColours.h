#pragma once

#include <cstdint>
#ifndef VINEX_VINEXCOLOURS_H
#define VINEX_VINEXCOLOURS_H

namespace vinex_colours
{
    const Colour background {0xff222222};
    const Colour blockBackground (0xff3F3F3F);
    const Colour blockHeader (0xff2E2E2E);

    const Colour textFirst (0xffFFFFFF);
    const Colour textSecond (0xffF5F5F5);
    const Colour textThird (0xffB8B8B8);

    const Colour knobText (0xffFFFFFF);
    const Colour knobRangeBackground (0xff242424);
    const Colour knobRangeValue (0xff35B56B);
    const Colour knobPointer (0xff242424);
    const Colour knob (0xff888888);

    const Colour toggledState (0xff35B56B);
    const Colour untoggledState (0xffD9D9D9);

    const Colour wavetableCurve (0xff35B56B);

    const Colour comboBoxBackground (0xff101010);
}

enum class OldVinexColours : std::uint32_t {
    background = 0x222222,

    blockBackground = 0x3F3F3F,
    blockHeader = 0x2E2E2E,

    textFirst = 0xFFFFFF,
    textSecond = 0xF5F5F5,
    textThird = 0xB8B8B8,

    knobText = 0xFFFFFF,
    knobRange = 0x35B56B,

    toggledState = 0x35B56B,
    untoggledState = 0xD9D9D9,

    wavetableCurve = 0x35B56B
};


#endif //VINEX_VINEXCOLOURS_H
