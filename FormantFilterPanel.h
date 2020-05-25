/*
  ==============================================================================

    FormantFilterPanel.h
    Created: 24 May 2020 7:48:56pm
    Author:  Thomas Royal

  ==============================================================================
*/

#pragma once

#include "HumConstants.h"
#include "HumComponent.h"
#include "FormantManager.h"
#include <JuceHeader.h>

class FormantFilterPanel : public HumComponent {
public:
    FormantFilterPanel(HomunculusAudioProcessor& proc, AudioProcessorValueTreeState& ps, HumLook& lf, FormantManager& fmgr);
    
    ~FormantFilterPanel();
    
    void setupFormantSliders();
    
    void resized() override ;
    
    void syncFormantManager();
    
private:
    FormantManager& formantManager;
    
    Label f1Label;
    Label f2Label;
    Label f3Label;
    Label freqLabel;
    Label QLabel;
    Label gainLabel;
    
    OwnedArray<Slider> sliders;

};
