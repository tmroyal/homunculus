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
    FormantVisualiser(HomunculusAudioProcessor& prc): processor(prc){
        for (int i = 0; i < VIS_TABLE_SIZE; i++){ visTable[i] = 0.0; }
    }
    
    ~FormantVisualiser(){}
    
    void refreshData(){
        double maxExp = exp(EXP_INDEX);
        
        for (int i = 0; i < VIS_TABLE_SIZE; i++){
            double ind = exp((EXP_INDEX*i)/VIS_TABLE_SIZE)/maxExp;
            //visTable[i] = prc.freqResponseAt(ind*MathConstants<double>*pi);
            visTable[i] = 0.35;
        }
    }
    
    void paint(Graphics& g){
        g.fillAll(Colours::black);

        double prevY = (1-visTable[0])*getHeight();
        double prevX = 0;
        
        g.setColour(Colours::lightgrey);
        
        for (int i = GRAPH_STEP; i < getWidth(); i += GRAPH_STEP){
            // interpolate width % over table size
            double ind = VIS_TABLE_SIZE*i/getWidth();
            int a = floor(ind);
            int b = ceil(ind);
            double intpart;
            double frac = modf(ind, &intpart);
            
            // perform interpolation
            double amp = (1-frac)*visTable[a]+frac*visTable[b];
        
            double thisY = (1-amp)*getHeight();
            
            g.drawLine(prevX, prevY, i, thisY);
            prevX = i;
            prevY = thisY;
        }
        g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 8, 4);
    }
    
private:
    HomunculusAudioProcessor& processor;
    std::array<double, VIS_TABLE_SIZE> visTable;
};
