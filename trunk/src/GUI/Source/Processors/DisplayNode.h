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
#include "Editors/Visualizer.h"
#include "GenericProcessor.h"

class DataViewport;

class DisplayNode : public GenericProcessor

{
public:
	// real member functions:
	DisplayNode(const String name, int* nSamples, 
		int nChans, const CriticalSection& lock, int nodeId,
		DataViewport* dv);
	~DisplayNode();

	AudioProcessorEditor* createEditor();

	bool isSink() {return true;}

	void setDestNode(GenericProcessor* sn) {destNode = 0;}
	
private:

	DataViewport* dataViewport;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DisplayNode);

};



#endif  // __DISPLAYNODE_H_C7B28CA4__
