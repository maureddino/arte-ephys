/*
  ==============================================================================

    SignalGenerator.cpp
    Created: 9 May 2011 8:11:41pm
    Author:  jsiegle

  ==============================================================================
*/


#include "SignalGenerator.h"
//#include "SourceNodeEditor.h"
#include <stdio.h>

SignalGenerator::SignalGenerator()
	: frequency(10.0),
	  sampleRate (44100.0),
	  currentPhase (0.0),
	  phasePerSample (0.0),
	  amplitude (0.5f),
	  name (T("Sine wave source"))
	
{

	setPlayConfigDetails(0, // numInputChannels
						 16, // numOutputChannels
						 44100.0, // expected sample rate
						 128); // expected number of samples per buffer

}

SignalGenerator::~SignalGenerator()
{
}


//AudioProcessorEditor* SignalGenerator::createEditor( )
//{
	//filterEditor = new FilterEditor(this);
	
//	std::cout << "Creating editor." << std::endl;
//	sourceEditor = new SourceNodeEditor(this);
//	return sourceEditor;
//}

//AudioProcessorEditor* FilterNode::createEditor(AudioProcessorEditor* const editor)
//{
	
//	return editor;
//}
void SignalGenerator::setParameter (int parameterIndex, float newValue)
{
	//std::cout << "Message received." << std::endl;
	frequency = newValue;
	phasePerSample = double_Pi * 2.0 / (sampleRate / frequency);

}

void SignalGenerator::prepareToPlay (double sampleRate_, int estimatedSamplesPerBlock)
{
	sampleRate = sampleRate_;
	phasePerSample = double_Pi * 2.0 / (sampleRate / frequency);
	//std::cout << "Prepare to play: " << std::endl;
}

void SignalGenerator::releaseResources() 
{	
}

void SignalGenerator::processBlock (AudioSampleBuffer &buffer, MidiBuffer &midiMessages)
{

	//std::cout << buffer.getNumChannels() << std::endl;
	
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        const float sample = amplitude * (float) std::sin (currentPhase);
        currentPhase += phasePerSample;

        for (int j = buffer.getNumChannels(); --j >= 0;)
        	// dereference pointer to one of the buffer's samples
            *buffer.getSampleData (j, i) = sample;
    }
}
