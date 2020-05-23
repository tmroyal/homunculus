/*
  ==============================================================================

    SineTable.h
    Created: 23 May 2020 3:13:05pm
    Author:  Thomas Royal

  ==============================================================================
*/

#pragma once

#include <vector>
#include <JuceHeader.h>
#include <cmath>

#define TABLE_SIZE 1024

using std::vector;

class SineTable{
    
public:
    SineTable(){
        // generate wavetable
        for (int i = 0; i < TABLE_SIZE; i++){
            double angle =
                (double(i)/TABLE_SIZE)*MathConstants<double>::twoPi;
            table.push_back(sin(angle));
        }
    }
    
    // where angle represents radians
    double get(double angle){
        
        // map 0-twopi to 0-tablesize
        // and then perform linear interpolation
        double index = TABLE_SIZE*(angle/MathConstants<double>::twoPi);
        int a = static_cast<int>(floor(index)) % TABLE_SIZE;
        int b = static_cast<int>(ceil(index)) % TABLE_SIZE;
        double intpart;
        double fracPart = modf(index, &intpart);
        
        return table[a]*(1-fracPart)+table[b]*fracPart;
    }
    
private:
    vector<float> table;

};
