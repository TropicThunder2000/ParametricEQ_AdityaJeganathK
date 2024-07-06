/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

//Author : Aditya Jeganath K
// 04/11/24

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ParametricEQEditor  : public juce::AudioProcessorEditor
{
public:
    ParametricEQEditor (ParametricEQ&);
    ~ParametricEQEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ParametricEQ& audioProcessor;
    juce::Slider gainKnob;
    juce::Slider fcenterSlider;
    juce::Slider qSlider;
    juce::Slider wetKnob;
    juce::Slider dryKnob;
    juce::Label gainLabel;
    juce::Label wetLabel;
    juce::Label fcenterLabel;
    juce::Label qLabel;
    juce::Label pluginTitle;
    juce::Label dryLabel;
    juce::AudioParameterFloat* gainKnobParam;
    juce::AudioParameterFloat* fCenterSliderParam;
    juce::AudioParameterFloat* qSliderParam;
    juce::AudioParameterFloat* wetKnobParam;
    juce::AudioParameterFloat* dryKnobParam;
 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParametricEQEditor)
};
