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
#include "FormantManager.h"
#include "PluginProcessor.h"
#include "HumComponent.h"
#include "HumLook.h"


class FormantSelectorPanel : public HumComponent {
public:
    FormantSelectorPanel(
                HomunculusAudioProcessor& prc, AudioProcessorValueTreeState& ps, HumLook& look,
                         FormantManager& fmgr);
    ~FormantSelectorPanel();

    void setupSlider();
    
    void setupEditButton();
    
    void setupFormantEditorSlider();
    
    void setupAddButton();
    
    void setupRemoveButton();
    
    void syncFormantManager();
    
  
    void resized() override;
    
    std::function<void(bool toggleState)> onEditToggle;
    std::function<void()> onFormantAdd;
    std::function<void()> onFormantRemove;
    std::function<void()> onEditSlider;
    std::function<void()> onInterpolatorSlider;

    
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
