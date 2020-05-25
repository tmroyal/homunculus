/*
  ==============================================================================

    FormantSelectorPanel.cpp
    Created: 24 May 2020 5:09:59pm
    Author:  Thomas Royal

  ==============================================================================
*/

#include "FormantSelectorPanel.h"

FormantSelectorPanel::FormantSelectorPanel(
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

FormantSelectorPanel::~FormantSelectorPanel(){}

void FormantSelectorPanel::setupSlider(){
    setupAttachedSlider(formantInterpolatorSlider, formantInterpolatorSliderAttachment, "interpolate");
    formantInterpolatorSlider.setSliderStyle(Slider::LinearVertical);
    formantInterpolatorSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, LABEL_SIZE);
    
    // BUG
    /*formantInterpolatorSlider.onValueChange = [this]{
        if (onInterpolatorSlider != nullptr){ onInterpolatorSlider(); }
    };*/
}

void FormantSelectorPanel::setupEditButton(){
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

void FormantSelectorPanel::setupFormantEditorSlider(){
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

void FormantSelectorPanel::setupAddButton(){
    addAndMakeVisible(addFormantButton);
    addFormantButton.setButtonText("+");
    addFormantButton.setEnabled(false);
    addFormantButton.setVisible(false);
    
    addFormantButton.onClick = [this]{
        formantManager.addFormant();
        if (onFormantAdd != nullptr){ onFormantAdd(); }
        
    };
}

void FormantSelectorPanel::setupRemoveButton(){
    addAndMakeVisible(removeFormantButton);
    removeFormantButton.setButtonText("-");
    removeFormantButton.setEnabled(false);
    removeFormantButton.setVisible(false);
    
    removeFormantButton.onClick = [this]{
        formantManager.removeFormant();
        
        if (onFormantRemove != nullptr){ onFormantRemove(); }
    };
}

void FormantSelectorPanel::syncFormantManager(){
    formantEditorSlider.setRange(0,formantManager.getNumberOfFormantSets()-1,1.0);
    formantEditorSlider.setValue((double)formantManager.getCurrentFormantSetId());
    
    formantEditorSlider.repaint();
    
    // remove formants allowed only when number of formantSets > 2
    removeFormantButton.setEnabled(formantManager.getNumberOfFormantSets() > 2);
}


void FormantSelectorPanel::resized() {
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
