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
