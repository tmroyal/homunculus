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


#include "HumBPF.h"

HumBPF::HumBPF(): ProcessorBase(getBusesProperties()), freq(440), Q(10), gain(1.0)
{
    Random rng;
    rnd = rng.nextFloat();
}

void HumBPF::prepareToPlay(double sampleRate, int samplesPerBlock)  {
    y1 = y2 = x1 = x2 = 0.0;
    setCoofs(sampleRate);
    
}

void HumBPF::setFreq(double f){
    freq = f;
    if (getSampleRate() != 0){
        setCoofs(getSampleRate());
    }
}

void HumBPF::setQ(double newQ){
    Q = newQ;
    if (getSampleRate() != 0){
        setCoofs(getSampleRate());
    }
}

void HumBPF::setGain(double g){
    gain = g;
    if (getSampleRate() != 0){
        setCoofs(getSampleRate());
    }
}

void HumBPF::setFreqAndQ(double f, double newQ){
    freq = f;
    Q = newQ;
}

void HumBPF::setCoofs(double sampleRate){
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

void HumBPF::processBlock(AudioSampleBuffer& buffer, MidiBuffer&) {
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

double HumBPF::freqResponseAt(double angle){
    std::complex<double> z1 = exp_z(angle, 1.0);
    std::complex<double> z2 = exp_z(angle, 2.0);
    std::complex<double> H = gain*( b0 + b1*z1 + b2*z2)/(1.0+a1*z1+a2*z2);
    
    return std::abs(H);
}


std::complex<double> HumBPF::exp_z(double delt, double power){
    std::complex<double> ni(0.0,-1);
    return std::exp(ni*delt*power);
}
