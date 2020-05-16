/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HomunculusAudioProcessorEditor::HomunculusAudioProcessorEditor (HomunculusAudioProcessor& p, AudioProcessorValueTreeState& ps, FormantManager& fmgr)
    : AudioProcessorEditor (&p), processor (p), params(ps), formantManager(fmgr)
{
    // TODO: break constructor apart
    
    setSize (400, 600);
    
    for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
        std::string num = std::to_string(i+1);
        
        auto* freqSlider = sliders.add(new Slider());
        auto* QSlider = sliders.add(new Slider());
        auto* gainSlider = sliders.add(new Slider());
        
        addAndMakeVisible(freqSlider);
        addAndMakeVisible(QSlider);
        addAndMakeVisible(gainSlider);
        
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
        
        freqSlider->setNormalisableRange(NormalisableRange<double>(20,20000,0,0.4));
        freqSlider->setEnabled(!processor.getEditMode());
        
        QSlider->setNormalisableRange(NormalisableRange<double>(0.5,20,0,0.8));
        QSlider->setEnabled(!processor.getEditMode());
        
        gainSlider->setNormalisableRange(NormalisableRange<double>(0.0,1.0,0,0.7));
        gainSlider->setEnabled(!processor.getEditMode());
    }
    
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);
    
    attackAttachment.reset(new SliderAttachment(params, "attack", attackSlider));
    decayAttachment.reset(new SliderAttachment(params, "decay", decaySlider));
    sustainAttachment.reset(new SliderAttachment(params, "sustain", sustainSlider));
    releaseAttachment.reset(new SliderAttachment(params, "release", releaseSlider));

    addAndMakeVisible(editModeButton);
    
    editModeButtonAttachment.reset(new ButtonAttachment(params, "editMode", editModeButton));

    addAndMakeVisible(formantEditorSlider);
    addAndMakeVisible(formantInterpolatorSlider);
    
    formantInterpolatorSlider.setEnabled(processor.getEditMode());
    formantEditorSlider.setEnabled(!processor.getEditMode());
    
    editModeButton.onClick = [this]{
        formantInterpolatorSlider.setEnabled(processor.getEditMode());
        formantEditorSlider.setEnabled(!processor.getEditMode());
        
        for (auto i = sliders.begin(); i != sliders.end(); i++){
            (*i)->setEnabled(!processor.getEditMode());
        }
    };
    
    formantInterpolatorSlider.setRange(0,processor.getNumFormantSets()-1);
    formantEditorSlider.setRange(0,processor.getNumFormantSets()-1,1);
    formantEditorSlider.setValue(processor.getCurrentFormantSetId());
    
    formantEditorSlider.onValueChange = [this]{
         formantManager.setCurrentFormantSet((int)formantEditorSlider.getValue());
         FormantSet currentSet = formantManager.getCurrentFormantSet();
         
         for (int i = 0; i < NUMBER_OF_FORMANTS ; i++){
             Formant fmt = currentSet.getFormant(i);
             sliders[i*3]->setValue(fmt.freq);
             sliders[i*3+1]->setValue(fmt.Q);
             sliders[i*3+2]->setValue(fmt.gain);

             processor.setFrequency(i, fmt.freq);
             processor.setQ(i, fmt.Q);
             processor.setGain(i, fmt.gain);
         }
         
    };
    
    formantInterpolatorSlider.onValueChange = [this]{
        FormantSet currentSet = formantManager.getInterpolatedFormants(formantInterpolatorSlider.getValue());
        for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
            Formant fmt = currentSet.getFormant(i);
            processor.setFrequency(i, fmt.freq);
            processor.setGain(i, fmt.gain);
            processor.setQ(i, fmt.Q);
        }
    };
    
    addAndMakeVisible(addFormantButton);
    addFormantButton.setButtonText("+");
    addFormantButton.onClick = [this]{
        formantManager.addFormant();
        formantEditorSlider.setRange(0,formantManager.getNumberOfFormantSets()-1,1.0);
        formantEditorSlider.repaint();
        formantEditorSlider.setValue((double)formantManager.getCurrentFormantSetId());
    };
    
     
    resized();
}

HomunculusAudioProcessorEditor::~HomunculusAudioProcessorEditor()
{
}
// -------------


//==============================================================================
void HomunculusAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void HomunculusAudioProcessorEditor::resized()
{
    auto it = sliders.begin();
    auto end = sliders.end();
    auto i = 0;
    
    while (it != end){
        (**it).setBounds(10, 10+i*30, getWidth()-10, 20);
        it++; i++;
    }
    
    auto envTop = sliders.size()*30+10;
    
    attackSlider.setBounds(10, envTop, getWidth()-10, 20);
    decaySlider.setBounds(10, envTop+30, getWidth()-10, 20);
    sustainSlider.setBounds(10, envTop+60, getWidth()-10, 20);
    releaseSlider.setBounds(10, envTop+90, getWidth()-10, 20);
    
    editModeButton.setBounds(10, envTop+120, (getWidth()-10)/2, 20);
    addFormantButton.setBounds(getWidth()/2, envTop+120, (getWidth()-10)/2, 20);
    
    formantEditorSlider.setBounds(10, envTop+150, getWidth()-10, 20);
    formantInterpolatorSlider.setBounds(10, envTop+180, getWidth()-10, 20);
    
}
