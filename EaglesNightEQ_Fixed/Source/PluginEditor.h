#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class EaglesNightEQAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    EaglesNightEQAudioProcessorEditor(EaglesNightEQAudioProcessor&);
    ~EaglesNightEQAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    EaglesNightEQAudioProcessor& audioProcessor;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;
    using ButtonAttachment = APVTS::ButtonAttachment;

    juce::Slider notchGainSlider, notchQSlider, lowGainSlider, lowMidGainSlider;
    juce::Slider presGainSlider, highGainSlider, outputGainSlider, mixSlider;
    juce::ToggleButton notchBypassButton;

    std::unique_ptr<Attachment> notchGainAttachment, notchQAttachment;
    std::unique_ptr<Attachment> lowGainAttachment, lowMidGainAttachment;
    std::unique_ptr<Attachment> presGainAttachment, highGainAttachment;
    std::unique_ptr<Attachment> outputGainAttachment, mixAttachment;
    std::unique_ptr<ButtonAttachment> notchBypassAttachment;

    juce::Label titleLabel, notchLabel, eaglesLabel, outputLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EaglesNightEQAudioProcessorEditor)
};
