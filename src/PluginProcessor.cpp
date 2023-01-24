#include "PluginProcessor.h"
#include "gui/PluginEditor.h"

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
#endif
{
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
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
}

//==============================================================================
bool VinexAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* VinexAudioProcessor::createEditor()
{
    return new VinexAudioProcessorEditor (*this);
}

//==============================================================================
void VinexAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void VinexAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VinexAudioProcessor();
}
