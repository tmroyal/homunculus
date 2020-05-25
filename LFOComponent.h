/*
  ==============================================================================

    LFOComponent.h
    Created: 24 May 2020 6:16:23pm
    Author:  Thomas Royal

  ==============================================================================
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
