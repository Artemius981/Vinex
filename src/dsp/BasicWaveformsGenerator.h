#ifndef VINEX_BASICWAVEFORMSGENERATOR_H
#define VINEX_BASICWAVEFORMSGENERATOR_H

#include <JuceHeader.h>
#include "../Constants.h"

class BasicWaveformsGenerator
{
public:
    BasicWaveformsGenerator() :  waveforms(3, std::vector<float>(constants::wavetableSize))
    {
        renderSine();
        renderSawtooth();
        renderSquare();
    }

    std::vector<float>& getWTById(int id)
    {
        return waveforms[id];
    }

private:
    void renderSine()
    {
        auto delta = juce::MathConstants<double>::twoPi / (constants::wavetableSize);
        auto currentArg = 0.0;

        for (int i = 0; i < constants::wavetableSize; ++i)
        {
            waveforms[sineIndex][i] = std::sin(currentArg);
            currentArg += delta;
        }
    }

    void renderSawtooth()
    {
        auto delta = juce::MathConstants<double>::twoPi / (constants::wavetableSize - 1);
        auto currentArg = -juce::MathConstants<double>::pi;

        for (int i = 0; i < constants::wavetableSize; ++i)
        {
            waveforms[sawtoothIndex][i] = currentArg / juce::MathConstants<double>::pi;
            currentArg += delta;
        }
    }

    void renderSquare()
    {
        auto delta = juce::MathConstants<double>::twoPi / (constants::wavetableSize - 1);
        auto currentArg = -juce::MathConstants<double>::pi;

        for (int i = 0; i < constants::wavetableSize; ++i)
        {
            waveforms[squareIndex][i] = (currentArg < 0.0 ? 1 : -1);
            currentArg += delta;
        }
    }

    void renderWave(int index, const std::function<float(float)>& f)
    {
        auto delta = juce::MathConstants<float>::twoPi / (constants::wavetableSize - 1);
        auto currentArg = -juce::MathConstants<float>::pi;

        for (int i = 0; i < constants::wavetableSize; ++i)
        {
            waveforms[index][i] = f((float) currentArg);
            currentArg += delta;
        }
    }

private:
    enum
    {
        sineIndex,
        sawtoothIndex,
        squareIndex
    };

    std::vector<std::vector<float> > waveforms;
};


#endif //VINEX_BASICWAVEFORMSGENERATOR_H
