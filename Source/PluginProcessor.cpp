/*
 Homunculus - formant synth written in JUCE
 Copyright (C) 2020 Thomas Royal
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 
 Source is available at: <https://github.com/tmroyal/homunculus>
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
        blitSynth.addVoice (new BlitSynthVoice(&sineTable));
    
    blitSynth.addSound (new BlitSynthSound());
    
    params.addParameterListener("attack", this);
    params.addParameterListener("decay", this);
    params.addParameterListener("sustain", this);
    params.addParameterListener("release", this);
    params.addParameterListener("interpolate", this);
    params.addParameterListener("lfoFreq", this);
    params.addParameterListener("lfoAmount", this);

    attackParam = params.getRawParameterValue("attack");
    decayParam = params.getRawParameterValue("decay");
    sustainParam = params.getRawParameterValue("sustain");
    releaseParam = params.getRawParameterValue("release");

    lfoFreqParam = params.getRawParameterValue("lfoFreq");
    lfoAmpParam = params.getRawParameterValue("lfoAmount");
    
    interpParam = params.getRawParameterValue("interpolate");
}

HomunculusAudioProcessor::~HomunculusAudioProcessor()
{
}

void HomunculusAudioProcessor::parameterChanged (const String& parameterID, float newValue) {
    
    // the parameter "interpolate" is the only one that requires
    // special treatment. everything else is an lfo or
    // evnvelope setting
    if (initialized){
        if (parameterID == "interpolate"){
            interpolateFormants(newValue);
        } else {
            setSynthParams();
        }
    }
}

void HomunculusAudioProcessor::resetInterpolator(){
    interpolateFormants(*interpParam);
    
}

void HomunculusAudioProcessor::interpolateFormants(float value){
    FormantSet currentSet = formantManager.getInterpolatedFormants(value);
    for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
        Formant fmt = currentSet.getFormant(i);
        setFrequency(i, fmt.freq);
        setGain(i, fmt.gain);
        setQ(i, fmt.Q);
    }
    
    HomunculusAudioProcessorEditor* editor = dynamic_cast<HomunculusAudioProcessorEditor*>(getActiveEditor());
    if (editor != nullptr){
        editor->updateVisualiser();
    }
}


void HomunculusAudioProcessor::setSynthParams(){
    for (auto i = 0; i < NUMBER_OF_VOICES; i++){
        dynamic_cast<BlitSynthVoice*>(
            blitSynth.getVoice(i))->setParams(
                *attackParam, *decayParam, *sustainParam, *releaseParam, *lfoFreqParam, *lfoAmpParam);
    }
}

void HomunculusAudioProcessor::setFrequency(int formant, float freq){
    if (filters.size() > 0){
        dynamic_cast<HumBPF*>(filters[formant]->getProcessor())->setFreq(freq);
    }
}

void HomunculusAudioProcessor::setQ(int formant, float Q){
    if (filters.size() > 0){
        dynamic_cast<HumBPF*>(filters[formant]->getProcessor())->setQ(Q);
    }
}

void HomunculusAudioProcessor::setGain(int formant, float gain){
    if (filters.size() > 0){
        dynamic_cast<HumBPF*>(filters[formant]->getProcessor())->setGain(gain);
    }
}


double HomunculusAudioProcessor::freqResponseAt(double angle){
    float amp = 0.0;
    if (filters.size() > 0){
        for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
            amp += dynamic_cast<HumBPF*>(filters[i]->getProcessor())->freqResponseAt(angle);
        }
    }
    return amp;
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
    keyboardState.reset();
    blitSynth.setCurrentPlaybackSampleRate(sampleRate);
    
    filterBankGraph->setPlayConfigDetails (1, 1, sampleRate, samplesPerBlock);
    filterBankGraph->prepareToPlay (sampleRate, samplesPerBlock);
    filterBankGraph->clear();
    
    // add input and output to filterBank
    filterBankInputNode = filterBankGraph->addNode(std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioInputNode));
    filterBankOutputNode = filterBankGraph->addNode(std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioOutputNode));

    filters.clear();
    
    // create three filters and connect them to the audiograph
    for (auto i = 0; i < NUMBER_OF_FORMANTS; i++){
        Node::Ptr newNode = filterBankGraph->addNode(std::make_unique<HumBPF>());
        Formant fmt = formantManager.getCurrentFormantSet().getFormant(i);
        
        filters.push_back(newNode);
        
        HumBPF* newFilter = dynamic_cast<HumBPF*>(newNode->getProcessor());
        newFilter->setFreq(fmt.freq);
        newFilter->setQ(fmt.Q);
        newFilter->setGain(fmt.gain);
        newFilter->prepareToPlay(sampleRate, samplesPerBlock);

        filterBankGraph->addConnection({
            {filterBankInputNode->nodeID, 0}, {newNode->nodeID, 0}
        });
        
        filterBankGraph->addConnection({
            {newNode->nodeID, 0}, {filterBankOutputNode->nodeID, 0}
        });
    }
    
    // we do this here in the event that filters are cleared
    // and we need to re populate the visualiser
    HomunculusAudioProcessorEditor* editor = dynamic_cast<HomunculusAudioProcessorEditor*>(getActiveEditor());
    if (editor != nullptr){
        editor->updateVisualiser();
    }

    setSynthParams();

    initialized = true;
}

void HomunculusAudioProcessor::releaseResources()
{
     filterBankGraph->releaseResources();
    keyboardState.reset();
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

    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
    
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
    // create xml from parameter state
    auto state = params.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    
    // append formantManager information
    auto* fsets = xml->getChildByName("FormantSets");
    if (fsets != nullptr){
        xml->replaceChildElement(fsets,formantManager.toXml().release());
    } else {
        xml->addChildElement(formantManager.toXml().release());
    }
    
    // copy to settings file
    copyXmlToBinary (*xml, destData);
}

void HomunculusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // make xml from data
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    // set formant manager and interface
    auto * fsets = xmlState->getChildByName("FormantSets");
    
    if (fsets != nullptr){
        formantManager.setFromXml(xmlState->getChildByName("FormantSets"));
        
        xmlState->removeChildElement(fsets, true);
    }
    
    // set params based on remaining xml
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
