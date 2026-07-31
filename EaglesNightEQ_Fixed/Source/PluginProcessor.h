#pragma once
#include <JuceHeader.h>

class EaglesNightEQAudioProcessor : public juce::AudioProcessor
{
public:
    EaglesNightEQAudioProcessor();
    ~EaglesNightEQAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    using Filter = juce::dsp::IIR::Filter<float>;
    using MonoChain = juce::dsp::ProcessorChain<
        Filter, Filter, Filter, Filter, Filter>;

    MonoChain leftChain, rightChain;

    enum ChainPositions { LowShelf = 0, LowMidCut, Notch2300, Presence, HighShelf };

    template<int Index>
    void updateCoefficients(const Filter::CoefficientsPtr& coefficients)
    {
        *leftChain.get<Index>().coefficients = *coefficients;
        *rightChain.get<Index>().coefficients = *coefficients;
    }

    void updateFilters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EaglesNightEQAudioProcessor)
};
