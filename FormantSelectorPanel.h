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
                         FormantManager& fmgr):
        HumComponent(prc, ps, look),
        formantManager(fmgr)
    {
        setupLabel(selectFormantLabel, "Edit Sel.");
        selectFormantLabel.setVisible(false);
        setupLabel(interpolateFormantLabel, "Morph");
        setupLabel(interpolateButtonLabel, "Edit");
        interpolateButtonLabel.setFont(Font(12.0));
        
        setupSlider();
        setupEditButton();
        setupFormantEditorSlider();
        setupAddButton();
        setupRemoveButton();
    }
    
    ~FormantSelectorPanel(){}

    void setupSlider(){
        setupAttachedSlider(formantInterpolatorSlider, formantInterpolatorSliderAttachment, "interpolate");
        formantInterpolatorSlider.setSliderStyle(Slider::LinearVertical);
        formantInterpolatorSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, LABEL_SIZE);
        
        formantInterpolatorSlider.onValueChange = [this]{
            if (onInterpolatorSlider != nullptr){ onInterpolatorSlider(); }
        };
    }
    
    void setupEditButton(){
        addAndMakeVisible(editModeButton);
        
        editModeButton.onClick = [this]{
            bool toggleState = editModeButton.getToggleState();
            // sliders
            formantInterpolatorSlider.setEnabled(!toggleState);
            formantInterpolatorSlider.setVisible(!toggleState);
            
            formantEditorSlider.setEnabled(toggleState);
            formantEditorSlider.setVisible(toggleState);
            
            // buttons
            addFormantButton.setEnabled(toggleState);
            addFormantButton.setVisible(toggleState);
            
            removeFormantButton.setEnabled(toggleState && formantManager.getNumberOfFormantSets() > 2);
            removeFormantButton.setVisible(toggleState);
            
            // labels
            selectFormantLabel.setVisible(toggleState);
            interpolateFormantLabel.setVisible(!toggleState);
            
            if (onEditToggle != nullptr){
                onEditToggle(toggleState);
            }
                
        };
    }
    
    void setupFormantEditorSlider(){
        addAndMakeVisible(formantEditorSlider);
        formantEditorSlider.setEnabled(false);
        formantEditorSlider.setVisible(false);
        formantEditorSlider.onValueChange = [this]{
            formantManager.setCurrentFormantSet((int)formantEditorSlider.getValue());
            if (onEditSlider != nullptr){
                onEditSlider();
            }
        };
        formantEditorSlider.setSliderStyle(Slider::LinearVertical);
        formantEditorSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, LABEL_SIZE);
    }
    
    void setupAddButton(){
        addAndMakeVisible(addFormantButton);
        addFormantButton.setButtonText("+");
        addFormantButton.setEnabled(false);
        addFormantButton.setVisible(false);
        
        addFormantButton.onClick = [this]{
            formantManager.addFormant();
            if (onFormantAdd != nullptr){ onFormantAdd(); }
            
        };
    }
    
    void setupRemoveButton(){
        addAndMakeVisible(removeFormantButton);
        removeFormantButton.setButtonText("-");
        removeFormantButton.setEnabled(false);
        removeFormantButton.setVisible(false);
        
        removeFormantButton.onClick = [this]{
            formantManager.removeFormant();
            
            if (onFormantRemove != nullptr){ onFormantRemove(); }
        };
    }
    
    void syncFormantManager(){
        // will this cause a loop?
        formantEditorSlider.setRange(0,formantManager.getNumberOfFormantSets()-1,1.0);
        formantEditorSlider.setValue((double)formantManager.getCurrentFormantSetId());
        
        formantEditorSlider.repaint();
        
        // remove formants allowed only when number of formantSets > 2
        removeFormantButton.setEnabled(formantManager.getNumberOfFormantSets() > 2);
    }
    
    std::function<void(bool toggleState)> onEditToggle;
    std::function<void()> onFormantAdd;
    std::function<void()> onFormantRemove;
    std::function<void()> onEditSlider;
    std::function<void()> onInterpolatorSlider;

    void resized() override{
        interpolateFormantLabel.setBounds(0, 0, 80, LABEL_SIZE);
        selectFormantLabel.setBounds(0, 0, 80, LABEL_SIZE);
        
        formantInterpolatorSlider.setBounds(0, LABEL_SIZE, 80, getHeight()-LABEL_SIZE);
        formantEditorSlider.setBounds(0, LABEL_SIZE, 80, getHeight()-LABEL_SIZE);
        
        int buttonHeight = (getHeight()-LABEL_SIZE)/3;
        
        addFormantButton.setBounds(80, LABEL_SIZE, 40, buttonHeight);
        removeFormantButton.setBounds(80, LABEL_SIZE+buttonHeight, 40, buttonHeight);
        interpolateButtonLabel.setBounds(80, LABEL_SIZE+buttonHeight*2, 40, 24);
        editModeButton.setBounds(80, LABEL_SIZE+buttonHeight*2+24, 40, buttonHeight-24);
    }
    
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
