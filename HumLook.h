/*
  ==============================================================================

    HumLook.h
    Created: 23 May 2020 4:52:24pm
    Author:  Thomas Royal

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class HumLook : public LookAndFeel_V4 {
public:
    HumLook(){
        setColourScheme(LookAndFeel_V4::getMidnightColourScheme());
        setColour(ToggleButton::tickColourId, Colour(219, 111, 61));
        
    }
    void drawRotarySlider (Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider&) override;
    
    Label* createSliderTextBox (Slider&) override;
    void drawToggleButton(Graphics&, ToggleButton&,
                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    Path getTickShape (float height) override;
    
};
