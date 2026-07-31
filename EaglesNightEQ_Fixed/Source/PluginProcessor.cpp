#include "PluginProcessor.h"
#include "PluginEditor.h"

EaglesNightEQAudioProcessor::EaglesNightEQAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

EaglesNightEQAudioProcessor::~EaglesNightEQAudioProcessor() = default;

juce::AudioProcessorValueTreeState::ParameterLayout EaglesNightEQAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "NOTCHGAIN", "2.3kHz Notch Gain",
        juce::NormalisableRange<float>(-18.0f, 6.0f, 0.1f, 1.0f),
        -6.0f, juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "NOTCHQ", "2.3kHz Notch Q",
        juce::NormalisableRange<float>(0.5f, 10.0f, 0.1f, 0.5f),
        3.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "LOWGAIN", "Low Shelf Gain",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f, 1.0f),
        2.5f, juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "LOMIDGAIN", "Low-Mid Cut Gain",
        juce::NormalisableRange<float>(-12.0f, 6.0f, 0.1f, 1.0f),
        -2.8f, juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "PRESGAIN", "Presence Gain",
        juce::NormalisableRange<float>(-6.0f, 6.0f, 0.1f, 1.0f),
        1.2f, juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "HIGHGAIN", "High Shelf Gain",
        juce::NormalisableRange<float>(-6.0f, 6.0f, 0.1f, 1.0f),
        1.8f, juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "OUTPUTGAIN", "Output Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f, 1.0f),
        0.0f, juce::AudioParameterFloatAttributes().withLabel("dB")));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "MIX", "Eagles Curve Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f, 1.0f),
        100.0f, juce::AudioParameterFloatAttributes().withLabel("%")));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "NOTCHBYPASS", "Notch Bypass", false));

    return { params.begin(), params.end() };
}

void EaglesNightEQAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.sampleRate = sampleRate;
    spec.numChannels = 1;

    leftChain.prepare(spec);
    rightChain.prepare(spec);

    updateFilters();
}

void EaglesNightEQAudioProcessor::releaseResources() {}

bool EaglesNightEQAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    return true;
}

void EaglesNightEQAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    updateFilters();

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto mix = apvts.getRawParameterValue("MIX")->load() / 100.0f;
    auto outputGain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("OUTPUTGAIN")->load());

    juce::AudioBuffer<float> dryBuffer;
    if (mix < 1.0f)
        dryBuffer.makeCopyOf(buffer);

    juce::dsp::AudioBlock<float> block(buffer);

    if (totalNumInputChannels == 1)
    {
        auto monoBlock = block.getSingleChannelBlock(0);
        juce::dsp::ProcessContextReplacing<float> monoContext(monoBlock);
        leftChain.process(monoContext);
    }
    else
    {
        auto leftBlock = block.getSingleChannelBlock(0);
        auto rightBlock = block.getSingleChannelBlock(1);
        juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
        juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
        leftChain.process(leftContext);
        rightChain.process(rightContext);
    }

    if (mix < 1.0f)
    {
        for (int ch = 0; ch < totalNumInputChannels; ++ch)
        {
            auto* wet = buffer.getWritePointer(ch);
            auto* dry = dryBuffer.getReadPointer(ch);
            for (int s = 0; s < buffer.getNumSamples(); ++s)
                wet[s] = dry[s] * (1.0f - mix) + wet[s] * mix;
        }
    }

    buffer.applyGain(outputGain);
}

void EaglesNightEQAudioProcessor::updateFilters()
{
    auto sampleRate = getSampleRate();
    if (sampleRate <= 0) return;

    auto notchGainDb = apvts.getRawParameterValue("NOTCHGAIN")->load();
    auto notchQ = apvts.getRawParameterValue("NOTCHQ")->load();
    auto lowGainDb = apvts.getRawParameterValue("LOWGAIN")->load();
    auto lowMidGainDb = apvts.getRawParameterValue("LOMIDGAIN")->load();
    auto presGainDb = apvts.getRawParameterValue("PRESGAIN")->load();
    auto highGainDb = apvts.getRawParameterValue("HIGHGAIN")->load();
    auto notchBypass = apvts.getRawParameterValue("NOTCHBYPASS")->load() > 0.5f;

    auto lowShelfCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        sampleRate, 120.0f, 0.7f, juce::Decibels::decibelsToGain(lowGainDb));
    updateCoefficients<LowShelf>(lowShelfCoefficients);

    auto lowMidCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, 280.0f, 1.2f, juce::Decibels::decibelsToGain(lowMidGainDb));
    updateCoefficients<LowMidCut>(lowMidCoefficients);

    float notchGain = notchBypass ? 0.0f : notchGainDb;
    auto notchCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, 2300.0f, notchQ, juce::Decibels::decibelsToGain(notchGain));
    updateCoefficients<Notch2300>(notchCoefficients);

    auto presCoefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, 4500.0f, 1.4f, juce::Decibels::decibelsToGain(presGainDb));
    updateCoefficients<Presence>(presCoefficients);

    auto highShelfCoefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 10000.0f, 0.7f, juce::Decibels::decibelsToGain(highGainDb));
    updateCoefficients<HighShelf>(highShelfCoefficients);
}

bool EaglesNightEQAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* EaglesNightEQAudioProcessor::createEditor()
{
    return new EaglesNightEQAudioProcessorEditor(*this);
}

const juce::String EaglesNightEQAudioProcessor::getName() const { return JucePlugin_Name; }
bool EaglesNightEQAudioProcessor::acceptsMidi() const { return false; }
bool EaglesNightEQAudioProcessor::producesMidi() const { return false; }
bool EaglesNightEQAudioProcessor::isMidiEffect() const { return false; }
double EaglesNightEQAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int EaglesNightEQAudioProcessor::getNumPrograms() { return 1; }
int EaglesNightEQAudioProcessor::getCurrentProgram() { return 0; }
void EaglesNightEQAudioProcessor::setCurrentProgram(int) {}
const juce::String EaglesNightEQAudioProcessor::getProgramName(int) { return {}; }
void EaglesNightEQAudioProcessor::changeProgramName(int, const juce::String&) {}

void EaglesNightEQAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void EaglesNightEQAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EaglesNightEQAudioProcessor();
}
