#include <juce_dsp/juce_dsp.h>
#include "Wavetable.h"
#include "../kissfft/kiss_fft.h"
#include "../kissfft/kiss_fftr.h"
#include <fstream>

Wavetable::Wavetable(std::vector<float> &wavetable, double sampleRate) : originalTable(wavetable), tables(constants::numOfSubtables), sampleRate(sampleRate)
{
    bakeTables(wavetable);
}

void Wavetable::setWavetable(std::vector<float> &newWavetable)
{
    originalTable = newWavetable;
    bakeTables(newWavetable);
}

void Wavetable::setSampleRate(double newSampleRate)
{
    this->sampleRate = newSampleRate;
    bakeTables(originalTable);
}

void Wavetable::bakeTables(std::vector<float> &wavetable)
{
    int wtSize = wavetable.size();
    int fftSize = wtSize;

    kiss_fftr_cfg cfg = kiss_fftr_alloc(fftSize, 0, 0, 0);
    kiss_fftr_cfg icfg = kiss_fftr_alloc(fftSize, 1, 0, 0);

    kiss_fft_scalar *cxInput = new kiss_fft_scalar[fftSize];
    kiss_fft_cpx *cxOutput = new kiss_fft_cpx[fftSize / 2 + 1];
    kiss_fft_cpx* IcxInput = new kiss_fft_cpx[fftSize / 2 + 1];
    kiss_fft_scalar* IcxOutput = new kiss_fft_scalar[fftSize];

    for (int i = 0; i < fftSize; ++i)
    {
        cxInput[i] = wavetable[i];
    }

    kiss_fftr(cfg, cxInput, cxOutput);

    cxOutput[0].r = 0;
    cxOutput[0].i = 0;
    cxOutput[fftSize / 2].r = 0;
    cxOutput[fftSize / 2].i = 0;

    int maxBaseHarmonic = wtSize / 2 - sampleRate / (40 << 1);

    for (int i = 0; i < maxBaseHarmonic; i++)
    {
        cxOutput[fftSize / 2 - i].r = 0;
        cxOutput[fftSize / 2 - i].i = 0;
    }

    double maxHarmonic = fftSize >> 1;
    double maxInc = 1. / 2. / maxHarmonic;

    for (int i = 0; i < constants::numOfSubtables; ++i)
    {
        zeromem(IcxOutput, fftSize);
        memset(IcxInput, 0.0f, sizeof(IcxInput) * ((fftSize / 2) + 1));
        for (int j = 0; j <= maxHarmonic; j++) {
            IcxInput[j].r = cxOutput[j].r;
            IcxInput[j].i = cxOutput[j].i;
        }
        kiss_fftri(icfg, IcxInput, IcxOutput);

        for (int j = 0; j < wtSize; ++j)
        {
            tables[i].table[j] = IcxOutput[j] / fftSize;
        }
        tables[i].maxInc = maxInc;

        maxHarmonic /= 2;
        maxInc *= 2;
    }

    free(cfg);
    free(icfg);
    delete[] cxInput;
    delete[] cxOutput;
    delete[] IcxInput;
    delete[] IcxOutput;

}

float* Wavetable::getDataPtrForIncrement(double inc)
{
    int id = 0;
    for (int i = 0; i < constants::numOfSubtables - 1; ++i)
    {
        if (inc >= tables[i].maxInc) ++id;
    }
    return tables[id].table.data();
}
