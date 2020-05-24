/*
  ==============================================================================

    HumBPF.h
    Created: 3 May 2020 4:20:51pm
    Author:  Thomas Royal

    Band pass filter
 
  ==============================================================================
*/

#pragma once

#include "ProcessorBase.h"

// a mono resonance filter
class HumBPF : public ProcessorBase {
public:
    HumBPF(): ProcessorBase(getBusesProperties()), freq(440), Q(10), gain(1.0)
    {
        Random rng;
        rnd = rng.nextFloat();
    }
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        y1 = y2 = x1 = x2 = 0.0;
        setCoofs(sampleRate);
        
    }
    
    void setFreq(double f){
        freq = f;
        if (getSampleRate() != 0){
            setCoofs(getSampleRate());
        }
    }
    
    void setQ(double newQ){
        Q = newQ;
        if (getSampleRate() != 0){
            setCoofs(getSampleRate());
        }
    }
    
    void setGain(double g){
        gain = g;
        if (getSampleRate() != 0){
            setCoofs(getSampleRate());
        }
    }
    
    void setFreqAndQ(double f, double newQ){
        freq = f;
        Q = newQ;
    }
    
    void setCoofs(double sampleRate){
        double omega = MathConstants<double>::twoPi*(freq/sampleRate);
        double alpha = sin(omega)/(2.0*Q);

        a0 = 1+alpha;

        b0 = alpha/a0;
        b1 = 0;
        b2 = -alpha/a0;
        a1 = -2*cos(omega)/a0;
        a2 = (1-alpha)/a0;
        
        // a0/a0
        a0 = 1;
    }
    
    void processBlock(AudioSampleBuffer& buffer, MidiBuffer&) override {
        auto numSamples = buffer.getNumSamples();
        auto *output = buffer.getWritePointer(0);
        auto *input = buffer.getReadPointer(0);
        
        for (int i = 0; i < numSamples; ++i){
            output[i] = b0*input[i]*gain + b1*x1 + b2*x2 - a1*y1 - a2*y2;
            
            x2 = x1;
            x1 = input[i];
            y2 = y1;
            y1 = output[i];
        }
    }
    
    
private:
    double b0, b1, b2, a0, a1, a2;
    double freq, Q;
    double y1, y2, x1, x2;
    double gain;
    
    float rnd;
    
    static BusesProperties getBusesProperties()
    {
        return BusesProperties().withOutput ("Output", AudioChannelSet::discreteChannels(1), true)
        .withInput ("Input", AudioChannelSet::discreteChannels(1), true);
    }
};
