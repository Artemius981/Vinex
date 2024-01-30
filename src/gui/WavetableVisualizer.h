#ifndef WAVETABLEVISUALIZER_H
#define WAVETABLEVISUALIZER_H

#include <JuceHeader.h>
#include "../service/WavetableManager.h"

class WavetableVisualizer : public juce::Component {
public:
    explicit WavetableVisualizer(service::WavetableManager& wavetableManager);

    void paint(Graphics& g) override;

private:
    service::WavetableManager& wavetableManager;
};

#endif //WAVETABLEVISUALIZER_H
