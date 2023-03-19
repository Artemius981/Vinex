#ifndef VINEX_SYNTHVOICE_H
#define VINEX_SYNTHVOICE_H

#include <JuceHeader.h>
#include "WavetableOscillator.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice();

    bool canPlaySound(SynthesiserSound *sound) override;

    void startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition) override;

    void stopNote(float velocity, bool allowTailOff) override;

    void pitchWheelMoved(int newPitchWheelValue) override;

    void controllerMoved(int controllerNumber, int newControllerValue) override;

    void renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;

    void setWavetable(std::vector<float>& wavetable);
    void setOscParams(int id, std::atomic<float> *oct, std::atomic<float> *phase, std::atomic<float> *pan, std::atomic<float> *level);

    void setSampleRate(double sampleRate);

private:
    double sampleRate;
    WavetableOscillator wtOsc;
};


#endif //VINEX_SYNTHVOICE_H
