/*
  ==============================================================================

    HumLook.cpp
    Created: 23 May 2020 4:52:24pm
    Author:  Thomas Royal

  ==============================================================================
*/

#include "HumLook.h"


HumLook::HumLook(){
    setColourScheme(LookAndFeel_V4::getMidnightColourScheme());
    setColour(ToggleButton::tickColourId, Colour(219, 111, 61));
    setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    
}

void HumLook::drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                                  float sliderPos, float rotaryStartAngle,
                                  float rotaryEndAngle, Slider& slider)
{
    auto outline = slider.findColour (Slider::rotarySliderOutlineColourId);
    auto fill    = slider.findColour (Slider::rotarySliderFillColourId);
    
    auto bounds = Rectangle<int> (x, y, width, height).toFloat().reduced (10);
    
    auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin (4.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;
    
    Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);
    
    g.setColour (outline);
    g.strokePath (backgroundArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::butt));
    
    if (slider.isEnabled())
    {
        Path valueArc;
        valueArc.addCentredArc (bounds.getCentreX(),
                                bounds.getCentreY(),
                                arcRadius,
                                arcRadius,
                                0.0f,
                                rotaryStartAngle,
                                toAngle,
                                true);
        
        g.setColour (fill);
        g.strokePath (valueArc, PathStrokeType (lineW, PathStrokeType::curved, PathStrokeType::rounded));
    }
    
    auto thumbWidth = lineW * 3.0f;
    Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                             bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));
    
    g.setColour (slider.findColour (Slider::thumbColourId));
    g.fillEllipse (Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
}

Label* HumLook::createSliderTextBox (Slider& slider){
    auto* l = LookAndFeel_V4::createSliderTextBox (slider);
    l->setFont(Font(10.0));
    return l;
}


void HumLook::drawToggleButton (Graphics& g, ToggleButton& button,
                                  bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    
    
    drawTickBox (g, button, 0, 0.0,
                 button.getWidth(), button.getHeight(),
                 button.getToggleState(),
                 button.isEnabled(),
                 shouldDrawButtonAsHighlighted,
                 shouldDrawButtonAsDown);
    
    g.setColour (button.findColour (ToggleButton::textColourId));
    g.setFont (15.0);
    
    if (! button.isEnabled())
        g.setOpacity (0.5f);
    
    g.drawFittedText (button.getButtonText(),
                      button.getLocalBounds().withTrimmedLeft (roundToInt (button.getWidth()) + 10)
                      .withTrimmedRight (2),
                      Justification::centredLeft, 10);
}


Path HumLook::getTickShape (float height)
{
    
    Path path;
    path.addRoundedRectangle(0, 0, height, height, 0.0);
    path.scaleToFit (0, 0, height * 2.0f, height, true);
    
    return path;
}
