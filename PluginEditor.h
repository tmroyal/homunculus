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

private:
    HumLook lookAndFeel;
    
    FormantVisualiser formVis;
    
    // labels
    
    Label titleLabel;

    
    Label selectFormantLabel;
    Label interpolateFormantLabel;
    Label interpolateButtonLabel;
    
    // modularized constructors
    void setupFormantUI();
    void setupLabels();
    
    void setupLabel(Label& label, String labelText);
    void setupAttachedSlider(Slider& slider, unique_ptr<SliderAttachment>& attachment, String parameterName);
    
    MidiKeyboardComponent kbComponent;
    
    HomunculusAudioProcessor& processor;
    AudioProcessorValueTreeState& params;
    
    FormantManager& formantManager;
    
    Slider formantEditorSlider;
    Slider formantInterpolatorSlider;
    
    ToggleButton editModeButton;
        
    TextButton addFormantButton;
    TextButton removeFormantButton;
    
    std::unique_ptr<SliderAttachment> lfoFreqSliderAttachment;
    std::unique_ptr<SliderAttachment> lfoAmountSliderAttachment;

    std::unique_ptr<SliderAttachment> formantInterpolatorSliderAttachment;

    
    // resize functions
    void resizeFormantControls();
    void resizeFormantEditors();
   
    LFOComponent lfoPanel;
    ADSRComponent adsrPanel;
    FormantFilterPanel filterPanel;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomunculusAudioProcessorEditor)
};
