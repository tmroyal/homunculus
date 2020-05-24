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
        formantManager(fmgr)
{
    setSize (FORMANT_EDITOR_LEFT+BOX_SIZE*4.3-80, 480);
    
    setLookAndFeel(&lookAndFeel);
    lookAndFeel.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);


    setupLabels();
    
    addAndMakeVisible(kbComponent);
    
    setupFormantSliders();
    setupEnvelopeSliders();
    setupLFOSliders();
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
    
    setupLabel(freqLabel,"F");
    setupLabel(QLabel, "Q");
    setupLabel(gainLabel, "Gain");
    setupLabel(f1Label, "F1");
    setupLabel(f2Label, "F2");
    setupLabel(f3Label, "F3");
    setupLabel(envALabel, "A");
    setupLabel(envDLabel, "D");
    setupLabel(envSLabel, "S");
    setupLabel(envRLabel, "R");
    setupLabel(selectFormantLabel, "Edit Sel.");
    selectFormantLabel.setVisible(false);
    setupLabel(interpolateFormantLabel, "Morph");
    setupLabel(lfoRateLabel, "LFO Hz.");
    setupLabel(lfoAmountLabel, "LFO %");
    setupLabel(interpolateButtonLabel, "Edit");
    interpolateButtonLabel.setFont(Font(12.0));
}

void HomunculusAudioProcessorEditor::setupLabel(Label& label, String labelText){
    addAndMakeVisible(label);
    label.setText(labelText, dontSendNotification);
    label.setJustificationType(Justification::centred);
}

void HomunculusAudioProcessorEditor::setupFormantSliders(){
    for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
        std::string num = std::to_string(i+1);
        
        // makeFormantSlider
        auto* freqSlider = sliders.add(new Slider());
        auto* QSlider = sliders.add(new Slider());
        auto* gainSlider = sliders.add(new Slider());
        
        addAndMakeVisible(freqSlider);
        addAndMakeVisible(QSlider);
        addAndMakeVisible(gainSlider);
        
        // callbacks for formant sliders
        freqSlider->onValueChange = [this, i, freqSlider]{
            processor.setFrequency(i, freqSlider->getValue());
            formantManager.setFreq(i, freqSlider->getValue());
        };
        
        QSlider->onValueChange = [this, i, QSlider]{
            processor.setQ(i, QSlider->getValue());
            formantManager.setQ(i, QSlider->getValue());
        };
        
        gainSlider->onValueChange = [this, i, gainSlider]{
            processor.setGain(i, gainSlider->getValue());
            formantManager.setGain(i, gainSlider->getValue());
        };
        
        // range and enablement
        freqSlider->setNormalisableRange(
                    NormalisableRange<double>(20,20000,0,0.4));
        freqSlider->setEnabled(false);
        freqSlider->setSliderStyle(Slider::SliderStyle::Rotary);
        freqSlider->setTextBoxStyle(Slider::TextBoxBelow, false, BOX_SIZE, BOX_SIZE/6);
        freqSlider->setNumDecimalPlacesToDisplay(2);
        
        QSlider->setNormalisableRange(
                    NormalisableRange<double>(0.5,20,0,0.8));
        QSlider->setEnabled(false);
        QSlider->setSliderStyle(Slider::SliderStyle::Rotary);
        QSlider->setTextBoxStyle(Slider::TextBoxBelow, false, BOX_SIZE, BOX_SIZE/6);
        QSlider->setNumDecimalPlacesToDisplay(2);
        
        gainSlider->setNormalisableRange(
                    NormalisableRange<double>(0.0,1.0,0,0.7));
        gainSlider->setEnabled(false);
        gainSlider->setSliderStyle(Slider::SliderStyle::Rotary);
        gainSlider->setTextBoxStyle(Slider::TextBoxBelow, false, BOX_SIZE, BOX_SIZE/6);
        gainSlider->setNumDecimalPlacesToDisplay(2);
    }
}

void HomunculusAudioProcessorEditor::setupEnvelopeSliders(){
    setupAttachedSlider(attackSlider, attackAttachment, "attack");
    setupAttachedSlider(decaySlider, decayAttachment, "decay");
    setupAttachedSlider(sustainSlider, sustainAttachment, "sustain");
    setupAttachedSlider(releaseSlider, releaseAttachment, "release");
    
    attackSlider.setNumDecimalPlacesToDisplay(3);
    decaySlider.setNumDecimalPlacesToDisplay(3);
    sustainSlider.setNumDecimalPlacesToDisplay(3);
    releaseSlider.setNumDecimalPlacesToDisplay(3);
    
}

void HomunculusAudioProcessorEditor::setupLFOSliders(){
    setupAttachedSlider(lfoFreqSlider, lfoFreqSliderAttachment, "lfoFreq");
    setupAttachedSlider(lfoAmountSlider, lfoAmountSliderAttachment, "lfoAmount");
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
        
        if (toggleState){
            syncFormantManager();
        } else {
            processor.resetInterpolator();
        }
        
        for (auto i = sliders.begin(); i != sliders.end(); i++){
            (*i)->setEnabled(toggleState);
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
    
    // set formants sliders to formant values
    for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
        sliders[i*3]->setValue(
            formantManager.getCurrentFormantSet().getFormant(i).freq);
        sliders[i*3+1]->setValue(
                formantManager.getCurrentFormantSet().getFormant(i).Q);
        sliders[i*3+2]->setValue(
                formantManager.getCurrentFormantSet().getFormant(i).gain);
    }
}


