# pragma once

# include <juce_audio_processors/juce_audio_processors.h>

namespace ODPedalParameters
{
    // parameter IDs
    constexpr auto DRIVE_ID = "drive";
    constexpr auto TONE_ID  = "tone";
    constexpr auto LEVEL_ID = "level";

    // parameter names
    constexpr auto DRIVE_NAME = "Drive";
    constexpr auto TONE_NAME  = "Tone";
    constexpr auto LEVEL_NAME = "Level";

    // APTVS layout
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
}