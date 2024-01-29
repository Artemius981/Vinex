#ifndef WAVETABLEMANAGER_H
#define WAVETABLEMANAGER_H

#include <JuceHeader.h>

#include "../dsp/SynthVoice.h"

namespace service
{
    class WavetableManager : public ValueTree::Listener
    {
    public:
        static const File defaultDirectory;

        WavetableManager(Synthesiser& synth, AudioProcessorValueTreeState& apvts, OscillatorId oscIdToControl);

        void loadWavetable(const String& wavetableName);
        String getCurrentWavetableName() const;
        const StringArray* getWavetables(bool refreshNeeded = false);
        void refreshWavetables();

        void valueTreeRedirected(ValueTree& treeWhichHasBeenChanged) override;

    private:
        Synthesiser& synth;
        AudioProcessorValueTreeState& apvts;
        AudioFormatManager formatManager;
        Value currentWavetable;
        StringArray wavetables;
        OscillatorId oscId;
    };
}

#endif //WAVETABLEMANAGER_H
