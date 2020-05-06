/*
  ==============================================================================

    BPFBank.h
    Created: 4 May 2020 11:44:09am
    Author:  Thomas Royal

  ==============================================================================
*/

#pragma once

#include <vector>
#include <JuceHeader.h>
#include "HumBPF.h"

class BPFBank : public ProcessorBase {
    using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = AudioProcessorGraph::Node;

public:
    BPFBank(int numFilters):
        ProcessorBase(getBusesProperties()),
    {
        for (int i = 0; i < numFilters; i++){
            
        }
    }
private:
    AudioProcessorGraph graph;
    
    BusesProperties getBusesProperties(){
        return BusesProperties().withOutput("Output", AudioChannelSet::discreteChannels(1), true)
        .withInput("Input", AudioChannelSet::discreteChannels(1), true);
    }
};
