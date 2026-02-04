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

    // drive slider
    driveSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    driveSlider.setRange(0.0f, 24.0f, 0.1f);
    driveSlider.setValue(0.0f);
    driveSlider.setNumDecimalPlacesToDisplay(2);
    driveSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    driveSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(driveSlider);

    // tone slider
    toneSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    toneSlider.setRange(200.0f, 8000.0f, 1.0f);
    toneSlider.setValue(3000.0f);
    toneSlider.setNumDecimalPlacesToDisplay(2);
    toneSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    toneSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(toneSlider);

    // level slider
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
    addAndMakeVisible(driveLabel);

    toneLabel.setText("Tone", juce::dontSendNotification);
    toneLabel.setJustificationType(juce::Justification::centred);
    toneLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(toneLabel);

    levelLabel.setText("Level", juce::dontSendNotification);
    levelLabel.setJustificationType(juce::Justification::centred);
    levelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(levelLabel);

    // bypass button (no label - just image-based foot switch)
    bypassButton.setButtonText("");
    bypassButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
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
        // Update LED state: lights when bypass is OFF
        isLit = !bypassButton.getToggleState();
        // Repaint the editor to switch pedal body image
        repaint();
    }
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    int windowWidth = bounds.getWidth();
    int windowHeight = bounds.getHeight();

    // pedal area with margin
    int pedalX = MARGIN;
    int pedalY = MARGIN;
    int pedalWidth = PEDAL_WIDTH;
    int pedalHeight = PEDAL_HEIGHT;

    // calculate knob size based on pedal width
    float knobSize = pedalWidth * 0.22f;
    float ledSize = knobSize / 3.0f;
    float buttonHeight = knobSize * 0.5f;
    float spacing = knobSize * 0.15f;
    float labelHeight = 25.0f;

    // top section (drive and level knobs) - positioned within pedal bounds
    float topY = pedalY + spacing;
    float driveX = pedalX + spacing * 2;
    float levelX = pedalX + pedalWidth - spacing * 2 - knobSize;
    float toneCentreX = pedalX + (pedalWidth / 2.0f) - (knobSize / 2.0f);

    // drive knob + label
    driveSlider.setBounds(driveX, topY, knobSize, knobSize);
    driveLabel.setBounds(driveX, topY + knobSize + spacing * 0.5f, knobSize, labelHeight);

    // level knob + label
    levelSlider.setBounds(levelX, topY, knobSize, knobSize);
    levelLabel.setBounds(levelX, topY + knobSize + spacing * 0.5f, knobSize, labelHeight);

    // tone knob (centered) + label above knob
    float toneTopY = topY + knobSize + labelHeight + spacing * 2;
    toneLabel.setBounds(toneCentreX, toneTopY, knobSize, labelHeight);
    toneSlider.setBounds(toneCentreX, toneTopY + labelHeight + spacing * 0.5f, knobSize, knobSize);

    // bypass button (centered at bottom, no label)
    float buttonWidth = pedalWidth * 0.4f;
    float buttonTopY = pedalY + pedalHeight - spacing * 2 - buttonHeight;
    float buttonX = pedalX + (pedalWidth / 2.0f) - (buttonWidth / 2.0f);
    bypassButton.setBounds(buttonX, buttonTopY, buttonWidth, buttonHeight);
}


void PluginEditor::loadPedalBodyImages()
{
    if (pedalBodyOnImage.isValid() && pedalBodyOffImage.isValid())
        return;

    // Load pedal body images from binary resources
    pedalBodyOnImage = juce::ImageCache::getFromMemory(BinaryData::pedal_body_on_png, BinaryData::pedal_body_on_pngSize);
    pedalBodyOffImage = juce::ImageCache::getFromMemory(BinaryData::pedal_body_off_png, BinaryData::pedal_body_off_pngSize);
}

void PluginEditor::paint(juce::Graphics& g)
{
    // Dark grey background for border/margin area
    g.fillAll(juce::Colour::fromFloatRGBA(0.15f, 0.15f, 0.15f, 1.0f));

    // Load and draw pedal body image based on LED state
    loadPedalBodyImages();
    auto imageToUse = isLit ? pedalBodyOnImage : pedalBodyOffImage;
    if (imageToUse.isValid())
    {
        // Scale image to fit pedal bounds
        g.drawImageWithin(imageToUse, 
                         MARGIN, MARGIN, PEDAL_WIDTH, PEDAL_HEIGHT,
                         juce::RectanglePlacement::stretchToFit,
                         false);
    }
}
