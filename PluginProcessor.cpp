/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Oscillators.h"

//==============================================================================
OsctestAudioProcessor::OsctestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
	treeState(*this, nullptr, "PARAMETERS", {
		std::make_unique<AudioParameterFloat>("masterGain", "Master Gain", NormalisableRange<float>(0.0f, 1.0f), 1.0f),

		std::make_unique<AudioParameterInt>("osc1Oct", "OCT OSC1", -4, 4, 0),
		std::make_unique<AudioParameterInt>("osc1Semi", "Semi OSC1", -12, 12, 0),
		std::make_unique<AudioParameterFloat>("osc1Pan", "Panning OSC1", NormalisableRange<float>(-1.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterFloat>("osc1Gain", "Gain OSC1", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
		std::make_unique<AudioParameterInt>("osc1WTPos", "Wavetable Position OSC1", 0, 1024, 0),
		std::make_unique<AudioParameterFloat>("osc1TableMod", "Wavetable Modification OSC1", NormalisableRange<float>(-1.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterInt>("osc1Phase", "Phase OSC1", 0, 1024, 0),
		std::make_unique<AudioParameterBool>("osc1Status", "Status OSC1", 1),
		std::make_unique<AudioParameterChoice>("osc1Wavetable", "Wavetable OSC1", StringArray("Sine", "Sawtooth", "Square", "Custom"), 0),

		std::make_unique<AudioParameterInt>("osc2Oct", "OCT OSC2", -4, 4, 0),
		std::make_unique<AudioParameterInt>("osc2Semi", "Semi OSC2", -12, 12, 0),
		std::make_unique<AudioParameterFloat>("osc2Pan", "Panning OSC2", NormalisableRange<float>(-1.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterFloat>("osc2Gain", "Gain OSC2", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
		std::make_unique<AudioParameterInt>("osc2WTPos", "Wavetable Position OSC2", 0, 1024, 0),
		std::make_unique<AudioParameterFloat>("osc2TableMod", "Wavetable Modification OSC2", NormalisableRange<float>(-1.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterInt>("osc2Phase", "Phase OSC2", 0, 1024, 0),
		std::make_unique<AudioParameterBool>("osc2Status", "Status OSC2", 1),
		std::make_unique<AudioParameterChoice>("osc2Wavetable", "Wavetable OSC2", StringArray("Sine", "Sawtooth", "Square", "Custom"), 0),

		std::make_unique<AudioParameterFloat>("env1Attack", "Envelope 1 Attack", NormalisableRange<float>(0.01f, 5.0f), 0.01f),
		std::make_unique<AudioParameterFloat>("env1Decay", "Envelope 1 Decay", NormalisableRange<float>(0.1f, 2.0f), 0.1f),
		std::make_unique<AudioParameterFloat>("env1Sustain", "Envelope 1 Sustain", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
		std::make_unique<AudioParameterFloat>("env1Release", "Envelope 1 Release", NormalisableRange<float>(0.01f, 5.0f), 0.01f),
		std::make_unique<AudioParameterFloat>("env1Value", "ADSR Value", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterBool>("env1Status", "ADSR Status", 0),

		std::make_unique<AudioParameterFloat>("filterCutoff", "Filter Cutoff", NormalisableRange<float>(10, 22000, 1, 0.1822), 22000),
		std::make_unique<AudioParameterFloat>("filterRes", "Filter Resonanse", NormalisableRange<float>(0.0f, 3.0f), 0.5f),
		std::make_unique<AudioParameterFloat>("filterDrive", "Filter Drive", NormalisableRange<float>(0.0f, 1.0f), 0.0f),
		std::make_unique<AudioParameterFloat>("filterGain", "Filter Level", NormalisableRange<float>(0.0f, 1.0f), 1.0f),
		std::make_unique<AudioParameterChoice>("filterType", "Filter Type", StringArray("Lowpass", "Bandpass", "Highpass"), 0),
		std::make_unique<AudioParameterBool>("filterRouteOsc1", "Route to OSC1", 1),
		std::make_unique<AudioParameterBool>("filterRouteOsc2", "Route to OSC2", 0),
		std::make_unique<AudioParameterBool>("filterRouteMaster", "Route to Master", 0)
		})
#endif
{
	treeState.state = ValueTree("savedParams");
	//currentTables.reserve(2);
	currentTables[0] = 0;
	currentTables[1] = 0;

	suka.resize(tableSize);
	//calling to make basic wavetables: sin, square, triangle, sawtooth;
	createSinTable();
	createSawtoothTable();
	createSquareTable();
	//createTriTable();

	Synth.clearVoices();

	for (int i = 0; i < numVoices; i++) {
		//Logger::outputDebugString(std::to_string(wavetables[0][512]));
		Synth.addVoice(new SynthVoice(wavetables[currentTables[0]], wavetables[currentTables[1]], env1Value, env1Status));
	}
	Synth.clearSounds();
	Synth.addSound(new SynthSound());
}

OsctestAudioProcessor::~OsctestAudioProcessor()
{
}
//===================================
void OsctestAudioProcessor::createSinTable() {
	auto* samples = suka.getRawDataPointer();

	auto angleDelta = MathConstants<double>::twoPi / tableSize;
	auto currentAngle = 0.0;

	for (int i = 0; i < tableSize; ++i) {
		auto sample = std::sin(currentAngle);
		samples[i] = (float)sample;
		currentAngle += angleDelta;
	}
	wavetables.add(suka);
}

void OsctestAudioProcessor::createSawtoothTable() {
	auto angleDelta = MathConstants<double>::twoPi / tableSize;
	auto currentAngle = 0.0;

	auto* samples = suka.getRawDataPointer();
	for (int i = 0; i < tableSize; i++) {
		auto sample = 2 * (currentAngle / MathConstants<float>::twoPi - floor(currentAngle / MathConstants<float>::twoPi + 0.5));
		samples[i] = sample;
		currentAngle += angleDelta;
	}
	wavetables.add(suka);
}
void OsctestAudioProcessor::createSquareTable() {
	auto* samples = suka.getRawDataPointer();

	for (int i = 0; i < tableSize; i++) {
		auto sample = i < tableSize / 2 ? 1.0 : -1.0;
		samples[i] = (float)sample;
	}
	wavetables.add(suka);
}

//===================================
void OsctestAudioProcessor::setVoiceWavetable(int boxId, int oscId) {
	for (int i = 0; i < numVoices; i++) {
		if ((Voice = dynamic_cast<SynthVoice*>(Synth.getVoice(i))))
			Voice->setWavetable(wavetables[boxId], oscId);
	}
}
void OsctestAudioProcessor::setFilterType(int typeId) {
	for (int i = 0; i < numVoices; i++) {
		if ((Voice = dynamic_cast<SynthVoice*>(Synth.getVoice(i))))
			Voice->setFilterType(typeId);
	}
}

//==============================================================================
const String OsctestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OsctestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OsctestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OsctestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OsctestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OsctestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OsctestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OsctestAudioProcessor::setCurrentProgram (int index)
{
}

const String OsctestAudioProcessor::getProgramName (int index)
{
    return {};
}

void OsctestAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void OsctestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	lastSampleRate = sampleRate;

	Synth.setCurrentPlaybackSampleRate(sampleRate);

	for (int i = 0; i < numVoices; i++) {
		if ((Voice = dynamic_cast<SynthVoice*>(Synth.getVoice(i))))
			Voice->setDSPSpecs(samplesPerBlock, getTotalNumOutputChannels());
	}

	//auto numberOfOscillators = 1;

	//for (int i = 0; i < numberOfOscillators; ++i) {

	//	auto* oscillator = new WavetableOscillator(sineTable);
	//	oscillator->setFrequency(220, lastSampleRate);

	//	oscillators.add(oscillator);
	//}

	//level = 0.25f / numberOfOscillators;
}

void OsctestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OsctestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OsctestAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	for (int i = 0; i < numVoices; i++) {
		if ((Voice = dynamic_cast<SynthVoice*>(Synth.getVoice(i)))){
			Voice->setMainParams(
				treeState.getRawParameterValue("masterGain"),
				treeState.getRawParameterValue("osc1Status"),
				treeState.getRawParameterValue("osc2Status")
			);
			Voice->setOsc1Params(
				treeState.getRawParameterValue("osc1Oct"),
				treeState.getRawParameterValue("osc1Semi"),
				treeState.getRawParameterValue("osc1Pan"),
				treeState.getRawParameterValue("osc1Gain"),
				treeState.getRawParameterValue("osc1WTPos"),
				treeState.getRawParameterValue("osc1Phase")
			);
			Voice->setOsc2Params(
				treeState.getRawParameterValue("osc2Oct"),
				treeState.getRawParameterValue("osc2Semi"),
				treeState.getRawParameterValue("osc2Pan"),
				treeState.getRawParameterValue("osc2Gain"),
				treeState.getRawParameterValue("osc2WTPos"),
				treeState.getRawParameterValue("osc2Phase")
			);
			Voice->setADSRParameters(
				treeState.getRawParameterValue("env1Attack"),
				treeState.getRawParameterValue("env1Decay"),
				treeState.getRawParameterValue("env1Sustain"),
				treeState.getRawParameterValue("env1Release")
			);
			Voice->updateFilter(
				treeState.getRawParameterValue("filterCutoff"),
				treeState.getRawParameterValue("filterRes"),
				treeState.getRawParameterValue("filterDrive"),
				treeState.getRawParameterValue("filterGain"),
				treeState.getRawParameterValue("filterRouteOsc1"),
				treeState.getRawParameterValue("filterRouteOsc2"),
				treeState.getRawParameterValue("filterRouteMaster")
			);
		}
	}
	buffer.clear();
	Synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool OsctestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* OsctestAudioProcessor::createEditor()
{
    return new OsctestAudioProcessorEditor (*this);
}

//==============================================================================
void OsctestAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	std::unique_ptr <XmlElement> xml(treeState.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void OsctestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr <XmlElement> xmlParams(getXmlFromBinary(data, sizeInBytes));
	if (xmlParams != nullptr) {
		if (xmlParams->hasTagName(treeState.state.getType())) {
			treeState.state = ValueTree::fromXml(*xmlParams);
		}
	}

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OsctestAudioProcessor();
}
