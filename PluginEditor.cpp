/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

//Author : Aditya Jeganath K
// 04/11/24

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ParametricEQEditor::ParametricEQEditor (ParametricEQ& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 300);
        /*
        The constructor of the AudioProcessorEditor class is used to define the properties of the GUI components that you want to add in
        These components can include Sliders, Knobs, TextBoxes, SubWindows etc.
        */
        /*
        Step 1 : You have to push your GUI components onto the Component Tree and make them visible
        To do that we use the addAndMakeVisible() function
         */


    addAndMakeVisible(gainKnob);
    addAndMakeVisible(fcenterSlider);
    addAndMakeVisible(qSlider);
    addAndMakeVisible(wetKnob);
    addAndMakeVisible(dryKnob);

    addAndMakeVisible(gainLabel);
    addAndMakeVisible(fcenterLabel);
    addAndMakeVisible(qLabel);
    addAndMakeVisible(wetLabel);
    addAndMakeVisible(dryLabel);

    addAndMakeVisible(pluginTitle);
    
    /*
        Step 2: Use the functions defined for each GUI component class to stylise your components in the way you want it to be
    */
    /****************************************************************************/
    /****************************************************************************/
   
     /*
        Now for our Sliders and Knobs, we need to set their ranges. These ranges are dependent on the audio parameters we defined in the AudioProcessor Class
        In the AudioProcessorEditor class, we have a pointer to our AudioProcessor class. 
        We will use that pointer to get information of all the parameters declared in the AudioProcessor class
    */


    gainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    gainKnob.setColour(juce::Slider::thumbColourId, juce::Colour(211, 198, 88));
    gainKnob.setColour(juce::Slider::rotarySliderFillColourId,juce::Colour(191, 167, 93));
    gainKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(34, 34, 34));
    gainKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(191, 167, 93));

    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainKnob, false);
    gainLabel.setColour(juce::Label::textColourId, juce::Colour(191, 167, 93));
    gainLabel.setJustificationType(juce::Justification::centredBottom);
    
   
    fcenterSlider.setSliderStyle(juce::Slider::LinearVertical);
    fcenterSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    fcenterSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(191, 167, 93));
    fcenterSlider.setColour(juce::Slider::thumbColourId, juce::Colour(211, 198, 88));
    fcenterSlider.setColour(juce::Slider::trackColourId, juce::Colour(34, 34, 34));
    fcenterSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(191, 167, 93));
  

    fcenterLabel.setText("Freq", juce::dontSendNotification);
    fcenterLabel.attachToComponent(&fcenterSlider, false);
    fcenterLabel.setColour(juce::Label::textColourId, juce::Colour(191, 167, 93));
    fcenterLabel.setJustificationType(juce::Justification::centredBottom);


    qSlider.setSliderStyle(juce::Slider::LinearVertical);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    qSlider.setColour(juce::Slider::backgroundColourId, juce::Colour(191, 167, 93));
    qSlider.setColour(juce::Slider::thumbColourId, juce::Colour(211, 198, 88));
    qSlider.setColour(juce::Slider::trackColourId, juce::Colour(34, 34, 34));
    qSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(191, 167, 93));

    qLabel.setText("Q", juce::dontSendNotification);
    qLabel.attachToComponent(&qSlider, false);
    qLabel.setColour(juce::Label::textColourId, juce::Colour(191, 167, 93));
    qLabel.setJustificationType(juce::Justification::centredBottom);

    
    wetKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    wetKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    wetKnob.setColour(juce::Slider::thumbColourId, juce::Colour(211, 198, 88));
    wetKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(191, 167, 93));
    wetKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(34, 34, 34));
    wetKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(191, 167, 93));

    wetLabel.setText("Wet Control", juce::dontSendNotification);
    wetLabel.attachToComponent(&wetKnob, false);
    wetLabel.setColour(juce::Label::textColourId, juce::Colour(191, 167, 93));
    wetLabel.setJustificationType(juce::Justification::centredBottom);

    
    dryKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dryKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    dryKnob.setColour(juce::Slider::thumbColourId, juce::Colour(211, 198, 88));
    dryKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(191, 167, 93));
    dryKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(34, 34, 34));
    dryKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(191, 167, 93));

    dryLabel.setText("Dry Control", juce::dontSendNotification);
    dryLabel.attachToComponent(&dryKnob, false);
    dryLabel.setColour(juce::Label::textColourId, juce::Colour(191, 167, 93));
    dryLabel.setJustificationType(juce::Justification::centredBottom);

    
    pluginTitle.setText("The Parametric EQ", juce::dontSendNotification);
    pluginTitle.setColour(juce::Label::textColourId, juce::Colour(191, 167, 93));

    juce::Font titleFont;
    titleFont.setHeight(20.0f);
    titleFont = titleFont.withStyle(juce::Font::italic);
    pluginTitle.setFont(titleFont);

    /*
        Step 3: Use the getParameters() function to get the audio parameter tree
    */

    auto parameterTree = audioProcessor.getParameters();

    
    /*
        Step 4: Use the function getUnchecked() to get the pointer to the specific parameter for your GUIs and we cast it to a AudioParameterFloat
    */

    gainKnobParam = (juce::AudioParameterFloat*)parameterTree.getUnchecked(0);
    fCenterSliderParam = (juce::AudioParameterFloat*)parameterTree.getUnchecked(1);
    qSliderParam = (juce::AudioParameterFloat*)parameterTree.getUnchecked(2);
    wetKnobParam = (juce::AudioParameterFloat*)parameterTree.getUnchecked(3);
    dryKnobParam = (juce::AudioParameterFloat*)parameterTree.getUnchecked(4);
    

    /*
        Step 5: Use the function setRange() to set the Ranges of GUIs
    */

    gainKnob.setRange(gainKnobParam->range.start, gainKnobParam->range.end, gainKnobParam->range.interval);
    fcenterSlider.setRange(fCenterSliderParam->range.start, fCenterSliderParam->range.end, 1);
    qSlider.setRange(qSliderParam->range.start, qSliderParam->range.end, qSliderParam->range.interval);
    wetKnob.setRange(wetKnobParam->range.start, wetKnobParam->range.end, wetKnobParam->range.interval);
    dryKnob.setRange(dryKnobParam->range.start, dryKnobParam->range.end, dryKnobParam->range.interval);

    /*
        Step 6: Set the default Value of the slider and knob
    */
    gainKnob.setValue(0, juce::dontSendNotification);
    fcenterSlider.setValue(2000, juce::dontSendNotification);
    qSlider.setValue(1.f, juce::dontSendNotification);
    wetKnob.setValue(0, juce::dontSendNotification);
    dryKnob.setValue(1, juce::dontSendNotification);

    /*
        Step 7 : Use the callback function to setValue of the audio processor parameters 
    */

    gainKnob.onValueChange = [this]
        {
            *gainKnobParam = gainKnob.getValue();
        };

    fcenterSlider.onValueChange = [this]
        {
            *fCenterSliderParam = fcenterSlider.getValue();
        };

    qSlider.onValueChange = [this]
        {
            *qSliderParam = qSlider.getValue();
        };

    wetKnob.onValueChange = [this]
        {
            *wetKnobParam= wetKnob.getValue();
        };

    dryKnob.onValueChange = [this]
        {
            *dryKnobParam = dryKnob.getValue();
        };
}

ParametricEQEditor::~ParametricEQEditor()
{
}

//==============================================================================
void ParametricEQEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(10,28,41));
  
}

void ParametricEQEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    /*
        Code written in this function is used to position and resize the GUI components on your window
    */

    /*
        Dividing the GUI window into a grid
    */
    
    /*
        Use the setBounds() function to position the GUI Components (x,y) and set the width and height (width,height)
    */


    auto x = getWidth() / 10;
    auto y = getHeight() / 10;

    gainKnob.setBounds(-20, 3*y, 3 * x, 5 * y);
    fcenterSlider.setBounds(1 * x, 3*y, 5 * x, 5 * y);
    qSlider.setBounds(3 * x, 3*y, 5 * x, 5 * y);
    wetKnob.setBounds(4.6 * x, 3 * y, 5 * x, 5 *y);
    dryKnob.setBounds(7 * x, 3 * y, 5 * x, 5 * y);

    pluginTitle.setBounds(4 * x, 0, 2 * x, 2 * y);

   
}
