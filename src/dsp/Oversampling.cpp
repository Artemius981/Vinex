#include "Oversampling.h"
#include <memory>
#include "../DspConstants.h"

Oversampling::Oversampling(int numChannels, Oversampling::FilterType filterType)
{
    switch(filterType)
    {
        case IIRCheby2:
            core = std::make_unique<OversamplingIIRCore>(numChannels, dsp_constants::cheby2HalfBandFilter);
            break;
        case IIRElliptic:
            core = std::make_unique<OversamplingIIRCore>(numChannels, dsp_constants::ellipHalfBandFilter);
            break;
        case FIREquiripple:
            core = std::make_unique<OversamplingFIRCore>(numChannels, dsp_constants::equirippleHalfBandFilter);
            break;
        default:
            break;
    };
}

void Oversampling::setup(int maxNumSamples)
{
    core->setup(maxNumSamples);
}

void Oversampling::processSamplesUp(const dsp::AudioBlock<const float>& block)
{
    core->processSamplesUp(block);
}

void Oversampling::processSamplesDown(dsp::AudioBlock<float> block)
{
    core->processSamplesDown(block);
}

dsp::AudioBlock<float> Oversampling::getUnprocessedUpsampleBlock(int numSamples)
{
    return core->getProcessedSamples(2 * numSamples);
}

OversamplingIIRCore::OversamplingIIRCore(int numChannels, const std::array<std::vector<float>, 2>& coefficients)
        : OversamplingCore(numChannels)
{
    coeffsB = coefficients[0];
    coeffsA = coefficients[1];

    int Nx = coeffsB.size();
    int Ny = coeffsA.size();

    stateUp = {juce::AudioBuffer<float>(numChannels, Nx), juce::AudioBuffer<float>(numChannels, Ny)};
    stateDown = {juce::AudioBuffer<float>(numChannels, Nx), juce::AudioBuffer<float>(numChannels, Ny)};

    for (int i = 0; i < 2; ++i)
    {
        stateUp[i].clear();
        stateDown[i].clear();
    }
}

void OversamplingIIRCore::setup(int maxNumSamples)
{
    buffer.setSize(numChannels, 2 * maxNumSamples, false, false, false);
}

void OversamplingIIRCore::processSamplesUp(const dsp::AudioBlock<const float>& block)
{
    int Nx = stateUp[0].getNumSamples();
    int Ny = stateUp[1].getNumSamples();
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto xBuf = stateUp[0].getWritePointer(channel);
        auto yBuf = stateUp[1].getWritePointer(channel);
        auto inputSamples = block.getChannelPointer(channel);
        auto samples = buffer.getWritePointer(channel);

        auto numSamples = block.getNumSamples();
        auto numOverSamples = 2 * numSamples;

        for (int i = 0; i < numSamples; ++i)
        {
            samples[i << 1] = inputSamples[i];
            samples[(i << 1) + 1] = 0;
        }

        for (int i = 0; i < numOverSamples; ++i)
        {
            float out = 0;

            xBuf[Nx - 1] = 2 * samples[i];

            for (int j = 0; j < Nx; ++j)
            {
                out += xBuf[Nx - j - 1] * coeffsB[j];
            }

            for (int j = 0; j < Nx - 1; ++j)
            {
                xBuf[j] = xBuf[j + 1];
            }

            for (int j = 1; j < Ny; ++j)
            {
                out -= yBuf[Ny - j - 1] * coeffsA[j];
            }

            yBuf[Ny - 1] = out;

            for (int j = 0; j < Ny - 1; ++j)
            {
                yBuf[j] = yBuf[j + 1];
            }
            samples[i] = out;
        }
    }
}

void OversamplingIIRCore::processSamplesDown(dsp::AudioBlock<float> &block)
{
    int Nx = stateDown[0].getNumSamples();
    int Ny = stateDown[1].getNumSamples();
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto xBuf = stateDown[0].getWritePointer(channel);
        auto yBuf = stateDown[1].getWritePointer(channel);
        auto outputSamples = block.getChannelPointer(channel);
        auto samples = buffer.getWritePointer(channel);

        auto numSamples = block.getNumSamples();
        auto numOverSamples = 2 * numSamples;

        for (int i = 0; i < numOverSamples; ++i)
        {
            float out = 0;

            xBuf[Nx - 1] = samples[i];

            for (int j = 0; j < Nx; ++j)
            {
                out += xBuf[Nx - j - 1] * coeffsB[j];
            }

            for (int j = 0; j < Nx - 1; ++j)
            {
                xBuf[j] = xBuf[j + 1];
            }

            for (int j = 1; j < Ny; ++j)
            {
                out -= yBuf[Ny - j - 1] * coeffsA[j];
            }

            yBuf[Ny - 1] = out;

            for (int j = 0; j < Ny - 1; ++j)
            {
                yBuf[j] = yBuf[j + 1];
            }
            samples[i] = out;
        }

        for (int i = 0; i < numSamples; ++i)
        {
            outputSamples[i] = samples[i << 1];
        }
    }
}

OversamplingFIRCore::OversamplingFIRCore(int numChannels, const std::vector<float> &coefficients)
        : OversamplingCore(numChannels)
{
    coeffs = coefficients;

    int N = coeffs.size();

    stateUp.setSize(numChannels, N);
    stateDown.setSize(numChannels, N);

    stateUp.clear();
    stateDown.clear();

}

void OversamplingFIRCore::setup(int maxNumSamples)
{
    buffer.setSize(numChannels, 2 * maxNumSamples, false, false, false);
}

void OversamplingFIRCore::processSamplesUp(const dsp::AudioBlock<const float> &block)
{
    int N = stateUp.getNumSamples();
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto xBuf = stateUp.getWritePointer(channel);
        auto inputSamples = block.getChannelPointer(channel);
        auto samples = buffer.getWritePointer(channel);

        auto numSamples = block.getNumSamples();
        auto numOverSamples = 2 * numSamples;

        for (int i = 0; i < numSamples; ++i)
        {
            samples[i << 1] = inputSamples[i];
            samples[(i << 1) + 1] = 0;
        }

        for (int i = 0; i < numOverSamples; ++i)
        {
            float out = 0;

            xBuf[N - 1] = 2 * samples[i];

            for (int j = 0; j < N; ++j)
            {
                out += xBuf[N - j - 1] * coeffs[j];
            }

            for (int j = 0; j < N - 1; ++j)
            {
                xBuf[j] = xBuf[j + 1];
            }

            samples[i] = out;
        }
    }
}

void OversamplingFIRCore::processSamplesDown(dsp::AudioBlock<float> &block)
{
    int N = stateDown.getNumSamples();
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto xBuf = stateDown.getWritePointer(channel);
        auto outputSamples = block.getChannelPointer(channel);
        auto samples = buffer.getWritePointer(channel);

        auto numSamples = block.getNumSamples();
        auto numOverSamples = 2 * numSamples;

        for (int i = 0; i < numOverSamples; ++i)
        {
            float out = 0;

            xBuf[N - 1] = samples[i];

            for (int j = 0; j < N; ++j)
            {
                out += xBuf[j] * coeffs[j];
            }

            for (int j = 0; j < N - 1; ++j)
            {
                xBuf[j] = xBuf[j + 1];
            }

            samples[i] = out;
        }

        for (int i = 0; i < numSamples; ++i)
        {
            outputSamples[i] = samples[i << 1];
        }
    }
}
