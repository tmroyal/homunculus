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
        filterPanel(p, ps, lookAndFeel, fmgr),
        formantPanel(p, ps, lookAndFeel, fmgr)

{
    setSize (FORMANT_EDITOR_LEFT+BOX_SIZE*4.3-80, 480);
    
    setLookAndFeel(&lookAndFeel);

    addAndMakeVisible(titleLabel);
    // the word humunculus
    titleLabel.setText("H\nu\nm\nu\nn\nc\nu\nl\nu\ns\nFormant\nSynth", dontSendNotification);
    titleLabel.setJustificationType(Justification::centred);
    titleLabel.setFont(Font("Futura", 28.0, Font::plain));

    addAndMakeVisible(lfoPanel);
    addAndMakeVisible(adsrPanel);
    addAndMakeVisible(filterPanel);
    addAndMakeVisible(formantPanel);
    
    filterPanel.setVisible(false);
    filterPanel.setEnabled(false);
    
    addAndMakeVisible(kbComponent);
    
    syncFormantManager();

    addAndMakeVisible(formVis);
    formVis.refreshData();
    
    setupFormantPanelLambdas();
    
    resized();
}

HomunculusAudioProcessorEditor::~HomunculusAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}
// -------------


void HomunculusAudioProcessorEditor::setupFormantPanelLambdas(){
    formantPanel.onInterpolatorSlider = [this]{
        formVis.refreshData();
        formVis.repaint();
    };
    
    formantPanel.onEditToggle = [this](bool toggleState){
        formVis.setVisible(!toggleState);
        formVis.setEnabled(!toggleState);
        
        filterPanel.setVisible(toggleState);
        filterPanel.setEnabled(toggleState);
        
        if (toggleState){
            syncFormantManager();
        } else {
            formVis.refreshData();
            formVis.repaint();
            processor.resetInterpolator();
        }
    };
    
    std::function<void()> fmgrSync = [this]{ syncFormantManager(); };
    
    formantPanel.onEditSlider = fmgrSync; //[this]{ syncFormantManager(); };
    formantPanel.onFormantAdd = fmgrSync;
    formantPanel.onFormantRemove = fmgrSync;
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
    
    // update components with new formant manager data
    filterPanel.syncFormantManager();
    formantPanel.syncFormantManager();

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
    g.drawLine(TITLE_LEFT, 10, TITLE_LEFT, getHeight()-90);
    
    // between flo and formants
    g.drawLine(LFO_LEFT, FORMANTS_TOP-LABEL_SIZE*0.25, LFO_LEFT+2*BOX_SIZE, FORMANTS_TOP-LABEL_SIZE*0.25);
}

void HomunculusAudioProcessorEditor::resized()
{
    lfoPanel.setBounds(LFO_LEFT, LFO_TOP, BOX_SIZE*2, BOX_SIZE+LABEL_SIZE);
    adsrPanel.setBounds(ADSR_LEFT, ADSR_TOP, BOX_SIZE*2, getHeight());
    filterPanel.setBounds(FORMANTS_LEFT, FORMANTS_TOP, BOX_SIZE*3+LABEL_SIZE*2, BOX_SIZE*3+LABEL_SIZE);
    formantPanel.setBounds(FORMANT_EDITOR_LEFT, FORMANT_EDITOR_TOP, BOX_SIZE*3, getHeight()-LABEL_SIZE-80);
    
    titleLabel.setBounds(TITLE_LEFT,0, BOX_SIZE*1.8, getHeight()-80);

    kbComponent.setBounds(0, getHeight()-80, getWidth(), 80);
    
    formVis.setBounds(FORMANTS_LEFT+LABEL_SIZE*2, FORMANTS_TOP+LABEL_SIZE, BOX_SIZE*3, BOX_SIZE*3);
}
