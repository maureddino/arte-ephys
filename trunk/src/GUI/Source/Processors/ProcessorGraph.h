/*
  ==============================================================================

    ProcessorGraph.h
    Created: 30 Apr 2011 8:36:35pm
    Author:  jsiegle

  ==============================================================================
*/

#ifndef __PROCESSORGRAPH_H_124F8B50__
#define __PROCESSORGRAPH_H_124F8B50__

#include "../../JuceLibraryCode/JuceHeader.h"

//#include "../UI/UIComponent.h"

class GenericProcessor;
class RecordNode;
class SourceNode;
class FilterViewport;
class SignalChainTabButton;
class AudioNode;
class UIComponent;
class Configuration;

class ProcessorGraph : public AudioProcessorGraph,
					   public ActionBroadcaster
{
public:
	ProcessorGraph();
	~ProcessorGraph();

	void* createNewProcessor(String& description);//,
							 //GenericProcessor* source,
							 //GenericProcessor* dest);

	GenericProcessor* createProcessorFromDescription(String& description);

	void removeProcessor(GenericProcessor* processor);

	bool enableSourceNodes();
	bool disableSourceNodes();

	RecordNode* getRecordNode();
	GenericProcessor* getSourceNode(int snID);
	AudioNode* getAudioNode();

	void setUIComponent(UIComponent* ui);
	void setFilterViewport(FilterViewport *fv);

	void updateConnections(Array<SignalChainTabButton*>);
	
//	int numSamplesInThisBuffer;

	

	//const CriticalSection lock;

	const String saveState(const File& file);
	const String loadState(const File& file);

	XmlElement* createNodeXml(GenericProcessor*);

private:	

	int currentNodeId;

	Array<int> source_node_IDs;

	const int RECORD_NODE_ID;
	const int AUDIO_NODE_ID;
	const int OUTPUT_NODE_ID;
	const int RESAMPLING_NODE_ID;

	void createDefaultNodes();

	UIComponent* UI;
	FilterViewport* filterViewport;
	Configuration* config;

};



#endif  // __PROCESSORGRAPH_H_124F8B50__
