#ifndef VINEX_WAVETABLE_H
#define VINEX_WAVETABLE_H

#include <JuceHeader.h>
#include <array>
#include "../Constants.h"

struct Subtable
{
    std::array<float, constants::wavetableSize> table {};
    double maxInc = 0;
};

class Wavetable
{
public:
    explicit Wavetable(double sampleRate = constants::defaultSampleRate) : originalTable(constants::wavetableSize, 0), tables(constants::numOfSubtables), sampleRate(sampleRate)
    {
        bakeTables(originalTable);
    }
    explicit Wavetable(std::vector<float> &wavetable, double sampleRate = constants::defaultSampleRate);

    void setWavetable(std::vector<float> &newWavetable);
    void setSampleRate(double newSampleRate);

    float* getDataPtrForIncrement(double inc);

private:

    void bakeTables(std::vector<float>& wavetable);

private:
    std::vector<float> originalTable;
    std::vector<Subtable> tables;
    double sampleRate;
};


#endif //VINEX_WAVETABLE_H
