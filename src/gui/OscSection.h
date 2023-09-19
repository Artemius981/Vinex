#ifndef VINEX_OSCSECTION_H
#define VINEX_OSCSECTION_H

#include "Section.h"
#include "Knob.h"
#include "../PluginProcessor.h"

class OscSection : public Section
{
    typedef juce::AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;
public:
    OscSection(int id, VinexAudioProcessor& processor, juce::AudioProcessorValueTreeState& apvts);

    void paint(Graphics& g) override;
    void resized() override;

private:
    void performKnobLayout(Rectangle<float> bounds) const;
    void changeWaveform();

    VinexAudioProcessor& processor;
    juce::AudioProcessorValueTreeState& apvts;

    std::unique_ptr<ComboBoxAttachment> waveSelectorAttachment;
    String prefix;

    juce::OwnedArray<Knob> basicKnobs;
    juce::OwnedArray<Knob> unisonKnobs;
    ComboBox waveSelector;
};


#endif //VINEX_OSCSECTION_H
