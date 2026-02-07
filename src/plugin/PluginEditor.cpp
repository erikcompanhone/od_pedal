# include "PluginEditor.h"
# include "BinaryData.h"

PluginEditor::PluginEditor(PluginProcessor& processorRef)
    : AudioProcessorEditor(processorRef), processor(processorRef)
{
    // set window size (fixed, non-resizable)
    setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // apply custom look and feel
    driveSlider.setLookAndFeel(&goldKnobLAF);
    toneSlider.setLookAndFeel(&goldKnobLAF);
    levelSlider.setLookAndFeel(&goldKnobLAF);
    bypassButton.setLookAndFeel(&goldButtonLAF);

    // sliders
    driveSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    driveSlider.setRange(0.0f, 24.0f, 0.1f);
    driveSlider.setValue(0.0f);
    driveSlider.setNumDecimalPlacesToDisplay(2);
    driveSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    driveSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(driveSlider);

    toneSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    toneSlider.setRange(800.0f, 8000.0f, 1.0f);
    toneSlider.setValue(800.0f);
    toneSlider.setNumDecimalPlacesToDisplay(2);
    toneSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    toneSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(toneSlider);

    levelSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    levelSlider.setRange(-12.0f, 12.0f, 0.1f);
    levelSlider.setValue(0.0f);
    levelSlider.setNumDecimalPlacesToDisplay(2);
    levelSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    levelSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(levelSlider);

    // labels
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    driveLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    addAndMakeVisible(driveLabel);

    toneLabel.setText("Tone", juce::dontSendNotification);
    toneLabel.setJustificationType(juce::Justification::centred);
    toneLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    toneLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    addAndMakeVisible(toneLabel);

    levelLabel.setText("Level", juce::dontSendNotification);
    levelLabel.setJustificationType(juce::Justification::centred);
    levelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    levelLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    addAndMakeVisible(levelLabel);

    // bypass button
    bypassButton.setButtonText("");
    bypassButton.setClickingTogglesState(true);
    bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    bypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    bypassButton.setColour(juce::ToggleButton::textColourId, juce::Colours::transparentBlack);
    bypassButton.setColour(juce::TextButton::textColourOffId, juce::Colours::transparentBlack);
    bypassButton.setColour(juce::TextButton::textColourOnId, juce::Colours::transparentBlack);
    bypassButton.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(bypassButton);

    // attachments
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.apvts, ODPedalParameters::DRIVE_ID, driveSlider
    );
    toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.apvts, ODPedalParameters::TONE_ID, toneSlider
    );
    levelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.apvts, ODPedalParameters::LEVEL_ID, levelSlider
    );
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processor.apvts, ODPedalParameters::BYPASS_ID, bypassButton
    );

    // set slider text formatting functions
    driveSlider.textFromValueFunction = [this](double v) {
        return juce::String(v, 2);
    };
    toneSlider.textFromValueFunction = [this](double v) {
        return juce::String((int)v);
    };
    levelSlider.textFromValueFunction = [this](double v) {
        return juce::String(v, 2);
    };

    // force immediate text update to apply formatting on startup
    driveSlider.updateText();
    toneSlider.updateText();
    levelSlider.updateText();

    // add button listener for LED update
    bypassButton.addListener(this);
}

PluginEditor::~PluginEditor()
{
    bypassButton.removeListener(this);
    driveSlider.setLookAndFeel(nullptr);
    toneSlider.setLookAndFeel(nullptr);
    levelSlider.setLookAndFeel(nullptr);
    bypassButton.setLookAndFeel(nullptr);
}

void PluginEditor::buttonClicked(juce::Button* button)
{
    if (button == &bypassButton)
    {
        isLit = !bypassButton.getToggleState();
        repaint();
    }
}

void PluginEditor::resized()
{
    // knob sizes (25% reduction from previous)
    float knobSize = DRIVE_LEVEL_KNOB_SIZE;
    float toneKnobSize = TONE_KNOB_SIZE;
    float labelHeight = LABEL_HEIGHT;

    // drive knob: left side
    driveSlider.setBounds(DRIVE_KNOB_X, DRIVE_KNOB_Y, knobSize, knobSize + 50.0f);
    driveLabel.setBounds(DRIVE_LABEL_X, DRIVE_LABEL_Y, knobSize, labelHeight);

    // level knob: right side
    levelSlider.setBounds(LEVEL_KNOB_X, LEVEL_KNOB_Y, knobSize, knobSize + 50.0f);
    levelLabel.setBounds(LEVEL_LABEL_X, LEVEL_LABEL_Y, knobSize, labelHeight);

    // tone knob: center
    toneSlider.setBounds(TONE_KNOB_X, TONE_KNOB_Y, toneKnobSize, toneKnobSize + 50.0f);
    toneLabel.setBounds(TONE_LABEL_X, TONE_LABEL_Y, toneKnobSize, labelHeight);

    // bypass button
    bypassButton.setBounds(BYPASS_BUTTON_X, BYPASS_BUTTON_Y, BYPASS_BUTTON_WIDTH, BYPASS_BUTTON_HEIGHT);
}


void PluginEditor::loadPedalBodyImages()
{
    if (pedalBodyOnImage.isValid() && pedalBodyOffImage.isValid())
        return;

    // load pedal body images from binary resources
    pedalBodyOnImage = juce::ImageCache::getFromMemory(BinaryData::pedal_body_on_png, BinaryData::pedal_body_on_pngSize);
    pedalBodyOffImage = juce::ImageCache::getFromMemory(BinaryData::pedal_body_off_png, BinaryData::pedal_body_off_pngSize);
}

void PluginEditor::paint(juce::Graphics& g)
{
    // dark grey background
    g.fillAll(juce::Colour::fromFloatRGBA(0.15f, 0.15f, 0.15f, 1.0f));

    // load and draw pedal body image based on LED state
    loadPedalBodyImages();
    auto imageToUse = isLit ? pedalBodyOnImage : pedalBodyOffImage;
    if (imageToUse.isValid())
    {
        // scale image to fit pedal bounds
        g.drawImageWithin(imageToUse, 
                         MARGIN, MARGIN, PEDAL_WIDTH, PEDAL_HEIGHT,
                         juce::RectanglePlacement::stretchToFit,
                         false);
    }
}
