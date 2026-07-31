#include "PluginEditor.h"

EaglesNightEQAudioProcessorEditor::EaglesNightEQAudioProcessorEditor(EaglesNightEQAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Title
    titleLabel.setText("Eagles Night EQ", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFE8D5B7));
    addAndMakeVisible(titleLabel);

    // Notch section label
    notchLabel.setText("2.3kHz Notch", juce::dontSendNotification);
    notchLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    notchLabel.setJustificationType(juce::Justification::centred);
    notchLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFD4A574));
    addAndMakeVisible(notchLabel);

    // Eagles curve label
    eaglesLabel.setText("Eagles Curve", juce::dontSendNotification);
    eaglesLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    eaglesLabel.setJustificationType(juce::Justification::centred);
    eaglesLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFD4A574));
    addAndMakeVisible(eaglesLabel);

    // Output label
    outputLabel.setText("Output", juce::dontSendNotification);
    outputLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFD4A574));
    addAndMakeVisible(outputLabel);

    auto setupSlider = [](juce::Slider& slider, const juce::String& suffix, double rangeStart, double rangeEnd)
    {
        slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xFFD4A574));
        slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFF8B6914));
        slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF3A3020));
        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFE8D5B7));
        slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF1A1510));
        slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF3A3020));
        slider.setRange(rangeStart, rangeEnd, 0.1);
        slider.setTextValueSuffix(suffix);
    };

    // Notch controls
    setupSlider(notchGainSlider, " dB", -18.0, 6.0);
    notchGainSlider.setName("Gain");
    addAndMakeVisible(notchGainSlider);
    notchGainAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "NOTCHGAIN", notchGainSlider);

    setupSlider(notchQSlider, "", 0.5, 10.0);
    notchQSlider.setName("Q");
    addAndMakeVisible(notchQSlider);
    notchQAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "NOTCHQ", notchQSlider);

    notchBypassButton.setButtonText("Bypass");
    notchBypassButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF3A3020));
    notchBypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF8B4513));
    notchBypassButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xFFE8D5B7));
    notchBypassButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    addAndMakeVisible(notchBypassButton);
    notchBypassAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "NOTCHBYPASS", notchBypassButton);

    // Eagles curve controls
    setupSlider(lowGainSlider, " dB", -12.0, 12.0);
    lowGainSlider.setName("Low");
    addAndMakeVisible(lowGainSlider);
    lowGainAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "LOWGAIN", lowGainSlider);

    setupSlider(lowMidGainSlider, " dB", -12.0, 6.0);
    lowMidGainSlider.setName("Low-Mid");
    addAndMakeVisible(lowMidGainSlider);
    lowMidGainAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "LOMIDGAIN", lowMidGainSlider);

    setupSlider(presGainSlider, " dB", -6.0, 6.0);
    presGainSlider.setName("Presence");
    addAndMakeVisible(presGainSlider);
    presGainAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "PRESGAIN", presGainSlider);

    setupSlider(highGainSlider, " dB", -6.0, 6.0);
    highGainSlider.setName("High");
    addAndMakeVisible(highGainSlider);
    highGainAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "HIGHGAIN", highGainSlider);

    // Output controls
    setupSlider(mixSlider, " %", 0.0, 100.0);
    mixSlider.setName("Mix");
    addAndMakeVisible(mixSlider);
    mixAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "MIX", mixSlider);

    setupSlider(outputGainSlider, " dB", -24.0, 24.0);
    outputGainSlider.setName("Gain");
    addAndMakeVisible(outputGainSlider);
    outputGainAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "OUTPUTGAIN", outputGainSlider);

    setSize(700, 400);
    setResizable(false, false);
}

EaglesNightEQAudioProcessorEditor::~EaglesNightEQAudioProcessorEditor()
{
}

void EaglesNightEQAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Vintage dark background - inspired by 70s studio gear
    g.fillAll(juce::Colour(0xFF1A1510));

    // Subtle panel backgrounds
    g.setColour(juce::Colour(0xFF252015));
    g.fillRoundedRectangle(20.0f, 60.0f, 200.0f, 310.0f, 8.0f);  // Notch panel
    g.fillRoundedRectangle(240.0f, 60.0f, 300.0f, 310.0f, 8.0f); // Eagles panel
    g.fillRoundedRectangle(560.0f, 60.0f, 120.0f, 310.0f, 8.0f); // Output panel

    // Draw parameter labels
    g.setColour(juce::Colour(0xFF9A8B7A));
    g.setFont(11.0f);

    auto drawLabel = [&](juce::Slider& slider, const juce::String& text)
    {
        auto bounds = slider.getBounds();
        g.drawFittedText(text, bounds.getX(), bounds.getY() - 18, bounds.getWidth(), 16,
                         juce::Justification::centred, 1);
    };

    drawLabel(notchGainSlider, "GAIN");
    drawLabel(notchQSlider, "Q");
    drawLabel(lowGainSlider, "LOW");
    drawLabel(lowMidGainSlider, "LOW-MID");
    drawLabel(presGainSlider, "PRESENCE");
    drawLabel(highGainSlider, "HIGH");
    drawLabel(mixSlider, "MIX");
    drawLabel(outputGainSlider, "GAIN");
}

void EaglesNightEQAudioProcessorEditor::resized()
{
    titleLabel.setBounds(0, 10, getWidth(), 35);

    // Notch section
    notchLabel.setBounds(30, 65, 180, 20);
    notchGainSlider.setBounds(40, 100, 80, 80);
    notchQSlider.setBounds(130, 100, 80, 80);
    notchBypassButton.setBounds(60, 200, 120, 28);

    // Eagles curve section
    eaglesLabel.setBounds(260, 65, 260, 20);
    lowGainSlider.setBounds(260, 100, 80, 80);
    lowMidGainSlider.setBounds(350, 100, 80, 80);
    presGainSlider.setBounds(440, 100, 80, 80);
    highGainSlider.setBounds(260, 220, 80, 80);
    mixSlider.setBounds(350, 220, 80, 80);

    // Output section
    outputLabel.setBounds(570, 65, 100, 20);
    outputGainSlider.setBounds(580, 100, 80, 80);

    // Add a descriptive text area at the bottom
    auto descBounds = juce::Rectangle<int>(40, 320, 620, 50);
}
