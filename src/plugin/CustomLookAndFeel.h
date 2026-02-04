# pragma once

# include <juce_gui_basics/juce_gui_basics.h>

class GoldKnobLookAndFeel : public juce::LookAndFeel_V4
{
    public:
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
        void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    private:
        juce::Image bypassUpImage;
        bool bypassImagesLoaded = false;
        void loadBypassImages();
};
