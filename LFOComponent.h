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
    LFOComponent(HomunculusAudioProcessor& proc, AudioProcessorValueTreeState& ps, HumLook& lf)
        : HumComponent(proc, ps, lf)
    {
        setupLabel(lfoRateLabel, "LFO Hz.");
        setupLabel(lfoAmountLabel, "LFO %");
                
        setupAttachedSlider(lfoFreqSlider, lfoFreqSliderAttachment, "lfoFreq");
        setupAttachedSlider(lfoAmountSlider, lfoAmountSliderAttachment, "lfoAmount");
    }
    
    ~LFOComponent(){};
    
    void resized() override{
        lfoRateLabel.setBounds(0, 0, BOX_SIZE, LABEL_SIZE);
        lfoFreqSlider.setBounds(0, LABEL_SIZE, BOX_SIZE, BOX_SIZE);
        
        lfoAmountLabel.setBounds(BOX_SIZE, 0, BOX_SIZE, LABEL_SIZE);
        lfoAmountSlider.setBounds(BOX_SIZE, LABEL_SIZE, BOX_SIZE, BOX_SIZE);
    }
    
private:
    Label lfoRateLabel;
    Label lfoAmountLabel;
    
    Slider lfoFreqSlider;
    Slider lfoAmountSlider;
    
    std::unique_ptr<SliderAttachment> lfoFreqSliderAttachment;
    std::unique_ptr<SliderAttachment> lfoAmountSliderAttachment;
};
