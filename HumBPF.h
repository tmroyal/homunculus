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
#include <complex>

// a mono resonance filter
class HumBPF : public ProcessorBase {
public:
    HumBPF();
    
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    
    void setFreq(double f);
    
    void setQ(double newQ);
    
    void setGain(double g);
    
    void setFreqAndQ(double f, double newQ);
    
    void setCoofs(double sampleRate);
    
    void processBlock(AudioSampleBuffer& buffer, MidiBuffer&) override;
    
    double freqResponseAt(double angle);
    
private:
    double b0, b1, b2, a0, a1, a2;
    double freq, Q;
    double y1, y2, x1, x2;
    double gain;
    
    float rnd;
    
    std::complex<double> exp_z(double delt, double power);
    
    static BusesProperties getBusesProperties()
    {
        return BusesProperties().withOutput ("Output", AudioChannelSet::discreteChannels(1), true)
        .withInput ("Input", AudioChannelSet::discreteChannels(1), true);
    }
};
