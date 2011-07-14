/*
  ==============================================================================

    EventNode.h
    Created: 13 Jun 2011 10:42:26am
    Author:  jsiegle

  ==============================================================================
*/

#ifndef __EVENTNODE_H_9B67A789__
#define __EVENTNODE_H_9B67A789__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "GenericProcessor.h"
//#include "Editors/FilterEditor.h"

class FilterViewport;

class EventNode : public GenericProcessor

{
public:
	
	EventNode(const String name, int* nSamples, int nChans, const CriticalSection& lock, int nodeId, bool src);
	~EventNode();
	
	void prepareToPlay (double sampleRate, int estimatedSamplesPerBlock);
	void releaseResources();
	void processBlock (AudioSampleBuffer &buffer, MidiBuffer &midiMessages);
	void setParameter (int parameterIndex, float newValue);

	void enable();
	void disable();

	// AudioProcessorEditor* createEditor();
	
private:

	int accumulator;
	bool isSource;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EventNode);

};






#endif  // __EVENTNODE_H_9B67A789__