//==============================================================================
void HomunculusAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    // draw divider lines
    
    // between formants and adsr
    g.drawLine(ADSR_LEFT-LABEL_SIZE*2, 10, ADSR_LEFT-LABEL_SIZE*2, getHeight()-90);
    
    // between adsr and formant chooser panel
    g.drawLine(FORMANT_EDITOR_LEFT, 10, FORMANT_EDITOR_LEFT, getHeight()-90);
    
    // between chooser and title box
    g.drawLine(TITLE_LEFT+BOX_SIZE*1/3, 10, TITLE_LEFT+BOX_SIZE*1/3, getHeight()-90);
    
    // between flo and formants
    g.drawLine(LFO_LEFT, FORMANTS_TOP-LABEL_SIZE*0.25, LFO_LEFT+2*BOX_SIZE, FORMANTS_TOP-LABEL_SIZE*0.25);
    
    //formVis.paint(g);

}

void HomunculusAudioProcessorEditor::resized()
{
    resizeFormantControls();
    resizeADSRControls();
    resizeFormantEditors();
    resizeLFOControls();
    
    titleLabel.setBounds(TITLE_LEFT,0,BOX_SIZE*2, getHeight()-80);

    kbComponent.setBounds(0, getHeight()-80, getWidth(), 80);
    
    formVis.setBounds(FORMANTS_LEFT-LABEL_SIZE, FORMANTS_TOP+BOX_SIZE*0.5, BOX_SIZE*3, BOX_SIZE*2);
    
}

void HomunculusAudioProcessorEditor::resizeFormantControls(){
    auto it = sliders.begin();
    auto end = sliders.end();
    auto i = 0;
    
    f1Label.setBounds(FORMANTS_LEFT, FORMANTS_TOP, BOX_SIZE, LABEL_SIZE);
    f2Label.setBounds(FORMANTS_LEFT+BOX_SIZE, FORMANTS_TOP, BOX_SIZE, LABEL_SIZE);
    f3Label.setBounds(FORMANTS_LEFT+BOX_SIZE*2, FORMANTS_TOP, BOX_SIZE, LABEL_SIZE);
    
    freqLabel.setBounds(FORMANTS_LEFT-LABEL_SIZE*2.5, FORMANTS_TOP+LABEL_SIZE, BOX_SIZE, BOX_SIZE);
    QLabel.setBounds(FORMANTS_LEFT-LABEL_SIZE*2.5, FORMANTS_TOP+LABEL_SIZE+BOX_SIZE, BOX_SIZE, BOX_SIZE);
    gainLabel.setBounds(FORMANTS_LEFT-LABEL_SIZE*2.5, FORMANTS_TOP+LABEL_SIZE+BOX_SIZE*2, BOX_SIZE, BOX_SIZE);
    
    while (it != end){
        (**it).setBounds((i/3)*BOX_SIZE+FORMANTS_LEFT, (i%3)*BOX_SIZE+FORMANTS_TOP+LABEL_SIZE, BOX_SIZE, BOX_SIZE);
        it++; i++;
    }
}

void HomunculusAudioProcessorEditor::resizeADSRControls(){
        envALabel.setBounds(ADSR_LEFT-LABEL_SIZE*2.5, ADSR_TOP, BOX_SIZE, BOX_SIZE);
    envDLabel.setBounds(ADSR_LEFT-LABEL_SIZE*2.5, ADSR_TOP+BOX_SIZE, BOX_SIZE, BOX_SIZE);
    envSLabel.setBounds(ADSR_LEFT-LABEL_SIZE*2.5, ADSR_TOP+BOX_SIZE*2, BOX_SIZE, BOX_SIZE);
    envRLabel.setBounds(ADSR_LEFT-LABEL_SIZE*2.5, ADSR_TOP+BOX_SIZE*3, BOX_SIZE, BOX_SIZE);
    
    
    attackSlider.setBounds(ADSR_LEFT, ADSR_TOP, BOX_SIZE, BOX_SIZE);
    decaySlider.setBounds(ADSR_LEFT, BOX_SIZE+ADSR_TOP, BOX_SIZE, BOX_SIZE);
    sustainSlider.setBounds(ADSR_LEFT, 2*BOX_SIZE+ADSR_TOP, BOX_SIZE, BOX_SIZE);
    releaseSlider.setBounds(ADSR_LEFT, 3*BOX_SIZE+ADSR_TOP, BOX_SIZE, BOX_SIZE);

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


void HomunculusAudioProcessorEditor::resizeLFOControls(){
    lfoRateLabel.setBounds(LFO_LEFT, LFO_TOP, BOX_SIZE, LABEL_SIZE);
    lfoFreqSlider.setBounds(LFO_LEFT, LFO_TOP+LABEL_SIZE, BOX_SIZE, BOX_SIZE);
    
    lfoAmountLabel.setBounds(LFO_LEFT+BOX_SIZE, LFO_TOP, BOX_SIZE, LABEL_SIZE);
    lfoAmountSlider.setBounds(LFO_LEFT+BOX_SIZE, LFO_TOP+LABEL_SIZE, BOX_SIZE, BOX_SIZE);
    
}
