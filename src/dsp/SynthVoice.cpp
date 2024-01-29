#include "SynthVoice.h"
#include "SynthSound.h"

bool SynthVoice::canPlaySound(SynthesiserSound *sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound *sound, int currentPitchWheelPosition)
{
    auto freq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    for (auto& osc : oscillators)
        osc.setFrequency(freq);
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    for (auto& osc : oscillators)
        osc.stop();
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
    for (auto& osc : oscillators)
        osc.renderNextBlock(outputBuffer, startSample, numSamples);
}

void SynthVoice::setWavetable(OscillatorId id, std::vector<float> &wavetable)
{
    oscillators[id].setWavetable(wavetable);
}

void SynthVoice::setOscParams(OscillatorId id, std::atomic<float> *oct, std::atomic<float> *phase, std::atomic<float> *pan, std::atomic<float> *level)
{
    oscillators[id].setParams(oct, phase, pan, level);
}

SynthVoice::SynthVoice()
{
    for (auto& osc : oscillators)
        osc.setSampleRate(getSampleRate());
}

void SynthVoice::setSampleRate(double sampleRate)
{
    SynthVoice::sampleRate = sampleRate;
    for (auto& osc : oscillators)
        osc.setSampleRate(getSampleRate());
}
