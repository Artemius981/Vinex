#ifndef VINEX_PRESETMANAGER_H
#define VINEX_PRESETMANAGER_H

#include <JuceHeader.h>

namespace service {
    class PresetManager : public ValueTree::Listener
    {
    public:
        static const File defaultDirectory;
        static const String extension;
        static const String presetNameProperty;

        explicit PresetManager(AudioProcessorValueTreeState& apvts);

        void savePreset(const String& presetName);
        void deletePreset(const String& presetName);
        void loadPreset(const String& presetName);
        int loadNextPreset();
        int loadPreviousPreset();
        String getCurrentPreset() const;

        const StringArray* getPresets(bool refreshNeeded = false);
        void refreshPresets();

        void valueTreeRedirected(ValueTree &treeWhichHasBeenChanged) override;

    private:
        AudioProcessorValueTreeState& apvts;

        StringArray presets;
        Value currentPreset;

        JUCE_DECLARE_NON_COPYABLE(PresetManager);
    };
}
#endif //VINEX_PRESETMANAGER_H
