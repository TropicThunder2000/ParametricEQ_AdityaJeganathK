/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
//Author : Aditya Jeganath K
// 04/11/24


#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ParametricEQ : public juce::AudioProcessor
{
public:
    //==============================================================================
    ParametricEQ();
    ~ParametricEQ() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void updateCoefficients();
    void resetDelays();
    void setSamplerate(double sampleRate);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParametricEQ)

        /*
            Declare Audio Parameters Variables
        */
        juce::AudioParameterFloat* gainKnobParam;
        juce::AudioParameterFloat* fCenterSliderParam;
        juce::AudioParameterFloat* qSliderParam;
        juce::AudioParameterFloat* wetKnobParam;
        juce::AudioParameterFloat* dryKnobParam;

        float a0, a1, a2, b1, b2, d0, c0;
        juce::Array<float> a1_delay, a2_delay, b1_delay, b2_delay;
        double fs;
        float currentGain, currentQ, currentCF, currentWet, currentDry;
    /*
        Declare Effect Classes
    */
    
};
