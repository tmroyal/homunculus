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


#include "FormantVisualiser.h"

FormantVisualiser::FormantVisualiser(HomunculusAudioProcessor& prc): processor(prc){
    for (int i = 0; i < VIS_TABLE_SIZE; i++){ visTable[i] = 0.0; }
}

FormantVisualiser::~FormantVisualiser(){}

void FormantVisualiser::refreshData(){
    double maxExp = exp(EXP_INDEX);
    
    for (int i = 0; i < VIS_TABLE_SIZE; i++){
        double ind = exp((EXP_INDEX*i)/VIS_TABLE_SIZE)/maxExp;
        visTable[i] = processor.freqResponseAt(ind*MathConstants<double>::pi)/2;
    }
}

void FormantVisualiser::paint(Graphics& g){
    g.fillAll(Colours::black);
    
    double prevY = (1-visTable[0])*getHeight();
    double prevX = 0;
    
    g.setColour(Colours::lightgrey);
    
    for (int i = GRAPH_STEP; i < getWidth(); i += GRAPH_STEP){
        // interpolate width % over table size
        double ind = VIS_TABLE_SIZE*i/getWidth();
        int a = floor(ind);
        int b = ceil(ind);
        double intpart;
        double frac = modf(ind, &intpart);
        
        // perform interpolation
        double amp = (1-frac)*visTable[a]+frac*visTable[b];
        
        double thisY = (1-amp)*getHeight();
        
        g.drawLine(prevX, prevY, i, thisY);
        prevX = i;
        prevY = thisY;
    }
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 8, 4);
}
