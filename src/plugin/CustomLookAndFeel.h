# pragma once

# include <juce_gui_basics/juce_gui_basics.h>

class GoldKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    // draw custom rotary slider
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                         float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                         juce::Slider&) override;

private:
    juce::Image knobImage;
    bool knobImageLoaded = false;
    void loadKnobImage();
};

class GoldButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    // draw custom button background
    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour,
                             bool isMouseOverButton, bool isButtonDown) override;

private:
    juce::Image bypassUpImage;
    juce::Image bypassDownImage;
    bool bypassImagesLoaded = false;
    void loadBypassImages();
};
