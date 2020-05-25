/*
 Homunculus - formant synth written in JUCE
 Copyright (C) 2020 Thomas Royal
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 
 Source is available at: <https://github.com/tmroyal/homunculus>
 */

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "HumConstants.h"
#include "FormantManager.h"
#include "HumLook.h"

#include "FormantVisualiser.h"
#include "LFOComponent.h"
#include "ADSRComponent.h"
#include "FormantFilterPanel.h"
#include "FormantSelectorPanel.h"

//==============================================================================
/**
*/
class HomunculusAudioProcessorEditor  : public AudioProcessorEditor
{
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

public:
    HomunculusAudioProcessorEditor (HomunculusAudioProcessor&, AudioProcessorValueTreeState&, FormantManager&);
    ~HomunculusAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void syncFormantManager();
    void updateVisualiser();

private:
    void setupFormantPanelLambdas();
    
    HumLook lookAndFeel;
    
    FormantVisualiser formVis;
    
    Label titleLabel;
    
    MidiKeyboardComponent kbComponent;
    
    HomunculusAudioProcessor& processor;
    AudioProcessorValueTreeState& params;
    
    FormantManager& formantManager;
    
    LFOComponent lfoPanel;
    ADSRComponent adsrPanel;
    FormantFilterPanel filterPanel;
    FormantSelectorPanel formantPanel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomunculusAudioProcessorEditor)
};
