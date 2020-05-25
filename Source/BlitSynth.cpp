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


#include "BlitSynth.h"

BlitSynthVoice::BlitSynthVoice(SineTable* table){
    sineTable = table;
    envelope.setSampleRate(getSampleRate());
    setParams(0.1, 0.2, 0.5, 0.5,0.0,0.0);
}


bool BlitSynthVoice::canPlaySound(SynthesiserSound* sound)  {
    return dynamic_cast<BlitSynthSound*>(sound) != nullptr;
}

void BlitSynthVoice::startNote ( int midiNoteNumber, float velocity, SynthesiserSound*, int curPitchWheelPos){
    
    float db = (1-velocity)*-40.0;
    
    if (db <= -39.9){
        level = 0.0;
        
    } else {
        level = Decibels::decibelsToGain(db);
    }
    
    currentAngle = 0.0;
    level = velocity*0.9;
    
    auto cps = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample = cps / getSampleRate();
    
    angleDelta = cyclesPerSample * MathConstants<double>::pi;
    
    int numHarmonics = (int)((getSampleRate()/2)/cps);
    m = 2*(float)(numHarmonics)+1;
    
    envelope.noteOn();
}

void BlitSynthVoice::stopNote (float, bool allowTailOff) {
    if (allowTailOff){
        tailOff = true;
        envelope.noteOff();
    } else {
        clearCurrentNote();
        angleDelta = 0.0;
        envelope.reset();
    }
    
}

void BlitSynthVoice::setLFOFreq(float freq){
    LFODelta = (freq/getSampleRate())*MathConstants<double>::twoPi;
}

void BlitSynthVoice::setLFOAmount(float perc){
    LFOScale = perc*(10/getSampleRate())*MathConstants<double>::twoPi;
}


void BlitSynthVoice::setParams(float attack, float decay, float sustain, float release, float lfoF, float lfoA){
    
    ADSR::Parameters params;
    params.attack = attack;
    params.decay = decay;
    params.sustain = sustain;
    params.release = release;
    
    envelope.setParameters(params);
    
    setLFOFreq(lfoF);
    setLFOAmount(lfoA);
}


void BlitSynthVoice::renderNextBlock (AudioSampleBuffer& buffer, int startSample, int numSamples) {
    
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
            
            currentAngle += fmod(
                                 angleDelta+sineTable->get(currentLFOAngle)*LFOScale,
                                 MathConstants<double>::twoPi
                                 );
            currentLFOAngle += fmod(
                                    LFODelta,
                                    MathConstants<double>::twoPi
                                    );
            
            ++startSample;
        }
    }
}
