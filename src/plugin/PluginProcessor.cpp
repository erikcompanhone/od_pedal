# include "PluginProcessor.h"

PluginProcessor::PluginProcessor()
    : juce::AudioProcessor (juce::AudioProcessor::BusesProperties()
                                .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                                .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                            ),
    apvts (*this, nullptr, "OD_PEDAL", createLayout())
{
}

PluginProcessor::~PluginProcessor()
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    dsp.prepare(static_cast<float>(sampleRate));
    DBG("Prepared to play at " << sampleRate << " Hz, block size: " << samplesPerBlock);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // no midi
    juce::ignoreUnused(midiMessages);

    // get pointer to audio data
    float* outputChannelPtr = buffer.getWritePointer(0);
    int numSamples = buffer.getNumSamples();

    // read params
    float drive = apvts.getRawParameterValue(ODPedalParameters::DRIVE_ID)->load();
    float tone = apvts.getRawParameterValue(ODPedalParameters::TONE_ID)->load();
    float level = apvts.getRawParameterValue(ODPedalParameters::LEVEL_ID)->load();

    // call dsp process
    dsp.process(outputChannelPtr, numSamples, drive, tone, level);
}

void PluginProcessor::releaseResources()
{
    dsp.reset();
}

int PluginProcessor::getNumPrograms()
{
    return 1;
}

void PluginProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

const juce::String PluginProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return "Default";
}

void PluginProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName(apvts.state.getType()))
    {
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

const juce::String PluginProcessor::getName() const
{
    return "OD Pedal";
}

bool PluginProcessor::acceptsMidi() const
{
    return false;
}

bool PluginProcessor::producesMidi() const
{
    return false;
}

bool PluginProcessor::isMidiEffect() const
{
    return false;
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createLayout()
{
    return ODPedalParameters::createParameterLayout();
}
