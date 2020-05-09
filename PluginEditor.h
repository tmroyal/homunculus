/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "HumConstants.h"
#include "FormantManager.h"
//==============================================================================
/**
*/
class HomunculusAudioProcessorEditor  : public AudioProcessorEditor
{
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

public:
    HomunculusAudioProcessorEditor (HomunculusAudioProcessor&, AudioProcessorValueTreeState&);
    ~HomunculusAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    HomunculusAudioProcessor& processor;
    AudioProcessorValueTreeState& params;
    
    OwnedArray<Slider> sliders;
    
    Slider attackSlider;
    Slider decaySlider;
    Slider sustainSlider;
    Slider releaseSlider;
    
    Slider formantEditorSlider;
    Slider formantInterpolatorSlider;
    ToggleButton editModeButton;
    
    std::unique_ptr<SliderAttachment> formantEditorSliderAttachment;
    std::unique_ptr<SliderAttachment> formantInterpolatorSliderAttachment;
    std::unique_ptr<ButtonAttachment> editModeButtonAttachment;

    
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomunculusAudioProcessorEditor)
};
