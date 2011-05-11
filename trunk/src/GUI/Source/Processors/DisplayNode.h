/*
  ==============================================================================

    DisplayNode.h
    Created: 7 May 2011 5:07:43pm
    Author:  jsiegle

  ==============================================================================
*/

#ifndef __DISPLAYNODE_H_C7B28CA4__
#define __DISPLAYNODE_H_C7B28CA4__


#include "../../JuceLibraryCode/JuceHeader.h"
#include "../UI/StreamViewer.h"

class StreamViewer;

class DisplayNode : public AudioProcessor

{
public:
	
	// real member functions:
	DisplayNode(const String name_, int* nSamps, const CriticalSection& lock);;
	~DisplayNode();

	const String getName() const {return name;}
	
	void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
	void releaseResources();
	void processBlock (AudioSampleBuffer &buffer, MidiBuffer &midiMessages);
	void setParameter (int parameterIndex, float newValue);

	StreamViewer* createDisplay();
	AudioProcessorEditor* createEditor( ) {return 0;}
	bool hasEditor() const {return false;}
	//AudioProcessorEditor* createEditor(AudioProcessorEditor* editor);
	
	// end real member functions
	
	// quick implementations of virtual functions, to be changed later:
	void reset() {}
	void setCurrentProgramStateInformation(const void* data, int sizeInBytes) {}
	void setStateInformation(const void* data, int sizeInBytes) {}
	void getCurrentProgramStateInformation(MemoryBlock &destData) {}
	void getStateInformation (MemoryBlock &destData) {}
	void changeProgramName (int index, const String &newName) {}
	void setCurrentProgram (int index) {}

	

	const String getInputChannelName (int channelIndex) const {return T(" ");}
	const String getOutputChannelName (int channelIndex) const {return T(" ");}
	const String getParameterName (int parameterIndex) {return T(" ");}
	const String getParameterText (int parameterIndex) {return T(" ");}
	const String getProgramName (int index) {return T(" ");}
	
	bool isInputChannelStereoPair (int index) const {return true;}
	bool isOutputChannelStereoPair (int index) const {return true;}
	bool acceptsMidi () const {return false;}
	bool producesMidi () const {return false;}

	bool isParameterAutomatable(int parameterIndex) {return false;}
	bool isMetaParameter(int parameterIndex) {return false;}
	
	int getNumParameters() {return 0;}
	int getNumPrograms() {return 0;}
	int getCurrentProgram() {return 0;}
	
	float getParameter (int parameterIndex) {return 1.0;}

	
private:
	const String name;
	double sampleRate;
	double refreshRate;
	float displayTimebase;

	AudioSampleBuffer* displayBuffer;
	
	int maxBufferSize;
	int samplesPerPixel;

	int* numSamplesInThisBuffer;
	const CriticalSection& lock;

	int accumulator;
	float pixelValue;
	int bufferIndex;

	StreamViewer* streamViewer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DisplayNode);

};



#endif  // __DISPLAYNODE_H_C7B28CA4__
