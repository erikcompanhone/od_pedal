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
    const float KNOB_CENTRE_OFFSET_X = -knobImage.getWidth() / 2.0f;
    const float KNOB_CENTRE_OFFSET_Y = -knobImage.getHeight() / 2.0f;

    if (knobImage.isValid())
    {
        // calculate rotation angle
        auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // draw rotated knob image with uniform scaling
        g.drawImageTransformed(knobImage,
                              juce::AffineTransform::translation(KNOB_CENTRE_OFFSET_X, KNOB_CENTRE_OFFSET_Y)
                                  .scaled(uniformSize / (float)knobImage.getWidth())
                                  .rotated(angle, 0.0f, 0.0f)
                                  .translated(centreX, centreY),
                              false);
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

void GoldButtonLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                             bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    
    loadBypassImages();

    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();
    constexpr float BYPASS_IMAGE_SCALE = 1.5f;
    const float IMAGE_CENTRE_OFFSET_X = -bypassUpImage.getWidth() / 2.0f;
    const float IMAGE_CENTRE_OFFSET_Y = -bypassUpImage.getHeight() / 2.0f;

    if (bypassUpImage.isValid())
    {
        auto scale = juce::jmin(bounds.getWidth() / (float)bypassUpImage.getWidth(),
                               bounds.getHeight() / (float)bypassUpImage.getHeight()) * BYPASS_IMAGE_SCALE;

        float opacity = shouldDrawButtonAsDown ? 0.6f : 1.0f;

        g.saveState();
        g.setOpacity(opacity);
        g.drawImageTransformed(bypassUpImage,
                              juce::AffineTransform::translation(IMAGE_CENTRE_OFFSET_X, IMAGE_CENTRE_OFFSET_Y)
                                  .scaled(scale)
                                  .translated(centreX, centreY),
                              false);
        g.restoreState();
    }
}
