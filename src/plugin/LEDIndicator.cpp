# include "LEDIndicator.h"

LEDIndicator::LEDIndicator()
{
    setInterceptsMouseClicks(false, false);
}

void LEDIndicator::paint(juce::Graphics& g)
{
    // Paint nothing - LED is now part of the pedal body image
    // This component just tracks the LED state
}

void LEDIndicator::setLit(bool shouldBeLit)
{
    if (isLitState != shouldBeLit)
    {
        isLitState = shouldBeLit;
        repaint();
    }
}
