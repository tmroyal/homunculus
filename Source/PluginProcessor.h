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


#pragma once

#include "HumConstants.h"
#include <JuceHeader.h>
#include <vector>
#include "BlitSynth.h"
#include "HumBPF.h"
#include "FormantManager.h"
#include "SineTable.h"

//==============================================================================


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
    Synthesiser blitSynth;

    void setFrequency(int formant, float freq);
    void setQ(int formant, float Q);
    void setGain(int formant, float gain);

    
    MidiKeyboardState keyboardState;
    void resetInterpolator();
    
    double freqResponseAt(double angle);
    
private:
    void setSynthParams();
    void interpolateFormants(float value);
    
    std::unique_ptr<AudioProcessorGraph> filterBankGraph;
    Node::Ptr filterBankInputNode;
    Node::Ptr filterBankOutputNode;
    std::vector<Node::Ptr> filters;
    
    AudioProcessorValueTreeState params;
        
    std::atomic<float>* attackParam = nullptr;
    std::atomic<float>* decayParam = nullptr;
    std::atomic<float>* sustainParam = nullptr;
    std::atomic<float>* releaseParam = nullptr;
    std::atomic<float>* lfoFreqParam = nullptr;
    std::atomic<float>* lfoAmpParam = nullptr;
    std::atomic<float>* interpParam = nullptr;

    
    FormantManager formantManager;

    bool initialized = false;
    
    SineTable sineTable;
    
    static AudioProcessorValueTreeState::ParameterLayout createLayout(){
        std::vector<std::unique_ptr<RangedAudioParameter>> newParams;
        
        NormalisableRange<float> envRange(0.0,8.0,0.0,0.3);
        
        newParams.push_back(std::make_unique<AudioParameterFloat>("attack", "Attack",envRange,0.1));
        newParams.push_back(std::make_unique<AudioParameterFloat>("decay", "Decay",envRange,0.3));
        newParams.push_back(std::make_unique<AudioParameterFloat>("sustain", "Sustain",NormalisableRange<float>(0.0,1.0,0.0,0.3),0.9));
        newParams.push_back(std::make_unique<AudioParameterFloat>("release", "Release",envRange,0.5));

        newParams.push_back(std::make_unique<AudioParameterFloat>("interpolate", "Morph Index", 0.0, 1.0, 0.0));
        newParams.push_back(std::make_unique<AudioParameterFloat>("lfoFreq", "LFO Frequency", 0.0, 20.0, 0.0));
        newParams.push_back(std::make_unique<AudioParameterFloat>("lfoAmount", "LFO Amount", 0.0, 1.0, 0.0));
        
        return { newParams.begin(), newParams.end() };
    }
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomunculusAudioProcessor)
};
