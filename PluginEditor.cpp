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
    setSize (400, 300);
    
    addAndMakeVisible(f1Freq);
    f1FreqAttachment.reset(new SliderAttachment(params, "f1freq", f1Freq));
    addAndMakeVisible(f2Freq);
    f2FreqAttachment.reset(new SliderAttachment(params, "f2freq", f2Freq));
    
    ps.addParameterListener("f1freq", this);
    ps.addParameterListener("f2freq", this);
}

HomunculusAudioProcessorEditor::~HomunculusAudioProcessorEditor()
{
}
// -------------
void HomunculusAudioProcessorEditor::parameterChanged (const String& parameterID, float newValue) {
    std::cout << parameterID << "\n";
}

//==============================================================================
void HomunculusAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void HomunculusAudioProcessorEditor::resized()
{
    f1Freq.setBounds(10, 10, getWidth(), 30);
    f2Freq.setBounds(10, 50, getWidth(), 30);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
