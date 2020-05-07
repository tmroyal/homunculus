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
//==============================================================================
/**
*/
class HomunculusAudioProcessorEditor  : public AudioProcessorEditor
{
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    
public:
    HomunculusAudioProcessorEditor (HomunculusAudioProcessor&, AudioProcessorValueTreeState&);
    ~HomunculusAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HomunculusAudioProcessor& processor;
    AudioProcessorValueTreeState& params;
    
    OwnedArray<Slider> sliders;
    OwnedArray<SliderAttachment> attachments;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomunculusAudioProcessorEditor)
};
