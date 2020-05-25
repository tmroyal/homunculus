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

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <array>
#include <cmath>

#define VIS_TABLE_SIZE 256
#define GRAPH_STEP 2
// this determines the graph scale
#define EXP_INDEX 5.0


using std::exp;

class FormantVisualiser : public Component {
public:
    FormantVisualiser(HomunculusAudioProcessor& prc);
    
    ~FormantVisualiser();
    
    void refreshData();
    
    void paint(Graphics& g);
    
private:
    HomunculusAudioProcessor& processor;
    std::array<double, VIS_TABLE_SIZE> visTable;
};
