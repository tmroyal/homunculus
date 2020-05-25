/*
  ==============================================================================

    Juce formant synth plugin editor

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HomunculusAudioProcessorEditor::HomunculusAudioProcessorEditor (HomunculusAudioProcessor& p, AudioProcessorValueTreeState& ps, FormantManager& fmgr)
    : AudioProcessorEditor (&p),
        formVis(p),
        kbComponent(p.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
        processor (p),
        params(ps),
        formantManager(fmgr),
        lfoPanel(p, ps, lookAndFeel),
        adsrPanel(p, ps, lookAndFeel),
        filterPanel(p, ps, lookAndFeel, fmgr)

{
    setSize (FORMANT_EDITOR_LEFT+BOX_SIZE*4.3-80, 480);
    
    setLookAndFeel(&lookAndFeel);

    addAndMakeVisible(lfoPanel);
    addAndMakeVisible(adsrPanel);
    addAndMakeVisible(filterPanel);
    
    filterPanel.setVisible(false);
    filterPanel.setEnabled(false);

    setupLabels();
    
    addAndMakeVisible(kbComponent);
    
    setupFormantUI();
    
    syncFormantManager();

    addAndMakeVisible(formVis);
    formVis.refreshData();
    
    resized();
}

HomunculusAudioProcessorEditor::~HomunculusAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}
// -------------

void HomunculusAudioProcessorEditor::setupLabels(){
    setupLabel(titleLabel, "H\nu\nm\nu\nn\nc\nl\no\nu\ns\nFormant\nSynth");
    titleLabel.setFont(Font("Futura", 28.0, Font::plain));


    setupLabel(selectFormantLabel, "Edit Sel.");
    selectFormantLabel.setVisible(false);
    setupLabel(interpolateFormantLabel, "Morph");
    setupLabel(interpolateButtonLabel, "Edit");
    interpolateButtonLabel.setFont(Font(12.0));
}

void HomunculusAudioProcessorEditor::setupLabel(Label& label, String labelText){
    addAndMakeVisible(label);
    label.setText(labelText, dontSendNotification);
    label.setJustificationType(Justification::centred);
}


void HomunculusAudioProcessorEditor::setupAttachedSlider(Slider& slider, unique_ptr<SliderAttachment>& attachment, String parameterName){
    
    addAndMakeVisible(slider);
    attachment.reset(new SliderAttachment(params, parameterName, slider));
    slider.setSliderStyle(Slider::Rotary);
    slider.setTextBoxStyle(Slider::TextBoxBelow, false, BOX_SIZE, BOX_SIZE/6);
    slider.setNumDecimalPlacesToDisplay(2);
}

void HomunculusAudioProcessorEditor::setupFormantUI(){
    setupAttachedSlider(formantInterpolatorSlider, formantInterpolatorSliderAttachment, "interpolate");
    formantInterpolatorSlider.setSliderStyle(Slider::LinearVertical);
    formantInterpolatorSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, LABEL_SIZE);
    formantInterpolatorSlider.onValueChange = [this]{
        formVis.refreshData();
        formVis.repaint();
    };
    
    // setup edit mode button
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
        
        formVis.setVisible(!toggleState);
        formVis.setEnabled(!toggleState);
        
        filterPanel.setVisible(toggleState);
        filterPanel.setEnabled(toggleState);

        
        if (toggleState){
            syncFormantManager();
        } else {
            processor.resetInterpolator();
        }
        

    };
    
    
    // setup formant editor slider
    addAndMakeVisible(formantEditorSlider);
    formantEditorSlider.setEnabled(false);
    formantEditorSlider.setVisible(false);
    formantEditorSlider.onValueChange = [this]{
        formantManager.setCurrentFormantSet(
                (int)formantEditorSlider.getValue());
        
        syncFormantManager();
    };
    formantEditorSlider.setSliderStyle(Slider::LinearVertical);
    formantEditorSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 80, LABEL_SIZE);
    
    // setup add formant button
    addAndMakeVisible(addFormantButton);
    addFormantButton.setButtonText("+");
    addFormantButton.setEnabled(false);
    addFormantButton.setVisible(false);

    addFormantButton.onClick = [this]{
        formantManager.addFormant();
        
        syncFormantManager();
    };
    
    // setup removeFormant button
    addAndMakeVisible(removeFormantButton);
    removeFormantButton.setButtonText("-");
    removeFormantButton.setEnabled(false);
    removeFormantButton.setVisible(false);

    removeFormantButton.onClick = [this]{
        formantManager.removeFormant();
        
        syncFormantManager();
    };

}

void HomunculusAudioProcessorEditor::syncFormantManager(){
    
    FormantSet currentSet = formantManager.getCurrentFormantSet();
    
    // set filter frequencies according to formants
    for (int i = 0; i < NUMBER_OF_FORMANTS ; i++){
        Formant fmt = currentSet.getFormant(i);
        
        processor.setFrequency(i, fmt.freq);
        processor.setQ(i, fmt.Q);
        processor.setGain(i, fmt.gain);
    }
    
    // update ui components
    
    formantEditorSlider.setRange(
            0,formantManager.getNumberOfFormantSets()-1,1.0);
    formantEditorSlider.setValue(
            (double)formantManager.getCurrentFormantSetId());
    
    formantEditorSlider.repaint();
    
    
    // remove formants allowed only when number of formantSets > 2
    removeFormantButton.setEnabled(
            formantManager.getNumberOfFormantSets() > 2);
    
    filterPanel.syncFormantManager();
}


//==============================================================================
void HomunculusAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    // draw divider lines
    
    // between formants and adsr
    g.drawLine(ADSR_LEFT+LABEL_SIZE*0.5, 10, ADSR_LEFT+LABEL_SIZE*0.5, getHeight()-90);
    
    // between adsr and formant chooser panel
    g.drawLine(FORMANT_EDITOR_LEFT, 10, FORMANT_EDITOR_LEFT, getHeight()-90);
    
    // between chooser and title box
    g.drawLine(TITLE_LEFT+BOX_SIZE*1/3, 10, TITLE_LEFT+BOX_SIZE*1/3, getHeight()-90);
    
    // between flo and formants
    g.drawLine(LFO_LEFT, FORMANTS_TOP-LABEL_SIZE*0.25, LFO_LEFT+2*BOX_SIZE, FORMANTS_TOP-LABEL_SIZE*0.25);
}

void HomunculusAudioProcessorEditor::resized()
{
    resizeFormantControls();
    resizeFormantEditors();

    lfoPanel.setBounds(LFO_LEFT, LFO_TOP, BOX_SIZE*2, BOX_SIZE+LABEL_SIZE);
    adsrPanel.setBounds(ADSR_LEFT, ADSR_TOP, BOX_SIZE*2, getHeight());
    filterPanel.setBounds(FORMANTS_LEFT, FORMANTS_TOP, BOX_SIZE*3+LABEL_SIZE*2, BOX_SIZE*3+LABEL_SIZE);
    
    titleLabel.setBounds(TITLE_LEFT,0,BOX_SIZE*2, getHeight()-80);

    kbComponent.setBounds(0, getHeight()-80, getWidth(), 80);
    
    formVis.setBounds(FORMANTS_LEFT+LABEL_SIZE*2, FORMANTS_TOP+LABEL_SIZE, BOX_SIZE*3, BOX_SIZE*3);
}

void HomunculusAudioProcessorEditor::resizeFormantControls(){
    
}

void HomunculusAudioProcessorEditor::resizeFormantEditors(){
    interpolateFormantLabel.setBounds(FORMANT_EDITOR_LEFT, FORMANT_EDITOR_TOP, 80, LABEL_SIZE);
    selectFormantLabel.setBounds(FORMANT_EDITOR_LEFT, FORMANT_EDITOR_TOP, 80, LABEL_SIZE);
    
    formantInterpolatorSlider.setBounds(FORMANT_EDITOR_LEFT, FORMANT_EDITOR_TOP+LABEL_SIZE, 80, getHeight()-LABEL_SIZE-FORMANT_EDITOR_TOP-80);
    formantEditorSlider.setBounds(FORMANT_EDITOR_LEFT, FORMANT_EDITOR_TOP+LABEL_SIZE, 80, getHeight()-LABEL_SIZE-FORMANT_EDITOR_TOP-80);
    
    int buttonHeight = (getHeight()-LABEL_SIZE-FORMANT_EDITOR_TOP-LABEL_SIZE-80)/3;
    
    addFormantButton.setBounds(FORMANT_EDITOR_LEFT+80, FORMANT_EDITOR_TOP+LABEL_SIZE, 40, buttonHeight);
    removeFormantButton.setBounds(FORMANT_EDITOR_LEFT+80, FORMANT_EDITOR_TOP+LABEL_SIZE+buttonHeight, 40, buttonHeight);
    interpolateButtonLabel.setBounds(FORMANT_EDITOR_LEFT+80, FORMANT_EDITOR_TOP+LABEL_SIZE+buttonHeight*2, 40, 24);
    editModeButton.setBounds(FORMANT_EDITOR_LEFT+80, FORMANT_EDITOR_TOP+LABEL_SIZE+buttonHeight*2+24, 40, buttonHeight-24);
}
