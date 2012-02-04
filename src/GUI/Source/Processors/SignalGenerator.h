/*
  ==============================================================================

    SignalGenerator.h
    Created: 9 May 2011 8:11:41pm
    Author:  jsiegle

  ==============================================================================
*/

#ifndef __SIGNALGENERATOR_H_EAA44B0B__
#define __SIGNALGENERATOR_H_EAA44B0B__


#include "../../JuceLibraryCode/JuceHeader.h"
#include "GenericProcessor.h"

//class SourceNodeEditor;

class SignalGenerator : public GenericProcessor

{
public:
	
	// real member functions:
	SignalGenerator();
	~SignalGenerator();
	
	void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
	void releaseResources();
	void process(AudioSampleBuffer &buffer, MidiBuffer &midiMessages, int& nSamples);

	void setParameter (int parameterIndex, float newValue);

	//AudioProcessorEditor* createEditor();
	bool hasEditor() const {return true;}

	void enable();
	void disable();

	bool isSource() {return true;}

private:
	double frequency, sampleRate;
	double currentPhase, phasePerSample;
	float amplitude;
	//SourceNodeEditor* sourceEditor;
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SignalGenerator);

};





#endif  // __SIGNALGENERATOR_H_EAA44B0B__
