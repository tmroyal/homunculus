/*
  ==============================================================================

    FormantSelectorPanel.h
    Created: 24 May 2020 5:09:59pm
    Author:  Thomas Royal

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "FormantManager.h"
#include "PluginProcessor.h"
#include "HumComponent.h"
#include "HumLook.h"


class FormantSelectorPanel : public HumComponent {
public:
    FormantSelectorPanel(
                HomunculusAudioProcessor& prc, AudioProcessorValueTreeState& ps, HumLook& look,
                         FormantManager& fmgr);
    ~FormantSelectorPanel();

    void setupSlider();
    
    void setupEditButton();
    
    void setupFormantEditorSlider();
    
    void setupAddButton();
    
    void setupRemoveButton();
    
    void syncFormantManager();
    
  
    void resized() override;
    
    std::function<void(bool toggleState)> onEditToggle;
    std::function<void()> onFormantAdd;
    std::function<void()> onFormantRemove;
    std::function<void()> onEditSlider;
    std::function<void()> onInterpolatorSlider;

    
private:
    Label selectFormantLabel;
    Label interpolateFormantLabel;
    Label interpolateButtonLabel;
    
    FormantManager& formantManager;
    
    Slider formantEditorSlider;
    Slider formantInterpolatorSlider;
    
    ToggleButton editModeButton;
    
    TextButton addFormantButton;
    TextButton removeFormantButton;
    
    std::unique_ptr<SliderAttachment> lfoFreqSliderAttachment;
    std::unique_ptr<SliderAttachment> lfoAmountSliderAttachment;
    std::unique_ptr<SliderAttachment> formantInterpolatorSliderAttachment;

};
