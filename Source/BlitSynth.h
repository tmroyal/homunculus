/*
  ==============================================================================

    BlitSynth.h
    Created: 3 May 2020 11:37:50am
    Author:  Thomas Royal

 
    Synth voice that implements a band limited impulse stream with envelope
 
  ==============================================================================
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
