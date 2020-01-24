/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "SynthVoice.h"

#define TABLE_SIZE 1024

//==============================================================================
/**
*/
class OsctestAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    OsctestAudioProcessor();
    ~OsctestAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//==============================================================================
	void createSinTable();
	void createSawtoothTable();
	void createSquareTable();

	//==============================================================================
	void setVoiceWavetable(int boxId, int oscId);
	void setFilterType(int typeId);

	AudioSampleBuffer sineTable;
	AudioSampleBuffer sawtoothTable;
	AudioSampleBuffer squareTable;

	Array<Array<float>> wavetables;
	Array<float> suka;
	OwnedArray<WavetableOscillator> oscillators;
	int currentTables[2];
	//std::vector<int> currentTables;

	AudioProcessorValueTreeState treeState;

	bool env1Status = 0;
	float env1Value = 0.0f;

private:
	float lastSampleRate;
	const int tableSize = 1 << 11;
	float level;

	int numVoices = 5;

	Synthesiser Synth;
	SynthVoice* Voice;

	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OsctestAudioProcessor)
};
