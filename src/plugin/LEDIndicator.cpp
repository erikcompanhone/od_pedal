# include "LEDIndicator.h"

LEDIndicator::LEDIndicator()
{
    setInterceptsMouseClicks(false, false);
}

void LEDIndicator::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;

    // LED glow
    if (isLit)
    {
        juce::ColourGradient glowGradient(juce::Colours::red.withAlpha(0.6f), {centreX, centreY},
                                          juce::Colours::red.withAlpha(0.0f), {centreX + radius * 1.5f, centreY},
                                          true);
        g.setGradientFill(glowGradient);
        g.fillEllipse(centreX - radius * 1.5f, centreY - radius * 1.5f, radius * 3.0f, radius * 3.0f);
    }

    // LED body (red or grey)
    g.setColour(isLit ? juce::Colours::red : juce::Colour::fromFloatRGBA(0.3f, 0.3f, 0.3f, 1.0f));
    g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

    // LED highlight
    g.setColour(juce::Colours::white.withAlpha(0.4f));
    g.fillEllipse(centreX - radius * 0.6f, centreY - radius * 0.6f, radius * 0.8f, radius * 0.8f);

    // LED outline
    g.setColour(juce::Colours::black);
    g.drawEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 1.5f);
}

void LEDIndicator::setLit(bool shouldBeLit)
{
    if (isLit != shouldBeLit)
    {
        isLit = shouldBeLit;
        repaint();
    }
}
