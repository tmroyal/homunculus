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

#define NUMBER_OF_FORMANTS 3
#define NUMBER_OF_VOICES 16
#define NUMBER_INITIAL_FORMANT_SETS 2

// TODO: anything but this

const static int LABEL_SIZE = 20;
const static int BOX_SIZE = 80;

const static int FORMANTS_LEFT = -20;
const static int FORMANTS_TOP = 120;

const static int ADSR_LEFT = 260;
const static int ADSR_TOP = LABEL_SIZE*2;

const static int FORMANT_EDITOR_LEFT = 390;
const static int FORMANT_EDITOR_TOP = 10;

const static int LFO_LEFT = BOX_SIZE-LABEL_SIZE;
const static int LFO_TOP = LABEL_SIZE*0.5;

const static int TITLE_LEFT = FORMANT_EDITOR_LEFT+BOX_SIZE*1.6;
