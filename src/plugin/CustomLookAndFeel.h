# pragma once

# include <juce_gui_basics/juce_gui_basics.h>

/**
 * Custom look and feel for rotary knobs.
 * 
 * Renders gold/bronze rotary knobs from PNG images with proper
 * rotation based on slider value. Prevents non-uniform stretching
 * by using square dimensions.
 */
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

/**
 * Custom look and feel for bypass toggle button.
 * 
 * Renders a bypass foot switch image with opacity feedback:
 * - 100% opacity when unpressed (OFF state)
 * - 60% opacity when pressed (ON state)
 */
class GoldButtonLookAndFeel : public juce::LookAndFeel_V4
{
public:
    // draw custom button background
    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour,
                             bool isMouseOverButton, bool isButtonDown) override;
    
    // override toggle button drawing
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    juce::Image bypassUpImage;
    bool bypassImagesLoaded = false;
    void loadBypassImages();
};
