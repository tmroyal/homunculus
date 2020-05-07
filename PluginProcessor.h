/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "HumConstants.h"
#include <JuceHeader.h>
#include <vector>
#include "BlitSynth.h"
#include "HumBPF.h"
//==============================================================================
/**
*/
class HomunculusAudioProcessor  : public AudioProcessor, public AudioProcessorValueTreeState::Listener
{
    using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = AudioProcessorGraph::Node;
    
public:
    //==============================================================================
    HomunculusAudioProcessor();
    ~HomunculusAudioProcessor();

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

    // APVTS::Listener
    void parameterChanged (const String& parameterID, float newValue) override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    Synthesiser blitSynth;
    
    std::unique_ptr<AudioProcessorGraph> filterBankGraph;
    Node::Ptr filterBankInputNode;
    Node::Ptr filterBankOutputNode;
    std::vector<Node::Ptr> filters;
    
    AudioProcessorValueTreeState params;
    
    static AudioProcessorValueTreeState::ParameterLayout createLayout(){
        std::vector<std::unique_ptr<AudioParameterFloat>> newParams;
        
        for (int i = 0; i < NUMBER_OF_FORMANTS; ++i){
            std::string num = std::to_string(i + 1);
            
            newParams.push_back(std::make_unique<AudioParameterFloat> ("freq"+num, "Freq F"+num, 20.0, 20000, 440));
            newParams.push_back(std::make_unique<AudioParameterFloat> ("Q"+num, "Q F"+num, 0.01, 10.0, 1.0));
            newParams.push_back(std::make_unique<AudioParameterFloat> ("gain"+num, "Gain F"+num, 0.0, 1.0, 0.3));
        }
        
        return { newParams.begin(), newParams.end() };
    }
        
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomunculusAudioProcessor)
};
