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
    
    Formant(const Formant& formant){
        freq = formant.freq;
        Q = formant.Q;
        gain = formant.gain;
    }
    
    Formant interpolate( Formant b, float t){
        Formant interpolated;
        interpolated.freq =  freq*(t-1)+b.freq*t;
        interpolated.Q =  Q*(t-1)+b.Q*t;
        interpolated.gain =  gain*(t-1)+b.gain*t;
        
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
            Formant f;
            formants.push_back(f);
        }
    }
    
    FormantSet(const FormantSet& fs){
        for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
            Formant f(getFormant(i));
            formants.push_back(f);
        }
    }
    
    void setParameter(int formantNumber, std::string parameterName, float value){
        // set a parameter of the formant in question, parsing the formant set
        if (parameterName == "freq"){
            formants[formantNumber].freq = value;
        } else if (parameterName == "Q"){
            formants[formantNumber].Q = value;
        } else if (parameterName == "gain"){
            formants[formantNumber].gain = value;
        }
    }
    
    static FormantSet interpolate(FormantSet a, FormantSet b, float t){
        FormantSet fs;
        
        for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
            Formant interpolatedFormant = a.getFormant(i).interpolate(b.getFormant(i), t);
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
        FormantSet fs;
        formantSets.push_back(fs);
    }
    
    void setParameter(int formantNumber, std::string& parameterName, float value){
        formantSets[currentFormantSet].setParameter(formantNumber, parameterName, value);
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
};
