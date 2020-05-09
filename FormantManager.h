/*
  ==============================================================================

    FormantManager.h
    Created: 8 May 2020 4:07:33pm
    Author:  Thomas Royal

  ==============================================================================
*/

#pragma once

#include <vector>
#include "HumConstants.h"
#include <cmath>
#include <string>

class Formant {
public:
    Formant(){}
    
    static Formant interpolate(Formant a, Formant b, float t){
        Formant interpolated;
        interpolated.freq =  a.freq*(t-1)+b.freq*t;
        interpolated.Q =  a.Q*(t-1)+b.Q*t;
        interpolated.gain =  a.gain*(t-1)+b.gain*t;
        
        return interpolated;
    }
    
    float freq = 440;
    float Q = 1.0;
    float gain = 1.0;
};

class FormantSet {
public:
    FormantSet(){
        for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
            formants.push_back(Formant());
        }
    }
    
    void setFreq(int formantNumber, float freq){
        formants[formantNumber].freq = freq;
    }
    
    void setQ(int formantNumber, float Q){
        formants[formantNumber].Q = Q;
    }
    
    void setGain(int formantNumber, float gain){
        formants[formantNumber].gain = gain;
    }
    
    static FormantSet interpolate(FormantSet a, FormantSet b, float t){
        FormantSet fs;
        
        for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
            Formant interpolatedFormant = Formant::interpolate(a.getFormant(i), b.getFormant(i), t);
            fs.setFormant( i, interpolatedFormant);
        }
        
        return fs;
    }
    
    Formant getFormant(int i){
        return formants[i];
    }
    
    void setFormant(int i, Formant formant){
        formants[i] = formant;
    }
public:
    std::vector<Formant> formants;
};

class FormantManager{
public:
    FormantManager(){
        formantSets.push_back(FormantSet());
    }

    void setFreq(int formantNumber, float freq){
        formantSets[currentFormantSet].setFreq(formantNumber, freq);
    }
    
    void setQ(int formantNumber, float Q){
        formantSets[currentFormantSet].setQ(formantNumber, Q);
    }
    
    void setGain(int formantNumber, float gain){
        formantSets[currentFormantSet].setGain(formantNumber, gain);
    }
    
    void addFormant(bool setCurrentFormantToLast = true){
        auto last = formantSets.size()-1;
        FormantSet fs(formantSets[last]);
        formantSets.push_back(fs);
        if (setCurrentFormantToLast){
            currentFormantSet = (int)last+1;
        }
    }
    
    void removeCurrentFormantSet(){
        if (formantSets.size()>0){
            formantSets.erase(formantSets.begin() + currentFormantSet);
            
            if (currentFormantSet > formantSets.size()-1){
                currentFormantSet -= 1;
            }
        }
    }
    
    FormantSet getInterpolatedFormants(float ind){
        int lastFS = (int)formantSets.size() - 1;
        
        if (ind > lastFS-1){
            return formantSets[lastFS];
        }
        
        if (ind < 0.0){
            return formantSets[0];
        }
        
        float a, b, t;
        t = std::modf(ind, &a);
        b = a+1;
        
        if (b > lastFS){
            return formantSets[lastFS];
        }
       
        return FormantSet::interpolate(formantSets[(int)a], formantSets[(int)b], t);
    }
    
    void reset(){
        formantSets.clear();
        FormantSet fs;
        formantSets.push_back(fs);
    }
    
private:
    int currentFormantSet = 0;
    std::vector<FormantSet> formantSets;
    
    // valueTree?
};
