/*
  ==============================================================================

    ADSRPanel.h
    Created: 24 May 2020 7:22:58pm
    Author:  Thomas Royal

  ==============================================================================
*/

#pragma once

#include "HumComponent.h"
#include <JuceHeader.h>

class ADSRComponent : public HumComponent {
public:
    ADSRComponent(HomunculusAudioProcessor& proc, AudioProcessorValueTreeState& ps, HumLook& lf);

    ~ADSRComponent();
    
    void resized() override;

private:
    Label envALabel;
    Label envDLabel;
    Label envSLabel;
    Label envRLabel;
    
    Slider attackSlider;
    Slider decaySlider;
    Slider sustainSlider;
    Slider releaseSlider;
    
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
};
