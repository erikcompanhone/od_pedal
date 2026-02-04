# include "CustomLookAndFeel.h"

void GoldKnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                                           juce::Slider& slider)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // Gold knob
    g.setColour(juce::Colour::fromFloatRGBA(0.86f, 0.76f, 0.24f, 1.0f)); // Gold
    g.fillEllipse(rx, ry, rw, rw);

    // Dark outline
    g.setColour(juce::Colours::darkgrey);
    g.drawEllipse(rx, ry, rw, rw, 2.0f);

    // Pointer
    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle, centreX, centreY));
    g.setColour(juce::Colours::black);
    g.fillPath(p);
}

void GoldButtonLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                                  const juce::Colour& backgroundColour,
                                                  bool isMouseOverButton, bool isButtonDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);
    auto baseColour = juce::Colour::fromFloatRGBA(0.86f, 0.76f, 0.24f, 1.0f); // Gold

    if (isButtonDown)
        g.setColour(baseColour.darker(0.2f));
    else if (isMouseOverButton)
        g.setColour(baseColour.brighter(0.1f));
    else
        g.setColour(baseColour);

    g.fillRoundedRectangle(bounds, 8.0f);

    // Border
    g.setColour(juce::Colours::darkgrey);
    g.drawRoundedRectangle(bounds, 8.0f, 2.0f);
}
