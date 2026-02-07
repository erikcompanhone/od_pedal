# include "PluginParameters.h"

juce::AudioProcessorValueTreeState::ParameterLayout ODPedalParameters::createParameterLayout()
{
    // create the parameter layout and add parameters
    return juce::AudioProcessorValueTreeState::ParameterLayout {
        std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { DRIVE_ID, 1 },
            DRIVE_NAME,
            juce::NormalisableRange<float> { 0.0f, 24.0f, 0.1f, 0.4f }, 0.0f,
            juce::AudioParameterFloatAttributes().withLabel ("dB")
        ),
        std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { TONE_ID, 1 },
            TONE_NAME,
            juce::NormalisableRange<float> { 800.0f, 8000.0f, 1.0f, 0.35f }, 3000.0f,
            juce::AudioParameterFloatAttributes().withLabel ("Hz")
        ),
        std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID { LEVEL_ID, 1 },
            LEVEL_NAME,
            juce::NormalisableRange<float> { -12.0f, 12.0f, 0.1f, 0.5f }, 0.0f,
            juce::AudioParameterFloatAttributes().withLabel ("dB")
        ),
        std::make_unique<juce::AudioParameterBool> (
            juce::ParameterID { BYPASS_ID, 1 },
            BYPASS_NAME,
            false
        )
    };
}
