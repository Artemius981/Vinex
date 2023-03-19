#ifndef VINEX_SYNTHSOUND_H
#define VINEX_SYNTHSOUND_H


#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override
    {
        return true;
    }

    bool appliesToChannel(int midiChannel) override
    {
        return true;
    }

};


#endif //VINEX_SYNTHSOUND_H
