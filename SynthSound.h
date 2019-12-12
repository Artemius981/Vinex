/*
  ==============================================================================

    SynthSound.h
    Created: 3 Jul 2019 12:39:32am
    Author:  amayn

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class SynthSound : public SynthesiserSound {
public:
	bool appliesToNote(int midiNoteNumber) {
		return true;
	}
	bool appliesToChannel(int midiChannel) {
		return true;
	}
private:

};