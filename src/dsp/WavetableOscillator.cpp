#include "WavetableOscillator.h"

WavetableOscillator::WavetableOscillator() : wavetable(), currentIndex(0.0), sampleRate(constants::defaultSampleRate), isActive(false)
{}

WavetableOscillator::WavetableOscillator(std::vector<float>& wavetable) : wavetable(wavetable), sampleRate(constants::defaultSampleRate), currentIndex(0.0), isActive(false)
{}

void WavetableOscillator::setFrequency(double newFreq)
{
    this->freq = newFreq;
}

void WavetableOscillator::setParams(std::atomic<float>* oct, std::atomic<float>* phase, std::atomic<float>* pan, std::atomic<float>* level)
{
    this->oct = oct;
    this->phase = phase;
    this->pan = pan;
    this->level = level;
}

void WavetableOscillator::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (freq == 0) return;

    double inc = freq / sampleRate;
    double currentSample = 0.0;
    int tableSize = constants::wavetableSize;
    float* table = wavetable.getDataPtrForIncrement(inc);

    for (int i = startSample; i < startSample + numSamples; ++i)
    {
        auto index0 = (unsigned int) currentIndex;
        auto index1 = index0 == (tableSize - 1) ? (unsigned int) 0 : index0 + 1;

        auto frac = currentIndex - (float) index0;

        auto value0 = table[index0];
        auto value1 = table[index1];

        currentSample = value0 + frac * (value1 - value0);

        if ((currentIndex += inc * tableSize) >= tableSize) currentIndex -= tableSize;

        for (int j = 0; j < outputBuffer.getNumChannels(); ++j)
        {
            outputBuffer.addSample(j, i, (*level) * currentSample);
        }
    }
}

void WavetableOscillator::setWavetable(std::vector<float>& newWavetable)
{
    wavetable.setWavetable(newWavetable);
}

void WavetableOscillator::stop()
{
    currentIndex = 0.0;
    freq = 0.0;
}

void WavetableOscillator::setSampleRate(double sampleRate)
{
    WavetableOscillator::sampleRate = sampleRate;
    wavetable.setSampleRate(sampleRate);
}

