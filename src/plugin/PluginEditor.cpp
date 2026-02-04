# include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& processorRef)
    : AudioProcessorEditor(processorRef), processor(processorRef)
{
    // set window size
    setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setResizable(true, true);
    setResizeLimits(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, MAX_WINDOW_WIDTH, MAX_WINDOW_HEIGHT);

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
    driveSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    driveSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(driveSlider);

    // tone slider
    toneSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    toneSlider.setRange(200.0f, 8000.0f, 1.0f);
    toneSlider.setValue(3000.0f);
    toneSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    toneSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(toneSlider);

    // level slider
    levelSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 60, 20);
    levelSlider.setRange(-12.0f, 12.0f, 0.1f);
    levelSlider.setValue(0.0f);
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

    // bypass button
    bypassButton.setButtonText("BYPASS");
    bypassButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    addAndMakeVisible(bypassButton);

    // led indicator
    addAndMakeVisible(ledIndicator);

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
        ledIndicator.setLit(bypassButton.getToggleState());
    }
}

void PluginEditor::resized()
{
    auto bounds = getLocalBounds();
    int windowWidth = bounds.getWidth();
    int windowHeight = bounds.getHeight();

    // calculate knob size based on window width (responsive)
    float knobSize = juce::jlimit(70.0f, 140.0f, windowWidth * 0.22f);
    float ledSize = knobSize / 3.0f;
    float buttonHeight = knobSize * 0.5f;
    float spacing = knobSize * 0.15f;
    float labelHeight = 25.0f;

    // top section (drive and level knobs)
    float topY = spacing;
    float driveX = spacing * 2;
    float levelX = windowWidth - driveX - knobSize;
    float toneCentreX = (windowWidth / 2.0f) - (knobSize / 2.0f);

    // drive knob + label
    driveSlider.setBounds(driveX, topY, knobSize, knobSize);
    driveLabel.setBounds(driveX, topY + knobSize + spacing * 0.5f, knobSize, labelHeight);

    // level knob + label
    levelSlider.setBounds(levelX, topY, knobSize, knobSize);
    levelLabel.setBounds(levelX, topY + knobSize + spacing * 0.5f, knobSize, labelHeight);

    // tone knob (centered) + label
    float toneTopY = topY + knobSize + labelHeight + spacing * 2;
    toneSlider.setBounds(toneCentreX, toneTopY, knobSize, knobSize);
    toneLabel.setBounds(toneCentreX, toneTopY + knobSize + spacing * 0.5f, knobSize, labelHeight);

    // LED indicator (centered, below tone)
    float ledTopY = toneTopY + knobSize + labelHeight + spacing * 3;
    float ledX = (windowWidth / 2.0f) - (ledSize / 2.0f);
    ledIndicator.setBounds(ledX, ledTopY, ledSize, ledSize);

    // bypass button (full width, below LED)
    float buttonTopY = ledTopY + ledSize + spacing * 2;
    bypassButton.setBounds(spacing * 2, buttonTopY, windowWidth - (spacing * 4), buttonHeight);
}

void PluginEditor::paint(juce::Graphics& g)
{
    // dark background (like a real pedal)
    g.fillAll(juce::Colour::fromFloatRGBA(0.15f, 0.15f, 0.15f, 1.0f));

    // optional: border effect
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 3);
}
