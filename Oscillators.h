/*
  ==============================================================================

    Oscillators.h
    Created: 5 Jul 2019 8:02:51pm
    Author:  amayn

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class WavetableOscillator {
public:
	WavetableOscillator(AudioSampleBuffer& wavetableToUse) : wavetable(wavetableToUse), tableSize(wavetable.getNumSamples() - 1) {
		
	}

	void setFrequency(int frequency, int sampleRate) {
		tableDelta = tableSize * frequency / sampleRate;
	}

	float getNextSample() noexcept {
		//int tableSize = wavetable.getNumSamples();              -pacani, eto predatel'

		auto* table = wavetable.getReadPointer(0);
		float currentSample = table[(unsigned int)currentIndex];
		//Logger::outputDebugString(std::to_string(currentIndex));
		if ((currentIndex += tableDelta) > tableSize) currentIndex -= tableSize;

		return currentSample;
	}
	void resetIndex() {
		currentIndex = 0.0f;
	}
private:
	const AudioSampleBuffer& wavetable;
	float currentIndex = 0.0f;
	float tableDelta = 0.0f;
	const int tableSize;
};

