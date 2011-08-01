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

SignalGenerator::SignalGenerator(const String name_, int* nSamps, int nChans, const CriticalSection& lock_, int id)
	: GenericProcessor(name_, nSamps, nChans, lock_, id),
	  frequency(25.0),
	  sampleRate (44100.0),
	  currentPhase (0.0),
	  phasePerSample (0.0),
	  amplitude (0.02f)
	
{

	setNumOutputs(16);
	setNumInputs(0);

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

void SignalGenerator::enable () {
	
	std::cout << "Signal generator received enable signal." << std::endl;

}

void SignalGenerator::disable() {
	
	std::cout << "Signal generator received disable signal." << std::endl;

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
