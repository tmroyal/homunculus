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
