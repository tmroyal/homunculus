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
