# include "CustomLookAndFeel.h"
# include "BinaryData.h"

void GoldKnobLookAndFeel::loadKnobImage()
{
    if (knobImageLoaded)
        return;

    // Load knob image from binary resources
    knobImage = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);
    knobImageLoaded = true;
}

void GoldKnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                                           juce::Slider& slider)
{
    loadKnobImage();

    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;

    if (knobImage.isValid())
    {
        // Calculate rotation angle
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Draw rotated knob image
        g.drawImageTransformed(knobImage,
                              juce::AffineTransform::translation(-knobImage.getWidth() / 2.0f, -knobImage.getHeight() / 2.0f)
                                  .scaled(width / (float)knobImage.getWidth(), height / (float)knobImage.getHeight())
                                  .rotated(angle, 0.0f, 0.0f)
                                  .translated(centreX, centreY),
                              false);
    }
    else
    {
        // Fallback: draw placeholder circle if image fails to load
        g.setColour(juce::Colours::darkgrey);
        auto radius = (float)juce::jmin(width / 2, height / 2);
        g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);
    }
}

void GoldButtonLookAndFeel::loadBypassImages()
{
    if (bypassImagesLoaded)
        return;

    // Load bypass button images from binary resources
    bypassUpImage = juce::ImageCache::getFromMemory(BinaryData::bypass_up_png, BinaryData::bypass_up_pngSize);
    bypassDownImage = juce::ImageCache::getFromMemory(BinaryData::bypass_down_png, BinaryData::bypass_down_pngSize);
    bypassImagesLoaded = true;
}

void GoldButtonLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                                  const juce::Colour& backgroundColour,
                                                  bool isMouseOverButton, bool isButtonDown)
{
    loadBypassImages();

    auto bounds = button.getLocalBounds().toFloat();
    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();

    // Choose image based on button press state (click animation)
    auto imageToUse = isButtonDown ? bypassDownImage : bypassUpImage;

    if (imageToUse.isValid())
    {
        // Opacity changes on press: 100% unpressed, 85% pressed
        float opacity = isButtonDown ? 0.85f : 1.0f;
        
        // Calculate scale to fit bounds
        auto scale = juce::jmin(bounds.getWidth() / (float)imageToUse.getWidth(),
                               bounds.getHeight() / (float)imageToUse.getHeight());

        // Draw button image centered with opacity
        g.saveState();
        g.setOpacity(opacity);
        g.drawImageTransformed(imageToUse,
                              juce::AffineTransform::translation(-imageToUse.getWidth() / 2.0f, -imageToUse.getHeight() / 2.0f)
                                  .scaled(scale)
                                  .translated(centreX, centreY),
                              false);
        g.restoreState();
    }
    else
    {
        // Fallback: draw placeholder button if images fail to load
        auto baseColour = juce::Colour::fromFloatRGBA(0.2f, 0.2f, 0.2f, 1.0f);

        if (isButtonDown)
            g.setColour(baseColour.darker(0.2f));
        else if (isMouseOverButton)
            g.setColour(baseColour.brighter(0.1f));
        else
            g.setColour(baseColour);

        g.fillRoundedRectangle(bounds, 8.0f);
        g.setColour(juce::Colours::darkgrey);
        g.drawRoundedRectangle(bounds, 8.0f, 2.0f);
    }
}
