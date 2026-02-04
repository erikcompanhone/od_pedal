# include "PluginProcessor.h"
# include "PluginEditor.h"

PluginProcessor::PluginProcessor()
    : juce::AudioProcessor (juce::AudioProcessor::BusesProperties()
                                .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                                .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                            ),
    apvts (*this, nullptr, "OD_PEDAL", createLayout())
{
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    dsp.prepare(static_cast<float>(sampleRate));
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // no midi
    juce::ignoreUnused(midiMessages);

    // get pointer to audio data
    float* outputChannelPtr = buffer.getWritePointer(0);
    int numSamples = buffer.getNumSamples();

    // read bypass parameter
    bool isBypassed = apvts.getRawParameterValue(ODPedalParameters::BYPASS_ID)->load() > 0.5f;
    if (isBypassed)
        return;

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

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // only support mono in/out
    if (layouts.inputBuses.size() != 1 || layouts.outputBuses.size() != 1)
        return false;
    
    return layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono();
}

bool PluginProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0; // no reverb tail
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createLayout()
{
    return ODPedalParameters::createParameterLayout();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
