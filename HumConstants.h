/*
  ==============================================================================

    HumConstants.h
    Created: 6 May 2020 6:38:21pm
    Author:  Thomas Royal

  ==============================================================================
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

const static int TITLE_LEFT = FORMANT_EDITOR_LEFT+BOX_SIZE*2.3-80;
