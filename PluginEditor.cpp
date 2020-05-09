/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HomunculusAudioProcessorEditor::HomunculusAudioProcessorEditor (HomunculusAudioProcessor& p, AudioProcessorValueTreeState& ps)
    : AudioProcessorEditor (&p), processor (p), params(ps)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
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
        };
        QSlider->onValueChange = [this, i, QSlider]{
            processor.setQ(i, QSlider->getValue());
        };
        gainSlider->onValueChange = [this, i, gainSlider]{
            processor.setGain(i, gainSlider->getValue());
        };
        
        freqSlider->setEnabled(!processor.getEditMode());
        QSlider->setEnabled(!processor.getEditMode());
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
    
    editModeButton.setBounds(10, envTop+120, getWidth()-10, 20);
    
    formantEditorSlider.setBounds(10, envTop+150, getWidth()-10, 20);
    formantInterpolatorSlider.setBounds(10, envTop+180, getWidth()-10, 20);
    
}
