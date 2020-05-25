/*
  ==============================================================================

    Juce formant sytnh plugin editor | header

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "HumConstants.h"
#include "FormantManager.h"
#include "HumLook.h"

#include "FormantVisualiser.h"
#include "LFOComponent.h"
#include "ADSRComponent.h"
#include "FormantFilterPanel.h"
#include "FormantSelectorPanel.h"

//==============================================================================
/**
*/
class HomunculusAudioProcessorEditor  : public AudioProcessorEditor
{
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

public:
    HomunculusAudioProcessorEditor (HomunculusAudioProcessor&, AudioProcessorValueTreeState&, FormantManager&);
    ~HomunculusAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void syncFormantManager();
    void updateVisualiser();

private:
    void setupFormantPanelLambdas();
    
    HumLook lookAndFeel;
    
    FormantVisualiser formVis;
    
    Label titleLabel;
    
    MidiKeyboardComponent kbComponent;
    
    HomunculusAudioProcessor& processor;
    AudioProcessorValueTreeState& params;
    
    FormantManager& formantManager;
    
    LFOComponent lfoPanel;
    ADSRComponent adsrPanel;
    FormantFilterPanel filterPanel;
    FormantSelectorPanel formantPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomunculusAudioProcessorEditor)
};
