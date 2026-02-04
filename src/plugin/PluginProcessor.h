# pragma once

# include <juce_audio_processors/juce_audio_processors.h>
# include "../dsp/OverdriveDSP.h"
# include "PluginParameters.h"

// forward declaration
class PluginEditor;

class PluginProcessor : public juce::AudioProcessor
{
    public:
        // params
        juce::AudioProcessorValueTreeState apvts;

        // constructor and destructor
        PluginProcessor();
        ~PluginProcessor() override = default;

        // called before playback starts
        void prepareToPlay(double sampleRate, int samplesPerBlock) override;
        
        // called repeatedly during playback
        void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    
        // called when playback stops
        void releaseResources() override;

        // program overrides for presets
        int getNumPrograms() override;
        void setCurrentProgram(int index) override;
        int getCurrentProgram() override;
        const juce::String getProgramName(int index) override;
        void changeProgramName(int index, const juce::String& newName) override;

        // state saving and loading for DAW sessions
        void getStateInformation(juce::MemoryBlock& destData) override;
        void setStateInformation(const void* data, int sizeInBytes) override;
        
        // plugin info
        const juce::String getName() const override;
        bool acceptsMidi() const override;
        bool producesMidi() const override;
        bool isMidiEffect() const override;

        // bus layout
        bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

        // editor
        bool hasEditor() const override;
        juce::AudioProcessorEditor* createEditor() override;

        // tail
        double getTailLengthSeconds() const override;

    private:
        // DSP instance
        OverdriveDSP dsp;

        // helper to update cached params
        juce::AudioProcessorValueTreeState::ParameterLayout createLayout();
};
