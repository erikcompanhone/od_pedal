# pragma once

# include <juce_gui_basics/juce_gui_basics.h>

class LEDIndicator : public juce::Component
{
public:
    // constructor
    LEDIndicator();

    // paint handler (hidden - LED is in pedal body image)
    void paint(juce::Graphics&) override;
    
    // set LED state
    void setLit(bool shouldBeLit);

    // getter for LED state
    bool isLit() const { return isLitState; }

private:
    bool isLitState = false;
};
