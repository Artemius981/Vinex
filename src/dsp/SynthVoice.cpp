#include "SynthVoice.h"
#include "SynthSound.h"

bool SynthVoice::canPlaySound(SynthesiserSound *sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition)
{
    auto freq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    wtOsc.setFrequency(freq);
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    wtOsc.stop();
    clearCurrentNote();
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    wtOsc.renderNextBlock(outputBuffer, startSample, numSamples);
}

void SynthVoice::setWavetable(std::vector<float> &wavetable)
{
    wtOsc.setWavetable(wavetable);
}

void SynthVoice::setOscParams(int id, std::atomic<float> *oct, std::atomic<float> *phase, std::atomic<float> *pan, std::atomic<float> *level)
{
    wtOsc.setParams(oct, phase, pan, level);
}

SynthVoice::SynthVoice() : wtOsc()
{
    wtOsc.setSampleRate(getSampleRate());
}

void SynthVoice::setSampleRate(double sampleRate)
{
    SynthVoice::sampleRate = sampleRate;
    wtOsc.setSampleRate(sampleRate);
}
