/*
  ==============================================================================

    HumComponent.cpp
    Created: 24 May 2020 6:09:48pm
    Author:  Thomas Royal

  ==============================================================================
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
