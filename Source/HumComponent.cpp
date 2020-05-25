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


#include "HumComponent.h"


HumComponent::HumComponent(HomunculusAudioProcessor& proc, AudioProcessorValueTreeState& ps, HumLook& lf):
processor(proc),
params(ps),
lookAndFeel(lf)
{
    setLookAndFeel(&lf);
}

HumComponent::~HumComponent()
{
    setLookAndFeel(nullptr);
}

void HumComponent::setupLabel(Label& label, String labelText){
    addAndMakeVisible(label);
    label.setText(labelText, dontSendNotification);
    label.setJustificationType(Justification::centred);
}

void HumComponent::setupAttachedSlider(Slider& slider, unique_ptr<SliderAttachment>& attachment, String parameterName){
    
    addAndMakeVisible(slider);
    attachment.reset(new SliderAttachment(params, parameterName, slider));
    slider.setSliderStyle(Slider::Rotary);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, BOX_SIZE, BOX_SIZE/6);
    slider.setNumDecimalPlacesToDisplay(2);
}
