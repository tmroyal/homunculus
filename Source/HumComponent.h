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

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "HumLook.h"

class HumComponent : public Component {
public:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    HumComponent(HomunculusAudioProcessor& proc, AudioProcessorValueTreeState& ps, HumLook& lf);
    
    ~HumComponent();
    
    void setupLabel(Label& label, String labelText);
    
    void setupAttachedSlider(Slider& slider, unique_ptr<SliderAttachment>& attachment, String parameterName);

    void resized() override {}

protected:
    HomunculusAudioProcessor& processor;
    AudioProcessorValueTreeState& params;
    HumLook& lookAndFeel;

};
