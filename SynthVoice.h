/*
  ==============================================================================

    SourceVoice.h
    Created: 1 Jul 2019 7:42:40pm
    Author:  amayn

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "Oscillators.h"
#include "kissfft/kiss_fft.h"
#include "kissfft/kiss_fftr.h"

class SynthVoice : public SynthesiserVoice {
public:
	SynthVoice(Array<float>& wavetable1ToUse, Array<float>& wavetable2ToUse) :
		wavetable1(wavetable1ToUse), 
		tableSize1(wavetable1.size()),
		wavetable2(wavetable2ToUse),
		tableSize2(wavetable2.size())
	{
		this->setWavetable(wavetable1, 1);
		this->setWavetable(wavetable2, 2);

		adsr.setSampleRate(getSampleRate());
	}
	//==============================================================================
	void setADSRSampleRate(double ADSRsampleRate) {
		adsr.setSampleRate(ADSRsampleRate);
	}
	void setADSRParameters(float* attack, float* decay, float* sustain, float* release) {
		adsrParams.attack = *attack;
		adsrParams.decay = *decay;
		adsrParams.sustain = *sustain;
		adsrParams.release = *release;
	}
	//==============================================================================
	void setWavetable(Array<float> wavetableC, int wtId) {
		kiss_fftr_cfg cfg = kiss_fftr_alloc(fftSize, 0, 0, 0);
		kiss_fftr_cfg icfg = kiss_fftr_alloc(fftSize, 1, 0, 0);

		kiss_fft_scalar *cxInput = new kiss_fft_scalar[fftSize];
		kiss_fft_cpx *cxOutput = new kiss_fft_cpx[fftSize / 2 + 1];
		kiss_fft_cpx* IcxInput = new kiss_fft_cpx[fftSize / 2 + 1];
		kiss_fft_scalar* IcxOutput = new kiss_fft_scalar[fftSize];

		for (int i = 0; i < fftSize; i++) {
			cxInput[i] = wavetableC.getUnchecked(i);
		}
		kiss_fftr(cfg, cxInput, cxOutput);

		int maxHarmonic = fftSize >> 1;
		float maxFrequency = 2. / 3. / maxHarmonic;

		int counter = 0;
		while (maxHarmonic) {
			zeromem(IcxOutput, fftSize);
			memset(IcxInput, 0.0f, sizeof(IcxInput) * ((fftSize / 2) + 1));
			for (int i = 0; i <= maxHarmonic; i++) {
				IcxInput[i].r = cxOutput[i].r;
				IcxInput[i].i = cxOutput[i].i;
			}
			kiss_fftri(icfg, IcxInput, IcxOutput);

			if (wtId == 1) {
				float* temp = subtables1[counter].wavetable = new float[fftSize];
				subtables1[counter].maxFrequency = maxFrequency;
				for (int i = 0; i < fftSize; i++) {
					temp[i] = IcxOutput[i] / fftSize;
				}
			}
			else {
				float* temp = subtables2[counter].wavetable = new float[fftSize];
				subtables2[counter].maxFrequency = maxFrequency;
				for (int i = 0; i < fftSize; i++) {
					temp[i] = IcxOutput[i] / fftSize;
				}
			}
			maxHarmonic /= 2;
			maxFrequency *= 2;
			counter++;
		}
		free(cfg);
		free(icfg);
		delete[] cxInput;
		delete[] cxOutput;
		delete[] IcxInput;
		delete[] IcxOutput;
	}
	void setMainParams(float* masterG, float* wt1S, float* wt2S) {
		osc1Status = *wt1S;
		osc2Status = *wt2S;
		masterGain = *masterG;
	}
	void setOsc1Params(float* oct, float* semi, float* pan, float* gain, float* wtpos, float* phase) {
		osc1Oct = *oct;
		osc1Semi = *semi;
		osc1Pan = *pan;
		osc1Gain = *gain;
		osc1WTPos = *wtpos;
		osc1Phase = *phase;
	}
	void setOsc2Params(float* oct, float* semi, float* pan, float* gain, float* wtpos, float* phase) {
		osc2Oct = *oct;
		osc2Semi = *semi;
		osc2Pan = *pan;
		osc2Gain = *gain;
		osc2WTPos = *wtpos;
		osc2Phase = *phase;
	}
	bool canPlaySound(SynthesiserSound* sound) {
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
		adsr.setParameters(adsrParams);
		adsr.noteOn();

		old1Oct = osc1Oct;
		old2Oct = osc2Oct;

		frequency1 = fmax(0, MidiMessage::getMidiNoteInHertz(midiNoteNumber + (osc1Oct * 12) + osc1Semi));
		frequency2 = fmax(0, MidiMessage::getMidiNoteInHertz(midiNoteNumber + (osc2Oct * 12) + osc2Semi));

		//tableDelta1 = frequency1 * tableSize1 / sampleRate;
		//tableDelta2 = frequency2 * tableSize2 / sampleRate;
		tableDelta1 = frequency1 / sampleRate;
		tableDelta2 = frequency2 / sampleRate;

		subtableId1 = 0;
		subtableId2 = 0;

		while ((subtableId1 < subtablesSize) && (tableDelta1 >= subtables1[subtableId1].maxFrequency)) subtableId1++;
		while ((subtableId2 < subtablesSize) && (tableDelta2 >= subtables2[subtableId2].maxFrequency)) subtableId2++;

		tableSize1 = tableSize2 = fftSize;

		currentIndex1 = osc1Phase / tableSize1;
		currentIndex2 = osc2Phase / tableSize2;
		noteNum = midiNoteNumber;
	}
	void stopNote(float velocity, bool allowTailOff) {
		adsr.noteOff();
	}
	void pitchWheelMoved(int newPitchWheelValue) {
		
	}
	void controllerMoved(int conttollerNumber, int newControllerValue) {
		
	}
	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
		if (tableDelta1 != 0.0 && tableDelta2 != 0.0) {
			if (osc1Oct != old1Oct) {
				old1Oct = osc1Oct;
				frequency1 = MidiMessage::getMidiNoteInHertz(noteNum + (osc1Oct * 12) + osc1Semi);
				tableDelta1 = frequency1 / sampleRate;
				subtableId1 = 0;
				while ((subtableId1 < subtablesSize) && (tableDelta1 >= subtables1[subtableId1].maxFrequency)) subtableId1++;

			}
			if (osc2Oct != old2Oct) {
				old2Oct = osc2Oct;
				frequency2 = MidiMessage::getMidiNoteInHertz(noteNum + (osc2Oct * 12) + osc2Semi);
				tableDelta2 = frequency2 / sampleRate;
				subtableId2 = 0;
				while ((subtableId2 < subtablesSize) && (tableDelta2 >= subtables2[subtableId2].maxFrequency)) subtableId2++;

			}
			//////////////////////////////////
			if (osc1Semi != old1Semi) {
				old1Oct = osc1Oct;
				frequency1 = fmax(0, MidiMessage::getMidiNoteInHertz(noteNum + (osc1Oct * 12) + osc1Semi));
				tableDelta1 = frequency1 / sampleRate;
				subtableId1 = 0;
				while ((subtableId1 < subtablesSize) && (tableDelta1 >= subtables1[subtableId1].maxFrequency)) subtableId1++;
			}
			if (osc2Semi != old2Semi) {
				old2Semi = osc2Semi;
				frequency2 = fmax(0, MidiMessage::getMidiNoteInHertz(noteNum + (osc2Oct * 12) + osc2Semi));
				tableDelta2 = frequency2 / sampleRate;
				subtableId2 = 0;
				while ((subtableId2 < subtablesSize) && (tableDelta2 >= subtables2[subtableId2].maxFrequency)) subtableId2++;
			}

			auto* table1 = subtables1[subtableId1].wavetable;
			auto* table2 = subtables2[subtableId2].wavetable;

			for (int i = 0; i < numSamples; ++i) {

				unsigned int index0 = (unsigned int)(currentIndex1 * tableSize1);
				float frac = currentIndex1 * tableSize1 - (float)index0;
				float value0 = table1[index0];
				if (index0 + 1 >= tableSize1) index0 = index0 - tableSize1;
				float value1 = table1[index0 + 1];

				float currentSample = (value0 + frac * (value1 - value0)) * osc1Gain * osc1Status;
				//================================================
				index0 = (unsigned int)(currentIndex2 * tableSize2);
				frac = currentIndex2 * tableSize2 - (float)index0;
				value0 = table2[index0];
				if (index0 + 1 >= tableSize2) index0 = index0 - tableSize2;
				value1 = table2[index0 + 1];

				currentSample += (value0 + frac * (value1 - value0)) * osc2Gain * osc2Status;
				for (int j = 0; j < outputBuffer.getNumChannels(); ++j) 
					outputBuffer.addSample(j, startSample, currentSample * adsr.getNextSample() * masterGain);

				if ((currentIndex1 += tableDelta1) > 1) currentIndex1 -= 1;
				if ((currentIndex2 += tableDelta2) > 1) currentIndex2 -= 1;

				++startSample;

				if (!adsr.isActive()) {
					tableDelta1 = 0.0f;
					currentIndex1 = osc1Phase;
					tableDelta2 = 0.0f;
					currentIndex2 = osc2Phase;
					clearCurrentNote();
					break;
				}
			}
		}

	}
private:
	Array<float> wavetable1;
	Array<float> wavetable2;
	//AudioSampleBuffer wavetable;

	float currentIndex1 = 0.0f;
	float tableDelta1 = 0.0f;
	float currentIndex2 = 0.0f;
	float tableDelta2 = 0.0f;

	float sampleRate = getSampleRate();
	float masterGain = 0.75f;
	float masterdb = 0.0f;
	float osc1db = 0.0f;
	float osc2db = 0.0f;

	//====================================
	int osc1Oct;
	int osc1Semi;
	float osc1Pan;
	float osc1Gain;
	int osc1WTPos;
	int osc1Phase;
	double frequency1 = 440;
	int old1Oct;
	int old1Semi;

	int osc2Oct;
	int osc2Semi;
	float osc2Pan;
	float osc2Gain;
	int osc2WTPos;
	int osc2Phase;
	double frequency2 = 440;
	int old2Oct;
	int old2Semi;
	//====================================4
	int noteNum;

	int tableSize1;
	int tableSize2;
	int tableSizeEX = 1 << 11;

	bool osc1Status = true;
	bool osc2Status = true;

	ADSR adsr;
	ADSR::Parameters adsrParams;

	int static const fftSize = 1 << 11;
	struct subtables {
		float* wavetable;
		float maxFrequency;
	};

	static const int subtablesSize = 13;
	subtables subtables1[subtablesSize];
	subtables subtables2[subtablesSize];

	int subtableId1 = 0;
	int subtableId2 = 0;
};