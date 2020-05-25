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


#include "SineTable.h"


SineTable::SineTable(){
    // generate wavetable
    for (int i = 0; i < TABLE_SIZE; i++){
        double angle =
        (double(i)/TABLE_SIZE)*MathConstants<double>::twoPi;
        table.push_back(sin(angle));
    }
}

// where angle represents radians
double SineTable::get(double angle){
    // map 0-twopi to 0-tablesize
    // and then perform linear interpolation
    double index = TABLE_SIZE*(angle/MathConstants<double>::twoPi);
    int a = static_cast<int>(floor(index)) % TABLE_SIZE;
    int b = static_cast<int>(ceil(index)) % TABLE_SIZE;
    double intpart;
    double fracPart = modf(index, &intpart);
    
    return table[a]*(1-fracPart)+table[b]*fracPart;
}
