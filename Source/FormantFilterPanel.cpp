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


#include "FormantFilterPanel.h"

FormantFilterPanel::FormantFilterPanel(HomunculusAudioProcessor& proc, AudioProcessorValueTreeState& ps, HumLook& lf, FormantManager& fmgr)
:
HumComponent(proc, ps, lf),
formantManager(fmgr)
{
    setupLabel(freqLabel,"F");
    setupLabel(QLabel, "Q");
    setupLabel(gainLabel, "Gain");
    setupLabel(f1Label, "F1");
    setupLabel(f2Label, "F2");
    setupLabel(f3Label, "F3");
    
    setupFormantSliders();
    
}

FormantFilterPanel::~FormantFilterPanel(){}

void FormantFilterPanel::setupFormantSliders(){
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
        freqSlider->setSliderStyle(Slider::SliderStyle::Rotary);
        freqSlider->setTextBoxStyle(Slider::TextBoxBelow, false, BOX_SIZE, BOX_SIZE/6);
        freqSlider->setNumDecimalPlacesToDisplay(2);
        
        QSlider->setNormalisableRange(
                                      NormalisableRange<double>(0.5,20,0,0.8));
        QSlider->setSliderStyle(Slider::SliderStyle::Rotary);
        QSlider->setTextBoxStyle(Slider::TextBoxBelow, false, BOX_SIZE, BOX_SIZE/6);
        QSlider->setNumDecimalPlacesToDisplay(2);
        
        gainSlider->setNormalisableRange(
                                         NormalisableRange<double>(0.0,1.0,0,0.7));
        gainSlider->setSliderStyle(Slider::SliderStyle::Rotary);
        gainSlider->setTextBoxStyle(Slider::TextBoxBelow, false, BOX_SIZE, BOX_SIZE/6);
        gainSlider->setNumDecimalPlacesToDisplay(2);
    }
}

void FormantFilterPanel::resized() {
    auto it = sliders.begin();
    auto end = sliders.end();
    auto i = 0;
    
    f1Label.setBounds(LABEL_SIZE*2.5, 0, BOX_SIZE, LABEL_SIZE);
    f2Label.setBounds(LABEL_SIZE*2.5+BOX_SIZE, 0, BOX_SIZE, LABEL_SIZE);
    f3Label.setBounds(LABEL_SIZE*2.5+BOX_SIZE*2, 0, BOX_SIZE, LABEL_SIZE);
    
    freqLabel.setBounds(0, LABEL_SIZE, BOX_SIZE, BOX_SIZE);
    QLabel.setBounds(0, LABEL_SIZE+BOX_SIZE, BOX_SIZE, BOX_SIZE);
    gainLabel.setBounds(0, LABEL_SIZE+BOX_SIZE*2, BOX_SIZE, BOX_SIZE);
    
    while (it != end){
        (**it).setBounds(LABEL_SIZE*2.5+(i/3)*BOX_SIZE, (i%3)*BOX_SIZE+LABEL_SIZE, BOX_SIZE, BOX_SIZE);
        it++; i++;
    }
}

void FormantFilterPanel::syncFormantManager(){
    for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
        sliders[i*3]->setValue(
                               formantManager.getCurrentFormantSet().getFormant(i).freq);
        sliders[i*3+1]->setValue(
                                 formantManager.getCurrentFormantSet().getFormant(i).Q);
        sliders[i*3+2]->setValue(
                                 formantManager.getCurrentFormantSet().getFormant(i).gain);
    }
}
