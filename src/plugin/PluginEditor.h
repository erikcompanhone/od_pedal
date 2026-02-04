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
        
        // knob and label sizes
        static constexpr float DRIVE_LEVEL_KNOB_SIZE = 74.0f;
        static constexpr float TONE_KNOB_SIZE = 62.0f;
        static constexpr float LABEL_HEIGHT = 25.0f;
        
        // drive knob and label positions
        static constexpr float DRIVE_KNOB_X = 83.0f;
        static constexpr float DRIVE_KNOB_Y = 90.0f;
        static constexpr float DRIVE_LABEL_X = 83.0f;
        static constexpr float DRIVE_LABEL_Y = 201.0f;
        
        // level knob and label positions
        static constexpr float LEVEL_KNOB_X = 193.0f;
        static constexpr float LEVEL_KNOB_Y = 90.0f;
        static constexpr float LEVEL_LABEL_X = 193.0f;
        static constexpr float LEVEL_LABEL_Y = 201.0f;
        
        // tone knob and label positions
        static constexpr float TONE_KNOB_X = 144.0f;
        static constexpr float TONE_KNOB_Y = 185.0f;
        static constexpr float TONE_LABEL_X = 144.0f;
        static constexpr float TONE_LABEL_Y = 275.0f;
        
        // bypass button positions and size
        static constexpr float BYPASS_BUTTON_X = 145.0f;  // Centered in pedal
        static constexpr float BYPASS_BUTTON_Y = 385.0f;  // Moved up 25px
        static constexpr float BYPASS_BUTTON_WIDTH = 60.0f;
        static constexpr float BYPASS_BUTTON_HEIGHT = 50.0f;
};
