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

#include <vector>
#include "HumConstants.h"
#include <JuceHeader.h>
#include <cmath>
#include <string>

using namespace std;

//=========================

class Formant  {
public:
    Formant();
    
    Formant(XmlElement* xml);
    
    static Formant interpolate(Formant a, Formant b, float t){
        Formant interpolated;
        interpolated.freq =  a.freq*(1-t)+b.freq*t;
        interpolated.Q =  a.Q*(1-t)+b.Q*t;
        interpolated.gain =  a.gain*(1-t)+b.gain*t;
        
        return interpolated;
    }
    
    unique_ptr<XmlElement> toXml();
    
    float freq = 440;
    float Q = 1.0;
    float gain = 0.2;
};

// ========================

class FormantSet {
public:
    FormantSet();
    FormantSet(XmlElement* xml);
    
    void setFreq(int formantNumber, float freq);
    
    void setQ(int formantNumber, float Q);
    
    void setGain(int formantNumber, float gain);
    
    static FormantSet interpolate(FormantSet a, FormantSet b, float t){
        FormantSet fs;
        
        for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
            Formant interpolatedFormant = Formant::interpolate(a.getFormant(i), b.getFormant(i), t);
            fs.setFormant( i, interpolatedFormant);
        }
        
        return fs;
    }
    
    Formant getFormant(int i);
    
    void setFormant(int i, Formant formant);
    
    unique_ptr<XmlElement> toXml();
    
private:
    
    void defaultInit();
    
    std::vector<Formant> formants;
};

// =========================

class FormantManager {
public:
    FormantManager();

    void setFreq(int formantNumber, float freq);
    void setQ(int formantNumber, float Q);
    void setGain(int formantNumber, float gain);
    
    void addFormant(bool setCurrentFormantToLast = true);
    void removeFormant();
    void removeCurrentFormantSet();
    
    FormantSet getInterpolatedFormants(float ind);
    
    void reset();
    
    int getNumberOfFormantSets();
    
    void setCurrentFormantSet(int formantSet);
    
    int getCurrentFormantSetId();
    
    FormantSet getCurrentFormantSet();
    
    unique_ptr<XmlElement> toXml();
    
    void setFromXml(XmlElement* element);
    
private:
    int currentFormantSet = 0;
    std::vector<FormantSet> formantSets;
    
};
