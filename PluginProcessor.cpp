/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HomunculusAudioProcessor::HomunculusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    auto numVoices = 8;
    
    // Add some voices...
    for (auto i = 0; i < numVoices; ++i)
        blitSynth.addVoice (new BlitSynthVoice());
    
    // ..and give the synth a sound to play
    blitSynth.addSound (new BlitSynthSound());
}

HomunculusAudioProcessor::~HomunculusAudioProcessor()
{
}

//==============================================================================
const String HomunculusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HomunculusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HomunculusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HomunculusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HomunculusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HomunculusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HomunculusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HomunculusAudioProcessor::setCurrentProgram (int index)
{
}

const String HomunculusAudioProcessor::getProgramName (int index)
{
    return {};
}

void HomunculusAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HomunculusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    blitSynth.setCurrentPlaybackSampleRate(sampleRate);
    bpf.prepareToPlay(sampleRate, samplesPerBlock);
}

void HomunculusAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HomunculusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void HomunculusAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    blitSynth.renderNextBlock(buffer, midiMessages, 0, numSamples);
    bpf.processBlock(buffer, midiMessages);
    
    buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
}

//==============================================================================
bool HomunculusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HomunculusAudioProcessor::createEditor()
{
    return new HomunculusAudioProcessorEditor (*this);
}

//==============================================================================
void HomunculusAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HomunculusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HomunculusAudioProcessor();
}
