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
    BlitSynthVoice(){
        
        envelope.setSampleRate(getSampleRate());
        setParams(0.1, 0.2, 0.5, 0.5);
        
    }
    
    bool canPlaySound(SynthesiserSound* sound) override {
        return dynamic_cast<BlitSynthSound*>(sound) != nullptr;
    }
    
    void startNote ( int midiNoteNumber, float velocity, SynthesiserSound*, int curPitchWheelPos) override {
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
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock (AudioSampleBuffer& buffer, int startSample, int numSamples) override {
        if (angleDelta != 0.0){
            while (--numSamples >= 0){
                double currentSample;
                
                if (currentAngle == 0.0){
                    currentSample = 1.0;
                } else {
                    currentSample = (sin(m*currentAngle)/(m*sin(currentAngle)))*level;
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
                
                currentAngle += angleDelta;

                ++startSample;
            }
        }
    }
    
    void setParams(float attack, float decay, float sustain, float release){
        ADSR::Parameters params;
        params.attack = attack;
        params.decay = decay;
        params.sustain = sustain;
        params.release = release;
        
        envelope.setParameters(params);

    }
    
private:
    double currentAngle = 0.0, angleDelta = 0.0, level=0.0;
    bool tailOff = false;
    double m = 1.0;
        
    ADSR envelope;
};
