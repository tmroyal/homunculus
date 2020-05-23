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
        kbComponent(p.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
        processor (p),
        params(ps),
        formantManager(fmgr)
{
    // TODO: break constructor apart
    
    setSize (400, 650);
    
    addAndMakeVisible(kbComponent);
    
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
        freqSlider->setEnabled(false);
        
        QSlider->setNormalisableRange(NormalisableRange<double>(0.5,20,0,0.8));
        QSlider->setEnabled(false);
        
        gainSlider->setNormalisableRange(NormalisableRange<double>(0.0,1.0,0,0.7));
        gainSlider->setEnabled(false);
    }
    
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);
    
    formantInterpolatorSliderAttachment.reset(
                            new SliderAttachment(params, "interpolate", formantInterpolatorSlider));
    
    attackAttachment.reset(new SliderAttachment(params, "attack", attackSlider));
    decayAttachment.reset(new SliderAttachment(params, "decay", decaySlider));
    sustainAttachment.reset(new SliderAttachment(params, "sustain", sustainSlider));
    releaseAttachment.reset(new SliderAttachment(params, "release", releaseSlider));

    addAndMakeVisible(lfoFreqSlider);
    addAndMakeVisible(lfoAmountSlider);
    
    lfoFreqSliderAttachment.reset(new SliderAttachment(params, "lfoFreq", lfoFreqSlider));
    lfoAmountSliderAttachment.reset(new SliderAttachment(params, "lfoAmount", lfoAmountSlider));
    
    addAndMakeVisible(editModeButton);
    
    addAndMakeVisible(formantEditorSlider);
    
    addAndMakeVisible(formantInterpolatorSlider);
    
    formantInterpolatorSlider.setEnabled(true);
    formantEditorSlider.setEnabled(false);
    
    editModeButton.onClick = [this]{
        
        bool toggleState = editModeButton.getToggleState();
        
        formantInterpolatorSlider.setEnabled(!toggleState);
        formantEditorSlider.setEnabled(toggleState);
        
        for (auto i = sliders.begin(); i != sliders.end(); i++){
            (*i)->setEnabled(toggleState);
        }
    };
    
    formantEditorSlider.setRange(0,processor.getNumFormantSets()-1,1);
    formantEditorSlider.setValue(processor.getCurrentFormantSetId());
    
    formantEditorSlider.onValueChange = [this]{
         formantManager.setCurrentFormantSet((int)formantEditorSlider.getValue());
        
         syncFormantManager();
    };
    
    addAndMakeVisible(addFormantButton);
    addFormantButton.setButtonText("+");
    addFormantButton.onClick = [this]{
        formantManager.addFormant();
        
        syncFormantManager();
    };
    
    addAndMakeVisible(removeFormantButton);
    removeFormantButton.setButtonText("-");
    removeFormantButton.setEnabled(false);
    
    removeFormantButton.onClick = [this]{
        formantManager.removeFormant();

        syncFormantManager();

    };
    
    syncFormantManager();
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
    
    editModeButton.setBounds(10, envTop+120, (getWidth()-10)/3, 20);
    addFormantButton.setBounds(getWidth()/3, envTop+120, (getWidth()-10)/3, 20);
    removeFormantButton.setBounds(getWidth()*2/3, envTop+120, (getWidth()-10)/3, 20);
    
    formantEditorSlider.setBounds(10, envTop+150, getWidth()-10, 20);
    formantInterpolatorSlider.setBounds(10, envTop+180, getWidth()-10, 20);
    
    lfoFreqSlider.setBounds(10, envTop+210, getWidth()-10, 20);
    lfoAmountSlider.setBounds(10, envTop+240, getWidth()-10, 20);
    
    kbComponent.setBounds(0, getHeight()-80, getWidth(), 80);
    
}

void HomunculusAudioProcessorEditor::syncFormantManager(){
    
    FormantSet currentSet = formantManager.getCurrentFormantSet();
    // update audio components
    for (int i = 0; i < NUMBER_OF_FORMANTS ; i++){
        Formant fmt = currentSet.getFormant(i);
        
        processor.setFrequency(i, fmt.freq);
        processor.setQ(i, fmt.Q);
        processor.setGain(i, fmt.gain);
    }
    // update ui components
    formantEditorSlider.setRange(0,formantManager.getNumberOfFormantSets()-1,1.0);
    formantEditorSlider.repaint();
    
    formantEditorSlider.setValue((double)formantManager.getCurrentFormantSetId());
    
    removeFormantButton.setEnabled(formantManager.getNumberOfFormantSets() > 2);
    
    for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
        sliders[i*3]->setValue(formantManager.getCurrentFormantSet().getFormant(i).freq);
        sliders[i*3+1]->setValue(formantManager.getCurrentFormantSet().getFormant(i).Q);
        sliders[i*3+2]->setValue(formantManager.getCurrentFormantSet().getFormant(i).gain);
    }
}
