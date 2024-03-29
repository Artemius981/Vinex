#pragma once

#include <JuceHeader.h>
#include "dsp/Oversampling.h"
#include "service/PresetManager.h"
#include "service/WavetableManager.h"

//==============================================================================
/**
*/
class VinexAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VinexAudioProcessor();
    ~VinexAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    service::PresetManager& getPresetManager() const;
    service::WavetableManager& getWavetableManager(OscillatorId oscId) const;

    void setWavetable(int id);

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParamLayout();

private:
    juce::AudioProcessorValueTreeState apvts;

    std::unique_ptr<service::PresetManager> presetManager;
    std::array<std::unique_ptr<service::WavetableManager>, oscCount> wavetableManagers;

    std::unique_ptr<Oversampling> oversampling;
    juce::AudioBuffer<float> oversamplingBuffer;

    juce::Synthesiser synth;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VinexAudioProcessor)
};
