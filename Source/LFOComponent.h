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

#include "HumComponent.h"
#include "PluginProcessor.h"
#include "HumLook.h"
#include "HumConstants.h"

class LFOComponent : public HumComponent{
public:
    LFOComponent(HomunculusAudioProcessor& proc, AudioProcessorValueTreeState& ps, HumLook& lf);
    
    ~LFOComponent();
    
    void resized() override;
    
private:
    Label lfoRateLabel;
    Label lfoAmountLabel;
    
    Slider lfoFreqSlider;
    Slider lfoAmountSlider;
    
    std::unique_ptr<SliderAttachment> lfoFreqSliderAttachment;
    std::unique_ptr<SliderAttachment> lfoAmountSliderAttachment;
};
