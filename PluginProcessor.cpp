/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HomunculusAudioProcessor::HomunculusAudioProcessor():
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
        filterBankGraph  (new AudioProcessorGraph()),
        params(*this, nullptr, Identifier("humunculus"), createLayout())
{
    
    for (auto i = 0; i < NUMBER_OF_VOICES; ++i)
        blitSynth.addVoice (new BlitSynthVoice());
    
    blitSynth.addSound (new BlitSynthSound());
    
    
    // parameter listeners
    for (auto i = 0; i < NUMBER_OF_FORMANTS; i++){
        std::string num = std::to_string(i+1);
        params.addParameterListener("freq"+num, this);
        params.addParameterListener("Q"+num, this);
        params.addParameterListener("gain"+num, this);
    }
    
    bpfParameterMap["freq"] = eFreq;
    bpfParameterMap["Q"] = eQ;
    bpfParameterMap["gain"] = eGain;
    
    adsrNames.insert("attack");
    adsrNames.insert("decay");
    adsrNames.insert("sustain");
    adsrNames.insert("release");
    
    params.addParameterListener("attack", this);
    params.addParameterListener("decay", this);
    params.addParameterListener("sustain", this);
    params.addParameterListener("release", this);
    
    attackParam = params.getRawParameterValue("attack");
    decayParam = params.getRawParameterValue("decay");
    sustainParam = params.getRawParameterValue("sustain");
    releaseParam = params.getRawParameterValue("release");
}

HomunculusAudioProcessor::~HomunculusAudioProcessor()
{
}

void HomunculusAudioProcessor::parameterChanged (const String& parameterID, float newValue) {
    if (initialized){
        for (auto i = 0; i < NUMBER_OF_VOICES; i++){
            dynamic_cast<BlitSynthVoice*>(blitSynth.getVoice(i))->setParams(
                *attackParam, *decayParam, *sustainParam, *releaseParam
            );
        }
    }
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
    
    filterBankGraph->setPlayConfigDetails (1,1, sampleRate, samplesPerBlock);
    
    filterBankGraph->prepareToPlay (sampleRate, samplesPerBlock);
    
    filterBankGraph->clear();
    
    filterBankInputNode = filterBankGraph->addNode(std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioInputNode));
    filterBankOutputNode = filterBankGraph->addNode(std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioOutputNode));

    filters.clear();
    
    for (auto i = 0; i < NUMBER_OF_FORMANTS; i++){
        Node::Ptr newNode = filterBankGraph->addNode(std::make_unique<HumBPF>());
        
        filters.push_back(newNode);
        
        HumBPF* newFilter = dynamic_cast<HumBPF*>(newNode->getProcessor());
        newFilter->setFreq(440);
        newFilter->setQ(4);
        newFilter->setGain(0.2);
        newFilter->prepareToPlay(sampleRate, samplesPerBlock);

        filterBankGraph->addConnection({
            {filterBankInputNode->nodeID, 0}, {newNode->nodeID, 0}
        });
        
        filterBankGraph->addConnection({
            {newNode->nodeID, 0}, {filterBankOutputNode->nodeID, 0}
        });
    }
    
    initialized = true;
}

void HomunculusAudioProcessor::releaseResources()
{
     filterBankGraph->releaseResources();
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

    // clear inputs
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    blitSynth.renderNextBlock(buffer, midiMessages, 0, numSamples);
    filterBankGraph->processBlock (buffer, midiMessages);
    
    // copy channel zero to all other channels
    for (int i = 1; i < totalNumOutputChannels; ++i){
        buffer.copyFrom(i, 0, buffer, 0, 0, numSamples);
    }
    
}

//==============================================================================
bool HomunculusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HomunculusAudioProcessor::createEditor()
{
    return new HomunculusAudioProcessorEditor (*this, params, formantManager);
}

//==============================================================================
void HomunculusAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = params.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    
    //xml->createNewChildElement("formantData");
    //TODO: only one
    //DBG(xml->toString());
    
    
    // formantManager.getState(xml);
    copyXmlToBinary (*xml, destData);
   
}

void HomunculusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    //DBG(xmlState->toString());

    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (params.state.getType()))
            params.replaceState (ValueTree::fromXml (*xmlState));
}



//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HomunculusAudioProcessor();
}


