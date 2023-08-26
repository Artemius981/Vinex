#ifndef VINEX_OVERSAMPLING_H
#define VINEX_OVERSAMPLING_H

#include <JuceHeader.h>

/**
 * Only does 2x oversampling.
 * Adapted from JUCE's Oversampling class for educational purposes.
 * Uses basic non-optimized filter implementations.
 **/
class Oversampling
{
public:
    enum FilterType
    {
        IIRCheby2,
        IIRElliptic,
        FIREquiripple,
    };

    Oversampling(int numChannels, FilterType filterType);

    void setup(int maxNumSamples);
    void processSamplesUp(const dsp::AudioBlock<const float>& block);
    void processSamplesDown(dsp::AudioBlock<float> block);
    dsp::AudioBlock<float> getUnprocessedUpsampleBlock(int numSamples);

    class OversamplingCore
    {
    public:
        explicit OversamplingCore(int numChannels) : numChannels(numChannels) {}
        virtual ~OversamplingCore() = default;

        virtual void setup(int maxNumSamples) = 0;
        virtual void processSamplesUp(const dsp::AudioBlock<const float>& block) = 0;
        virtual void processSamplesDown(dsp::AudioBlock<float>& block) = 0;

        dsp::AudioBlock<float> getProcessedSamples(int numSamples)
        {
            return dsp::AudioBlock<float>(buffer).getSubBlock(0, numSamples);
        }

        AudioBuffer<float> buffer;
        int numChannels;
    };

private:

    std::unique_ptr<OversamplingCore> core;
};

class OversamplingIIRCore : public Oversampling::OversamplingCore
{
public:
    OversamplingIIRCore(int numChannels, const std::array<std::vector<float>, 2>& coefficients);
    ~OversamplingIIRCore() override = default;

    void setup(int maxNumSamples) override;
    void processSamplesUp(const dsp::AudioBlock<const float> &block) override;
    void processSamplesDown(dsp::AudioBlock<float> &block) override;

private:
    std::vector<float> coeffsB;
    std::vector<float> coeffsA;

    std::array<juce::AudioBuffer<float>, 2> stateUp;
    std::array<juce::AudioBuffer<float>, 2> stateDown;
};

class OversamplingFIRCore : public Oversampling::OversamplingCore
{
public:
    OversamplingFIRCore(int numChannels, const std::vector<float> &coefficients);
    ~OversamplingFIRCore() override = default;

    void setup(int maxNumSamples) override;
    void processSamplesUp(const dsp::AudioBlock<const float> &block) override;
    void processSamplesDown(dsp::AudioBlock<float> &block) override;

private:
    std::vector<float> coeffs;

    juce::AudioBuffer<float> stateUp;
    juce::AudioBuffer<float> stateDown;
};


#endif //VINEX_OVERSAMPLING_H
