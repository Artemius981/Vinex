#ifndef VINEX_WAVETABLEOSCILLATOR_H
#define VINEX_WAVETABLEOSCILLATOR_H

#include "Wavetable.h"

class WavetableOscillator
{
public:
    WavetableOscillator();

    explicit WavetableOscillator(std::vector<float>& wavetable);

    void setFrequency(double freq);
    void setParams(std::atomic<float> *oct, std::atomic<float> *phase, std::atomic<float> *pan, std::atomic<float> *level);
    void renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples);
    void setWavetable(std::vector<float>& newWavetable);

    void setSampleRate(double sampleRate);

    void stop();

private:
    Wavetable wavetable;
    double sampleRate{};
    double freq{};

    double currentIndex;
    bool isActive;

    std::atomic<float>* oct{};
    std::atomic<float>* phase{};
    std::atomic<float>* pan{};
    std::atomic<float>* level{};
};


#endif //VINEX_WAVETABLEOSCILLATOR_H
