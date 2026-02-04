# include "CustomLookAndFeel.h"
# include "BinaryData.h"

void GoldKnobLookAndFeel::loadKnobImage()
{
    if (knobImageLoaded)
        return;

    // load knob image from binary resources
    knobImage = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    knobImageLoaded = true;
}

void GoldKnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                                           juce::Slider& slider)
{
    loadKnobImage();

    // use uniform square dimensions to prevent stretching
    float uniformSize = (float)juce::jmin(width, height);
    auto centreX = (float)x + (float)uniformSize * 0.5f;
    auto centreY = (float)y + (float)uniformSize * 0.5f;

    if (knobImage.isValid())
    {
        // calculate rotation angle
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // draw rotated knob image with uniform scaling
        g.drawImageTransformed(knobImage,
                              juce::AffineTransform::translation(-knobImage.getWidth() / 2.0f, -knobImage.getHeight() / 2.0f)
                                  .scaled(uniformSize / (float)knobImage.getWidth())
                                  .rotated(angle, 0.0f, 0.0f)
                                  .translated(centreX, centreY),
                              false);
    }
    else
    {
        // fallback: draw placeholder circle if image fails to load
        g.setColour(juce::Colours::darkgrey);
        auto radius = (float)juce::jmin(width / 2, height / 2);
        g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);
    }
}

void GoldButtonLookAndFeel::loadBypassImages()
{
    if (bypassImagesLoaded)
        return;

    // load bypass button image from binary resources
    bypassUpImage = juce::ImageCache::getFromMemory(BinaryData::bypass_up_png, BinaryData::bypass_up_pngSize);
    bypassImagesLoaded = true;
}

// goldButtonLookAndFeel: Custom rendering for bypass toggle button
// =================================================================

void GoldButtonLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                                  const juce::Colour& backgroundColour,
                                                  bool isMouseOverButton, bool isButtonDown)
{
    // not used for toggle buttons - see drawToggleButton
}

void GoldButtonLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                             bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    
    loadBypassImages();

    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();

    if (bypassUpImage.isValid())
    {
        // calculate scale to fit bounds, then double it for larger size, then reduce by 25%
        auto scale = juce::jmin(bounds.getWidth() / (float)bypassUpImage.getWidth(),
                               bounds.getHeight() / (float)bypassUpImage.getHeight()) * 2.0f * 0.75f;

        // opacity changes on press: 100% unpressed, 60% pressed
        float opacity = shouldDrawButtonAsDown ? 0.6f : 1.0f;

        // draw button image centered within button bounds
        g.saveState();
        g.setOpacity(opacity);
        g.drawImageTransformed(bypassUpImage,
                              juce::AffineTransform::translation(-bypassUpImage.getWidth() / 2.0f, -bypassUpImage.getHeight() / 2.0f)
                                  .scaled(scale)
                                  .translated(centreX, centreY),
                              false);
        g.restoreState();
    }
}
