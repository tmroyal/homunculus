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
#include "SineTable.h"

class BlitSynthSound : public SynthesiserSound {
public:
    BlitSynthSound(){}
    
    bool appliesToNote (int note) override {
        return note >= 21 && note <= 120;
    }
    bool appliesToChannel (int) override { return true; }
    
};

// TODO:: pull these methods into c++ file
class BlitSynthVoice : public SynthesiserVoice {    
public:
    BlitSynthVoice(SineTable* table);
    
    bool canPlaySound(SynthesiserSound* sound) override;
    
    void startNote ( int midiNoteNumber, float velocity, SynthesiserSound*, int curPitchWheelPos) override;
    void stopNote (float, bool allowTailOff) override;
    
    void setLFOFreq(float freq);
    void setLFOAmount(float perc);
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void setParams(float attack, float decay, float sustain, float release, float lfoF, float lfoA);
    
    void renderNextBlock (AudioSampleBuffer& buffer, int startSample, int numSamples) override;
    
private:
    double currentAngle = 0.0, angleDelta = 0.0, level=0.0;
    double currentLFOAngle = 0.0, LFODelta = 0.0, LFOScale = 0.0;
    bool tailOff = false;
    double m = 1.0;
    SineTable* sineTable;
        
    ADSR envelope;
};
