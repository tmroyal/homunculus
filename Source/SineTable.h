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
    SineTable();
    
    // where angle represents radians
    double get(double angle);
    
private:
    vector<float> table;

};
