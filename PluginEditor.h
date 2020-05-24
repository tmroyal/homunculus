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
    Label lfoRateLabel;
    Label lfoAmountLabel;
    
    Label f1Label;
    Label f2Label;
    Label f3Label;
    Label freqLabel;
    Label QLabel;
    Label gainLabel;
    
    Label envALabel;
    Label envDLabel;
    Label envSLabel;
    Label envRLabel;
    
    Label selectFormantLabel;
    Label interpolateFormantLabel;
    Label interpolateButtonLabel;
    
    // modularized constructors
    void setupFormantSliders();
    void setupEnvelopeSliders();
    void setupLFOSliders();
    void setupFormantUI();
    void setupLabels();
    
    void setupLabel(Label& label, String labelText);
    void setupAttachedSlider(Slider& slider, unique_ptr<SliderAttachment>& attachment, String parameterName);
    
    MidiKeyboardComponent kbComponent;
    
    HomunculusAudioProcessor& processor;
    AudioProcessorValueTreeState& params;
    
    OwnedArray<Slider> sliders;
    
    FormantManager& formantManager;
    
    Slider attackSlider;
    Slider decaySlider;
    Slider sustainSlider;
    Slider releaseSlider;
    
    Slider formantEditorSlider;
    Slider formantInterpolatorSlider;
    
    Slider lfoFreqSlider;
    Slider lfoAmountSlider;
    
    ToggleButton editModeButton;
        
    TextButton addFormantButton;
    TextButton removeFormantButton;
    
    std::unique_ptr<SliderAttachment> lfoFreqSliderAttachment;
    std::unique_ptr<SliderAttachment> lfoAmountSliderAttachment;

    std::unique_ptr<SliderAttachment> formantInterpolatorSliderAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    
    // resize functions
    void resizeFormantControls();
    void resizeADSRControls();
    void resizeFormantEditors();
    void resizeLFOControls();
    LFOComponent lfoPanel;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomunculusAudioProcessorEditor)
};
