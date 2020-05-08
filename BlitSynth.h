/*
  ==============================================================================

    BlitSynth.h
    Created: 3 May 2020 11:37:50am
    Author:  Thomas Royal

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
        ADSR::Parameters params;
        params.attack = 1.0;
        params.decay = 0.5;
        params.sustain = 0.8;
        params.release = 3.0;
        
        envelope.setSampleRate(getSampleRate());
        envelope.setParameters(params);
        
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
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override {
        if (angleDelta != 0.0){
            while (--numSamples >= 0){
                double currentSample;
                
                if (currentAngle == 0.0){
                    currentSample = 1.0;
                } else {
                    currentSample = (sin(m*currentAngle)/(m*sin(currentAngle)))*level;
                }
                

                float envSample = envelope.getNextSample();
                
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;){
                    outputBuffer.addSample(i, startSample, currentSample*envSample);
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

    
private:
    double currentAngle = 0.0, angleDelta = 0.0, level=0.0;
    bool tailOff = false;
    double m = 1.0;
    
    ADSR envelope;
};
