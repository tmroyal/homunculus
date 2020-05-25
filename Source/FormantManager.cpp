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


#include "FormantManager.h"

Formant::Formant(){}

Formant::Formant(XmlElement* xml){
    freq = xml->getDoubleAttribute("freq", 440.0);
    Q = xml->getDoubleAttribute("Q", 1.0);
    gain = xml->getDoubleAttribute("gain", 0.2);
}

unique_ptr<XmlElement> Formant::toXml(){
    auto tree = make_unique<XmlElement>("Formant");
    tree->setAttribute("freq", freq);
    tree->setAttribute("Q", Q);
    tree->setAttribute("gain", gain);
    return tree;
}


// ==========================


FormantSet::FormantSet(){
    defaultInit();
}

FormantSet::FormantSet(XmlElement* xml){
    
    forEachXmlChildElementWithTagName(*xml, formant, "Formant"){
        formants.push_back(Formant(formant));
    }
    
    if (formants.size() < NUMBER_OF_FORMANTS){
        defaultInit();
    }
}

void FormantSet::setFreq(int formantNumber, float freq){
    formants[formantNumber].freq = freq;
}

void FormantSet::setQ(int formantNumber, float Q){
    formants[formantNumber].Q = Q;
}

void FormantSet::setGain(int formantNumber, float gain){
    formants[formantNumber].gain = gain;
}

Formant FormantSet::getFormant(int i){
    return formants[i];
}

void FormantSet::setFormant(int i, Formant formant){
    formants[i] = formant;
}

unique_ptr<XmlElement> FormantSet::toXml(){
    auto tree = make_unique<XmlElement>("FormantSet");
    for (Formant formant: formants){
        tree->addChildElement(formant.toXml().release());
    }
    return tree;
}

void FormantSet::defaultInit(){
    formants.clear();
    for (int i = 0; i < NUMBER_OF_FORMANTS; i++){
        formants.push_back(Formant());
    }
}


// ======================


FormantManager::FormantManager(){
    Random rand;
    for (int i = 0; i < NUMBER_INITIAL_FORMANT_SETS; i++){
        formantSets.push_back(FormantSet());
       
        for (int fn = 0; fn < NUMBER_OF_FORMANTS; fn++){
            formantSets[i].setFreq(fn, rand.nextFloat()*5000.0+100.0);
            formantSets[i].setQ(fn, rand.nextFloat()*10.0+1.0);

        }
    }
}

void FormantManager::setFreq(int formantNumber, float freq){
    
    formantSets[currentFormantSet].setFreq(formantNumber, freq);
}

void FormantManager::setQ(int formantNumber, float Q){
    formantSets[currentFormantSet].setQ(formantNumber, Q);
}

void FormantManager::setGain(int formantNumber, float gain){
    formantSets[currentFormantSet].setGain(formantNumber, gain);
}

void FormantManager::addFormant(bool setCurrentFormantToLast){
    auto last = formantSets.size()-1;
    FormantSet fs(formantSets[last]);
    formantSets.push_back(fs);
    if (setCurrentFormantToLast){
        currentFormantSet = (int)last+1;
    }
}

void FormantManager::removeFormant(){
    formantSets.erase(formantSets.begin()+currentFormantSet);
    
    if (currentFormantSet > 0){
        currentFormantSet -= 1;
    }
}

void FormantManager::removeCurrentFormantSet(){
    if (formantSets.size()>0){
        formantSets.erase(formantSets.begin() + currentFormantSet);
        
        if (currentFormantSet > formantSets.size()-1){
            currentFormantSet -= 1;
        }
    }
}

FormantSet FormantManager::getInterpolatedFormants(float ind){
    // ind is 0-1
    float scaledInd = ind*(formantSets.size()-1);
    
    int lastFS = (int)formantSets.size() - 1;
    
    if (scaledInd > lastFS){
        return formantSets[lastFS];
    }
    
    if (scaledInd < 0.0){
        return formantSets[0];
    }
    
    float a, b, t;
    t = std::modf(scaledInd, &a);
    b = a+1;
    
    if (b > lastFS){
        return formantSets[lastFS];
    }
    
    return FormantSet::interpolate(formantSets[(int)a], formantSets[(int)b], t);
}

void FormantManager::reset(){
    formantSets.clear();
    for (int i = 0; i < NUMBER_INITIAL_FORMANT_SETS; i++){
        formantSets.push_back(FormantSet());
    }
}

int FormantManager::getNumberOfFormantSets(){
    return (int)formantSets.size();
}

void FormantManager::setCurrentFormantSet(int formantSet){
    currentFormantSet = formantSet;
}

int FormantManager::getCurrentFormantSetId(){
    return currentFormantSet;
}

FormantSet FormantManager::getCurrentFormantSet(){
    return formantSets[currentFormantSet];
}

unique_ptr<XmlElement> FormantManager::toXml(){
    auto tree = make_unique<XmlElement>("FormantSets");
    
    tree->setAttribute(Identifier("currentFormantSet"), currentFormantSet);
    
    for (FormantSet formantSet: formantSets){
        tree->addChildElement(formantSet.toXml().release());
    }
    
    return tree;
}

void FormantManager::setFromXml(XmlElement* element){
    formantSets.clear();
    currentFormantSet = element->getIntAttribute("currentFormantSet",0);
    
    forEachXmlChildElementWithTagName(*element, formantSetXml, "FormantSet"){
        formantSets.push_back(FormantSet(formantSetXml));
    }
    
    // if xml has 0-1 formanSets
    // start over
    if (formantSets.size() < 2){
        formantSets.clear();
        for (int i = 0; i < NUMBER_INITIAL_FORMANT_SETS; i++){
            formantSets.push_back(FormantSet());
        }
    }
}
