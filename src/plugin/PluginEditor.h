# pragma once

# include <juce_gui_basics/juce_gui_basics.h>
# include <juce_audio_processors/juce_audio_processors.h>
# include "PluginProcessor.h"
# include "CustomLookAndFeel.h"

class PluginEditor : public juce::AudioProcessorEditor, public juce::Button::Listener
{
    public:
        // constructor and destructor
        PluginEditor (PluginProcessor& processorRef);
        ~PluginEditor() override;
        
        // window resize handler
        void resized() override;

        // window paint handler
        void paint (juce::Graphics& graphicsRef) override;
        
        // button listener
        void buttonClicked(juce::Button* button) override;
    
    private:
        void timerCallback();

        // processor
        PluginProcessor& processor;

        // pedal body images
        juce::Image pedalBodyOnImage;
        juce::Image pedalBodyOffImage;
        void loadPedalBodyImages();

        // sliders
        juce::Slider driveSlider;
        juce::Slider toneSlider;
        juce::Slider levelSlider;

        // labels
        juce::Label driveLabel;
        juce::Label toneLabel;
        juce::Label levelLabel;

        // bypass button
        juce::ToggleButton bypassButton;
        
        // LED state tracker (LED is part of pedal body image)
        // Initialize to true: LED lights when bypass is OFF (bypass starts OFF)
        bool isLit = true;

        // attachments
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;

        // custom look and feel
        GoldKnobLookAndFeel goldKnobLAF;
        GoldButtonLookAndFeel goldButtonLAF;

        // layout constants
        static constexpr int WINDOW_WIDTH = 350;
        static constexpr int WINDOW_HEIGHT = 600;
        static constexpr int MARGIN = 10;
        static constexpr int PEDAL_WIDTH = 330;
        static constexpr int PEDAL_HEIGHT = 580;
};
