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
    BlitSynthVoice(SineTable* table){
        sineTable = table;
        envelope.setSampleRate(getSampleRate());
        setParams(0.1, 0.2, 0.5, 0.5,0.0,0.0);
    }
    
    bool canPlaySound(SynthesiserSound* sound) override {
        return dynamic_cast<BlitSynthSound*>(sound) != nullptr;
    }
    
    void startNote ( int midiNoteNumber, float velocity, SynthesiserSound*, int curPitchWheelPos) override {
        float db = (1-velocity)*-40.0;
        
        if (db <= -39.9){ level = 0.0; } else { level = Decibels::decibelsToGain(db); }
        
        currentAngle = 0.0;
        level = velocity*0.9;
        
        auto cps = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cps / getSampleRate();
        
        angleDelta = cyclesPerSample * MathConstants<double>::pi;
        
        int numHarmonics = (int)((getSampleRate()/2)/cps); // since this will always be positve
        m = 2*(float)(numHarmonics)+1;
        
        envelope.noteOn();
        
    }
    
    void stopNote (float, bool allowTailOff) override {
        if (allowTailOff){
            tailOff = true;
            envelope.noteOff();
        } else {
            clearCurrentNote();
            angleDelta = 0.0;
            envelope.reset();
        }
        
    }
    
    void setLFOFreq(float freq){
        LFODelta = (freq/getSampleRate())*MathConstants<double>::twoPi;
    }
    
    void setLFOAmount(float perc){
        LFOScale = perc*(10/getSampleRate())*MathConstants<double>::twoPi;
    }
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock (AudioSampleBuffer& buffer, int startSample, int numSamples) override {
        if (angleDelta != 0.0){
            while (--numSamples >= 0){
                double currentSample;
                
                if (currentAngle == 0.0){
                    currentSample = 1.0;
                } else {
                    currentSample = (sineTable->get(m*currentAngle)/(m*sineTable->get(currentAngle)))*level;
                }

                float envSample = envelope.getNextSample();
                
                for (auto i = buffer.getNumChannels(); --i >= 0;){
                    buffer.addSample(i, startSample, currentSample*envSample);
                }
                
                if (tailOff == true && !envelope.isActive()){
                    clearCurrentNote();
                    envelope.reset();
                    angleDelta = 0.0;
                    tailOff = false;
                    break;
                }
                
                currentAngle += angleDelta+sineTable->get(currentLFOAngle)*LFOScale;
                currentLFOAngle += LFODelta;
                
                ++startSample;
            }
        }
    }
    
    void setParams(float attack, float decay, float sustain, float release, float lfoF, float lfoA){
        
        ADSR::Parameters params;
        params.attack = attack;
        params.decay = decay;
        params.sustain = sustain;
        params.release = release;
        
        envelope.setParameters(params);
        
        setLFOFreq(lfoF);
        setLFOAmount(lfoA);
    }
    
private:
    double currentAngle = 0.0, angleDelta = 0.0, level=0.0;
    double currentLFOAngle = 0.0, LFODelta = 0.0, LFOScale = 0.0;
    bool tailOff = false;
    double m = 1.0;
    SineTable* sineTable;
        
    ADSR envelope;
};
