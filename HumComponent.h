/*
  ==============================================================================

    HumComponent.h
    Created: 24 May 2020 6:09:48pm
    Author:  Thomas Royal

  ==============================================================================
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
