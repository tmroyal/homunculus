/*
  ==============================================================================

    ADSRPanel.cpp
    Created: 24 May 2020 7:22:58pm
    Author:  Thomas Royal

  ==============================================================================
*/

#include "ADSRComponent.h"

ADSRComponent::ADSRComponent(HomunculusAudioProcessor& proc, AudioProcessorValueTreeState& ps, HumLook& lf)
: HumComponent(proc, ps, lf)
{
    setupLabel(envALabel, "A");
    setupLabel(envDLabel, "D");
    setupLabel(envSLabel, "S");
    setupLabel(envRLabel, "R");
    
    setupAttachedSlider(attackSlider, attackAttachment, "attack");
    setupAttachedSlider(decaySlider, decayAttachment, "decay");
    setupAttachedSlider(sustainSlider, sustainAttachment, "sustain");
    setupAttachedSlider(releaseSlider, releaseAttachment, "release");
    
    attackSlider.setNumDecimalPlacesToDisplay(3);
    decaySlider.setNumDecimalPlacesToDisplay(3);
    sustainSlider.setNumDecimalPlacesToDisplay(3);
    releaseSlider.setNumDecimalPlacesToDisplay(3);
}

ADSRComponent::~ADSRComponent(){}

void ADSRComponent::resized() {
    envALabel.setBounds(0, 0, BOX_SIZE, BOX_SIZE);
    envDLabel.setBounds(0, BOX_SIZE, BOX_SIZE, BOX_SIZE);
    envSLabel.setBounds(0, BOX_SIZE*2, BOX_SIZE, BOX_SIZE);
    envRLabel.setBounds(0, BOX_SIZE*3, BOX_SIZE, BOX_SIZE);
    
    attackSlider.setBounds(LABEL_SIZE*2.5, 0, BOX_SIZE, BOX_SIZE);
    decaySlider.setBounds(LABEL_SIZE*2.5, BOX_SIZE, BOX_SIZE, BOX_SIZE);
    sustainSlider.setBounds(LABEL_SIZE*2.5, 2*BOX_SIZE, BOX_SIZE, BOX_SIZE);
    releaseSlider.setBounds(LABEL_SIZE*2.5, 3*BOX_SIZE, BOX_SIZE, BOX_SIZE);
    
}
