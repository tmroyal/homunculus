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
    BlitSynthVoice(){}
    
    bool canPlaySound(SynthesiserSound* sound) override {
        return dynamic_cast<BlitSynthSound*>(sound) != nullptr;
    }
    
    void startNote ( int midiNoteNumber, float velocity, SynthesiserSound*, int curPitchWheelPos) override {
        currentAngle = 0.0;
        level = velocity*0.5;
        tailOff = 0.0;
        
        auto cps = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cps / getSampleRate();
        
        angleDelta = cyclesPerSample * MathConstants<double>::pi;
        
        int numHarmonics = (int)((getSampleRate()/2)/cps); // since this will always be positve
        m = 2*(float)(numHarmonics)+1;
        
    }
    
    void stopNote (float, bool allowTailOff) override {
        if (allowTailOff){
            if (tailOff == 0.0){ tailOff = 1.0; }
        } else {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }
    
    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override {
        if (angleDelta != 0.0){
            while (--numSamples >= 0){
                double currentSample;
                if (angle == 0.0){
                    currentSample = 1.0;
                } else {
                    currentSample = (sin(m*currentAngle)/(m*sin(currentAngle)))*level;
                }
                
                if (tailOff > 0.0){
                    currentSample *= tailOff;
                    tailOff *= 0.99;
                }
                
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;){
                    outputBuffer.addSample(i,startSample, currentSample);
                }
                
                if (tailOff > 0.0 && tailOff <= 0.005){
                    clearCurrentNote();
                    
                    angleDelta = 0.0;
                    break;
                }
                
                currentAngle += angleDelta;

                ++startSample;
            }
        }
    }

    
private:
    double currentAngle = 0.0, angleDelta = 0.0, level=0.0, tailOff = 0.0;
    double m = 1.0;
};
