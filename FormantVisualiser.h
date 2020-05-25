/*
  ==============================================================================

    FormantVisualiser.h
    Created: 24 May 2020 3:32:01pm
    Author:  Thomas Royal

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <array>
#include <cmath>

#define VIS_TABLE_SIZE 256
#define GRAPH_STEP 2
// this determines the graph scale
#define EXP_INDEX 5.0


using std::exp;

class FormantVisualiser : public Component {
public:
    FormantVisualiser(HomunculusAudioProcessor& prc);
    
    ~FormantVisualiser();
    
    void refreshData();
    
    void paint(Graphics& g);
    
private:
    HomunculusAudioProcessor& processor;
    std::array<double, VIS_TABLE_SIZE> visTable;
};
