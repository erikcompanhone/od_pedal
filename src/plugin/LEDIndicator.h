# pragma once

# include <juce_gui_basics/juce_gui_basics.h>

class LEDIndicator : public juce::Component
{
public:
    // constructor
    LEDIndicator();

    // paint handler
    void paint(juce::Graphics&) override;
    
    // set LED state
    void setLit(bool shouldBeLit);

private:
    bool isLit = false;
};
