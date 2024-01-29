#include "WavetableManager.h"
#include "../dsp/SynthVoice.h"
#include <memory>

namespace service
{
    const File WavetableManager::defaultDirectory = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory)
            .getChildFile(ProjectInfo::companyName)
            .getChildFile(ProjectInfo::projectName)
            .getChildFile("wavetables");

    WavetableManager::WavetableManager(Synthesiser& synth,
                                       AudioProcessorValueTreeState& apvts,
                                       OscillatorId oscIdToControl)
    : synth(synth), apvts(apvts), oscId(oscIdToControl)
    {
        formatManager.registerBasicFormats();
        if (!defaultDirectory.exists())
        {
            const auto result = defaultDirectory.createDirectory();
            if (result.failed())
            {
                DBG("Could not create wavetable directory: " + result.getErrorMessage());
                jassertfalse;
            }
        }
        apvts.state.addListener(this);
        auto wavetablesTree = apvts.state.getChildWithName("wavetables");
        currentWavetable.referTo(
            wavetablesTree.getPropertyAsValue("osc" + String(oscId), nullptr));
    }

    void WavetableManager::loadWavetable(const String& wavetableName)
    {
        if (wavetableName.isEmpty())
            return;

        const auto wavetableFile = defaultDirectory.getChildFile(wavetableName + ".wav");
        if (!wavetableFile.existsAsFile())
            return;

        std::unique_ptr<AudioFormatReader> reader{formatManager.createReaderFor(wavetableFile)};
        if (reader == nullptr)
            return;

        AudioBuffer<float> buffer(reader->numChannels, reader->lengthInSamples);
        reader->read(&buffer, 0, reader->lengthInSamples, 0, true, true);

        std::vector<float> wavetable(reader->lengthInSamples);
        const auto bufPtr = buffer.getReadPointer(0);
        for (int i = 0; i < reader->lengthInSamples; ++i)
            wavetable[i] = bufPtr[i];

        for (int i = 0; i < synth.getNumVoices(); ++i)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setWavetable(oscId, wavetable);
            }
        }
        currentWavetable.setValue(wavetableName);
    }

    String WavetableManager::getCurrentWavetableName() const
    {
        return currentWavetable.toString();
    }

    const StringArray* WavetableManager::getWavetables(bool refreshNeeded)
    {
        if (!refreshNeeded)
            return &wavetables;
        refreshWavetables();
        return &wavetables;
    }

    void WavetableManager::refreshWavetables()
    {
        wavetables.clearQuick();
        const auto wavetableFiles = defaultDirectory.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.wav");

        for (const auto& file: wavetableFiles)
            wavetables.add(file.getFileNameWithoutExtension());
    }

    void WavetableManager::valueTreeRedirected(ValueTree& treeWhichHasBeenChanged)
    {
        auto wavetablesTree = apvts.state.getChildWithName("wavetables");
        currentWavetable.referTo(wavetablesTree.getPropertyAsValue("osc" + String(oscId), nullptr));
        loadWavetable(currentWavetable.toString());
    }
}
