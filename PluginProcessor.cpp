/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

//Author : Aditya Jeganath K
// 04/11/24

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
 ParametricEQ::ParametricEQ()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    /*
        Initialize Audio Parameters Variables
    */
    gainKnobParam = new juce::AudioParameterFloat(juce::ParameterID("1",1), "Gain", -24.f, 5.f, 0.f);
  
    fCenterSliderParam = new juce::AudioParameterFloat(juce::ParameterID("2",2), "Center Frequency", 1, 12000, 2000);

    qSliderParam = new juce::AudioParameterFloat(juce::ParameterID("3",3), "Q Factor", 0.f, 2.f, 1.f);

    wetKnobParam = new juce::AudioParameterFloat(juce::ParameterID("4", 4), "Wet", -24.f, 5.f, 0.f);

    dryKnobParam = new juce::AudioParameterFloat(juce::ParameterID("5", 5), "Dry", 0.0f, 1.0f, 1.0f);

    
    /*
        Add Audio Parameters to Parameter Tree
    */
    addParameter(gainKnobParam);
    addParameter(fCenterSliderParam);
    addParameter(qSliderParam);
    addParameter(wetKnobParam);
    addParameter(dryKnobParam);

    /*
        Initialize any properties of effect classes
    */


    //Reset Delays

    auto numInputChannels = getTotalNumInputChannels();
    for (int i = 0; i < numInputChannels; i++)
    {
        a1_delay.add(0.f);
        a2_delay.add(0.f);
        b1_delay.add(0.f);
        b2_delay.add(0.f);
    }

}

 ParametricEQ::~ParametricEQ()
{
}

//==============================================================================
const juce::String ParametricEQ::getName() const
{
    return JucePlugin_Name;
}

bool ParametricEQ::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ParametricEQ::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ParametricEQ::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ParametricEQ::getTailLengthSeconds() const
{
    return 0.0;
}

int ParametricEQ::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ParametricEQ::getCurrentProgram()
{
    return 0;
}

void ParametricEQ::setCurrentProgram (int index)
{
}

const juce::String ParametricEQ::getProgramName (int index)
{
    return {};
}

void ParametricEQ::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ParametricEQ::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    /*
        Initialize any variables or effect class properties
    */ 
    setSamplerate(sampleRate);
    resetDelays();
}

void ParametricEQ::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ParametricEQ::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ParametricEQ::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float wet = *wetKnobParam;
    float dry = *dryKnobParam;
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
  
        // This is the place where you'd normally do the guts of your plugin's
        // audio processing...
        // Make sure to reset the state if your inner loop is processing
        // the samples and the outer loop is handling the channels.
        // Alternatively, you can process the samples with the channels
        // interleaved by keeping the same state.

    if (currentGain != *gainKnobParam || currentCF != *fCenterSliderParam || currentQ != *qSliderParam  || currentWet != *wetKnobParam|| currentDry != *dryKnobParam)  {
        updateCoefficients();

      /*  float gainValue = *gainKnobParam;
        float centerfreqValue = *fCenterSliderParam;
        float qValue = *qSliderParam;
        */
        }


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

       
       for (int channel = 0; channel < totalNumInputChannels; ++channel)
       {
                auto *outchannelData = buffer.getWritePointer(channel);
                auto *inchannelData = buffer.getReadPointer(channel);

            for (int n = 0; n < buffer.getNumSamples(); n++)
            {
                float z = a0 * inchannelData[n] +(a1 * a1_delay[channel] 
                                                + a2 * a2_delay[channel] 
                                                - b1 * b1_delay[channel]
                                                - b2 * b2_delay[channel]);

                a2_delay.set(channel, a1_delay[channel]);
                a1_delay.set(channel, inchannelData[n]);
                b2_delay.set(channel, b1_delay[channel]);
                b1_delay.set(channel, z);

                outchannelData[n] = dry * inchannelData[n] +  wet * z;
            }
            
      }
    
}

//==============================================================================
bool ParametricEQ::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ParametricEQ::createEditor()
{
    return new ParametricEQEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void ParametricEQ::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ParametricEQ::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ParametricEQ();
}


void ParametricEQ::updateCoefficients()
{
    float gainValue = *gainKnobParam;
    float centerfreqValue = *fCenterSliderParam;
    float qValue = *qSliderParam;
    
    float Theta = 2.0f * 3.14f * (centerfreqValue / fs);
    float Mu = pow(10.0f, gainValue / 20.0f);
    float Zeta = 1.0f / 4.0f + Mu;
    float Beta = 0.5f * ((1.0f - Zeta * tan(Theta / 2.0f * qValue)) / (1.0f + Zeta * tan(Theta / 2.0f * qValue)));
    float Gama = (0.5f + Beta) * cos(Theta);

    //d0 = 1;
    //c0 = Mu - 1;

    a0 = 0.5 - Beta;
    a1 = 0;
    a2 = -a0;
    b1 = (-2) * Gama;
    b2 = 2 * Beta;
}

void ParametricEQ::resetDelays()
{

        auto numInputChannels = getTotalNumInputChannels();

        for (int i = 0; i < numInputChannels; i++)
        {
            a1_delay.set(i, 0.0f);
            a2_delay.set(i, 0.0f);
            b1_delay.set(i, 0.0f);
            b2_delay.set(i, 0.0f);
        }
    
}

void ParametricEQ::setSamplerate(double sampleRate) {

    fs = sampleRate;
    updateCoefficients();
}