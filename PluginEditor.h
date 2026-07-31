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

    juce::Slider notchGainSlider;
    juce::Slider notchQSlider;
    juce::Slider lowGainSlider;
    juce::Slider lowMidGainSlider;
    juce::Slider presGainSlider;
    juce::Slider highGainSlider;
    juce::Slider outputGainSlider;
    juce::Slider mixSlider;

    juce::ToggleButton notchBypassButton;

    std::unique_ptr<Attachment> notchGainAttachment;
    std::unique_ptr<Attachment> notchQAttachment;
    std::unique_ptr<Attachment> lowGainAttachment;
    std::unique_ptr<Attachment> lowMidGainAttachment;
    std::unique_ptr<Attachment> presGainAttachment;
    std::unique_ptr<Attachment> highGainAttachment;
    std::unique_ptr<Attachment> outputGainAttachment;
    std::unique_ptr<Attachment> mixAttachment;

    std::unique_ptr<ButtonAttachment> notchBypassAttachment;

    juce::Label titleLabel;
    juce::Label notchLabel;
    juce::Label eaglesLabel;
    juce::Label outputLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EaglesNightEQAudioProcessorEditor)
};
