#include "PluginProcessor.h"
#include "gui/PluginEditor.h"
#include "dsp/SynthSound.h"
#include "dsp/SynthVoice.h"
#include <iostream>

//==============================================================================
VinexAudioProcessor::VinexAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
                       , apvts(*this, nullptr, juce::Identifier ("Vinex"), createParamLayout())
#endif
{
    apvts.state.setProperty(service::PresetManager::presetNameProperty, "", nullptr);
    apvts.state.setProperty("version", ProjectInfo::versionString, nullptr);

    auto wavetablesTree = apvts.state.getOrCreateChildWithName("wavetables", nullptr);
    presetManager = std::make_unique<service::PresetManager>(apvts);
    for (int id = osc1; id < oscCount; ++id)
    {
        wavetablesTree.setProperty("osc" + String(id), "Init", nullptr);
        wavetableManagers[id] = std::make_unique<service::WavetableManager>(synth, apvts, static_cast<OscillatorId>(id));
    }

    synth.addSound(new SynthSound());

    for (int i = 0; i < constants::numOfVoices; ++i)
    {
        synth.addVoice(new SynthVoice());
    }

    oversampling = std::make_unique<Oversampling>(getTotalNumOutputChannels(), Oversampling::FilterType::IIRCheby2);
}

VinexAudioProcessor::~VinexAudioProcessor()
{
}

//==============================================================================
const juce::String VinexAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VinexAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VinexAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VinexAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VinexAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VinexAudioProcessor::getNumPrograms()
{
    return 1;
}

int VinexAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VinexAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VinexAudioProcessor::getProgramName (int index)
{
    return {};
}

void VinexAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VinexAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(constants::oversamplingRatio * sampleRate);
    for (int id = osc1; id < oscCount; ++id)
    {
        const auto prefix = String("osc") + String(id);
        auto octParam = apvts.getRawParameterValue(prefix + "Oct");
        auto phaseParam = apvts.getRawParameterValue(prefix + "Phase");
        auto panParam = apvts.getRawParameterValue(prefix + "Pan");
        auto lvlParam = apvts.getRawParameterValue(prefix + "Lvl");
        for(int i = 0; i < synth.getNumVoices(); ++i)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setOscParams(static_cast<OscillatorId>(id), octParam, phaseParam, panParam, lvlParam);
            }
        }
    }

    oversamplingBuffer.setSize(getTotalNumOutputChannels(), constants::oversamplingRatio * samplesPerBlock);
    oversampling->setup(samplesPerBlock);
}

void VinexAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VinexAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void VinexAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    oversamplingBuffer.clear();
    synth.renderNextBlock(oversamplingBuffer, midiMessages, 0, constants::oversamplingRatio * buffer.getNumSamples());

    auto oversamplingInternalBuffer = oversampling->getUnprocessedUpsampleBlock(buffer.getNumSamples());
    oversamplingInternalBuffer.copyFrom(oversamplingBuffer, 0, 0, constants::oversamplingRatio * buffer.getNumSamples());

    oversampling->processSamplesDown(dsp::AudioBlock<float>(buffer));
}

//==============================================================================
bool VinexAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* VinexAudioProcessor::createEditor()
{
    return new VinexAudioProcessorEditor (*this, apvts);
}

//==============================================================================
void VinexAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void VinexAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout VinexAudioProcessor::createParamLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    String oscPrefix = "osc";

    for (int i = osc1; i < oscCount; ++i)
    {
        String id = String(i);
        params.add(std::make_unique<AudioParameterInt>(oscPrefix + id + "Oct", "Octave" + id, -4, 4, 0));
        params.add(std::make_unique<AudioParameterFloat>(oscPrefix + id + "Phase", "Phase" + id, 0, 1, 0));
        params.add(std::make_unique<AudioParameterFloat>(oscPrefix + id + "Pan", "Pan" + id, NormalisableRange<float>(-1, 1), 0));
        params.add(std::make_unique<AudioParameterFloat>(oscPrefix + id + "Lvl", "Level" + id, NormalisableRange<float>(0, 1), 1));
        params.add(std::make_unique<AudioParameterFloat>(oscPrefix + id + "Detune", "Detune" + id, NormalisableRange<float>(0, 1), 0.25));
        params.add(std::make_unique<AudioParameterInt>(oscPrefix + id + "Blend", "Blend" + id, 0, 100, 75));
    }

    return params;
}

service::PresetManager& VinexAudioProcessor::getPresetManager() const
{
    return *presetManager;
}

service::WavetableManager& VinexAudioProcessor::getWavetableManager(OscillatorId oscId) const
{
    return *wavetableManagers[oscId];
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VinexAudioProcessor();
}
