/*
  ==============================================================================

    LFOComponent.cpp
    Created: 24 May 2020 6:16:23pm
    Author:  Thomas Royal

  ==============================================================================
*/

#include "LFOComponent.h"

LFOComponent::LFOComponent(HomunculusAudioProcessor& proc, AudioProcessorValueTreeState& ps, HumLook& lf)
: HumComponent(proc, ps, lf)
{
    setupLabel(lfoRateLabel, "LFO Hz.");
    setupLabel(lfoAmountLabel, "LFO %");
    
    setupAttachedSlider(lfoFreqSlider, lfoFreqSliderAttachment, "lfoFreq");
    setupAttachedSlider(lfoAmountSlider, lfoAmountSliderAttachment, "lfoAmount");
}

LFOComponent::~LFOComponent(){};

void LFOComponent::resized() {
    lfoRateLabel.setBounds(0, 0, BOX_SIZE, LABEL_SIZE);
    lfoFreqSlider.setBounds(0, LABEL_SIZE, BOX_SIZE, BOX_SIZE);
    
    lfoAmountLabel.setBounds(BOX_SIZE, 0, BOX_SIZE, LABEL_SIZE);
    lfoAmountSlider.setBounds(BOX_SIZE, LABEL_SIZE, BOX_SIZE, BOX_SIZE);
}
