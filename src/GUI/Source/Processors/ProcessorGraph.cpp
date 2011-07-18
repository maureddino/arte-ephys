/*
  ==============================================================================

    ProcessorGraph.cpp
    Created: 30 Apr 2011 8:36:35pm
    Author:  jsiegle

  ==============================================================================
*/

#include "ProcessorGraph.h"
#include "SourceNode.h"
#include "FilterNode.h"
#include "GenericProcessor.h"
#include "AudioNode.h"
#include "ResamplingNode.h"
#include "SignalGenerator.h"
#include "RecordNode.h"
#include "EventNode.h"
#include "DisplayNode.h"

#include <stdio.h>

ProcessorGraph::ProcessorGraph(int numChannels) : currentNodeId(100), lastNodeId(1), 
	SOURCE_NODE_ID(0), 
	RECORD_NODE_ID(199), 
	AUDIO_NODE_ID(10), 
	OUTPUT_NODE_ID(201), 
	RESAMPLING_NODE_ID(202),
	numSamplesInThisBuffer(1024)
	
	{

	setPlayConfigDetails(0,2,44100.0, 128);

	createDefaultNodes();

}

ProcessorGraph::~ProcessorGraph() { }


void ProcessorGraph::createDefaultNodes()
{

	// add output node
	AudioProcessorGraph::AudioGraphIOProcessor* on = 
		new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);

	// add record node
	RecordNode* recn = new RecordNode(T("Record Node"), &numSamplesInThisBuffer, 1024, lock, RECORD_NODE_ID);

	// add audio node
	AudioNode* an = new AudioNode(T("Audio Node"), &numSamplesInThisBuffer, 1024, lock, AUDIO_NODE_ID);

	// add resampling node
	ResamplingNode* rn = new ResamplingNode(T("Resampling Node"), &numSamplesInThisBuffer, 2, lock, RESAMPLING_NODE_ID, true);

	addNode(on,OUTPUT_NODE_ID);
	addNode(recn,RECORD_NODE_ID);
	addNode(an, AUDIO_NODE_ID);
	addNode(rn, RESAMPLING_NODE_ID);

	// connect audio network
	for (int n = 0; n < 2; n++) {
		
		addConnection(AUDIO_NODE_ID, n,
		              RESAMPLING_NODE_ID, n);
		
		addConnection(RESAMPLING_NODE_ID, n,
		              OUTPUT_NODE_ID, n);

	}

	std::cout << "Default nodes created." << std::endl;
	
}


void* ProcessorGraph::createNewProcessor(String& description,
										 FilterViewport* vp,
										 GenericProcessor* source,
										 GenericProcessor* dest) {

	int splitPoint = description.indexOf("/");
	String processorType = description.substring(0,splitPoint);
	String subProcessorType = description.substring(splitPoint+1);

	std::cout << processorType << "::" << subProcessorType << std::endl;

	GenericProcessor* processor = 0;

	int id = ++currentNodeId;
	bool connectToAudioAndRecordNodes = false;

	if (processorType.equalsIgnoreCase("Data Sources")) {

		std::cout << "Creating a new data source." << std::endl;
		processor = new SourceNode(description, &numSamplesInThisBuffer, 16, lock, id);
		processor->setDestNode(dest);
		
		if (dest != 0)
			dest->setSourceNode(processor);

		SOURCE_NODE_ID = id;

		connectToAudioAndRecordNodes = true;

	} else if (processorType.equalsIgnoreCase("Filters")) {

		if (subProcessorType.equalsIgnoreCase("Bandpass Filter")) {
			std::cout << "Creating a new filter." << std::endl;
			processor = new FilterNode(description, &numSamplesInThisBuffer, getSourceNode()->getNumOutputs(), lock, id);

		} else if (subProcessorType.equalsIgnoreCase("Resampler")) {
			std::cout << "Creating a new resampler." << std::endl;
			processor = new ResamplingNode(description, &numSamplesInThisBuffer, getSourceNode()->getNumOutputs(), lock, id, false);
		} 

		processor->setSourceNode(source);
		processor->setDestNode(dest);

		processor->setNumInputs(getSourceNode()->getNumOutputs());
		processor->setNumOutputs(getSourceNode()->getNumOutputs());

		if (dest != 0)
			dest->setSourceNode(processor);
		
		if (source != 0)
			source->setDestNode(processor);

		 connectToAudioAndRecordNodes = true;

	} else if (processorType.equalsIgnoreCase("Utilities")) {
		
		if (subProcessorType.equalsIgnoreCase("Event Node")) {
			
			if (SOURCE_NODE_ID == 0) {
				SOURCE_NODE_ID = id;
				std::cout << "Creating a new event source." << std::endl;
				processor = new EventNode(description, &numSamplesInThisBuffer, 2, lock, SOURCE_NODE_ID, true);
			} else {
				std::cout << "Creating a new event receiver." << std::endl;
				processor = new EventNode(description, &numSamplesInThisBuffer, 2, lock, currentNodeId, false);
				
				std::cout << midiChannelIndex << " is MIDI index." << std::endl;

			}

		}

		processor->setSourceNode(source);
		processor->setDestNode(dest);

		if (dest != 0)
			dest->setSourceNode(processor);
		
		if (source != 0)
			source->setDestNode(processor);

	} else if (processorType.equalsIgnoreCase("Visualizers")) {
		
		if (subProcessorType.equalsIgnoreCase("Stream Viewer")) {
			
			std::cout << "Creating a display node." << std::endl;
			processor = new DisplayNode(description, &numSamplesInThisBuffer, getSourceNode()->getNumOutputs(), lock, currentNodeId);
		}

		processor->setSourceNode(source);

		//processor = (DisplayNode*) processor;
		processor->setUIComponent(UI);

		if (source != 0)
			source->setDestNode(processor);
	
	}// else {

		//processor = new GenericProcessor(subProcessorType, &numSamplesInThisBuffer, 16, lock, id);

	//}

	if (processor != 0) {

		std::cout << "  Adding node to graph with ID number " << id << std::endl;
		addNode(processor,id);

		// need to update source and dest, in case the processor is a source or a visualizer
		source = processor->getSourceNode();
		dest = processor->getDestNode();


		if (source != 0) {
		
			std::cout << "   Connecting to source node " << source->getNodeId() << std::endl;
			


			for (int chan = 0; chan < processor->getNumInputs(); chan++) {

				//std::cout << "1";
				addConnection(source->getNodeId(), // sourceNodeID
				  	chan, // sourceNodeChannelIndex
				   	id, // destNodeID
				  	chan); // destNodeChannelIndex
			}

			std::cout << std::endl;

			//processor->setNumOutputs

			// connect event channel
			addConnection(source->getNodeId(), // sourceNodeID
				  	midiChannelIndex, // sourceNodeChannelIndex
				   	id, // destNodeID
				  	midiChannelIndex); // destNodeChannelIndex

		}

		if (dest != 0) {

			std::cout << "   Connecting to destination node " << dest->getNodeId() << std::endl;

			
			

			for (int chan = 0; chan < processor->getNumOutputs(); chan++) {
				//std::cout << "1";
				addConnection(id, // sourceNodeID
				  	chan, // sourceNodeChannelIndex
				   	dest->getNodeId(), // destNodeID
				  	chan); // destNodeChannelIndex
			}

			std::cout << std::endl;

			// connect event channel
			addConnection(id, // sourceNodeID
				  	midiChannelIndex, // sourceNodeChannelIndex
				   	dest->getNodeId(), // destNodeID
				  	midiChannelIndex); // destNodeChannelIndex

		}

		if (connectToAudioAndRecordNodes) {

			std::cout << "   Connecting to audio and record nodes." << std::endl;

			for (int chan = 0; chan < processor->getNumOutputs(); chan++) {

				addConnection(id, // sourceNodeID
				  	chan, // sourceNodeChannelIndex
				   	AUDIO_NODE_ID, // destNodeID
				  	chan); // destNodeChannelIndex

				addConnection(id, // sourceNodeID
				  	chan, // sourceNodeChannelIndex
				   	RECORD_NODE_ID, // destNodeID
				  	chan); // destNodeChannelIndex
			}

		}


		processor->setViewport(vp);
		return processor->createEditor();

	} else {
		return 0;
	}

}

void ProcessorGraph::removeProcessor(GenericProcessor* processor) {
	
	std::cout << "Removing processor with ID " << processor->getNodeId() << std::endl;

	GenericProcessor* source = processor->getSourceNode();
	GenericProcessor* dest = processor->getDestNode();
	int numInputs = processor->getNumInputs();

	std::cout << "  Source " << source << std::endl;
	std::cout << "  Dest " << dest << std::endl;

	removeNode(processor->getNodeId());

	if (dest !=0 && source !=0) {

		std::cout << "   Making new connections...." << std::endl;

		// connect source and dest
		for (int chan = 0; chan < numInputs; chan++) {
			
			addConnection(source->getNodeId(),
						  chan,
						  dest->getNodeId(),
						  chan);
		}

	}

	if (dest != 0)
		dest->setSourceNode(source);
	
	if (source != 0)
		source->setDestNode(dest);

}

bool ProcessorGraph::enableSourceNode() {
	//std::cout << "Enabling source node..." << std::endl;
	SourceNode* sn = getSourceNode();

	if (sn != 0) {
		sn->enable();
		return true;
	} else {
		return false;
	}
}

bool ProcessorGraph::disableSourceNode() {

	//std::cout << "Disabling source node..." << std::endl;
	SourceNode* sn = getSourceNode();

	if (sn != 0) {
		sn->disable();
		return true;
	} else {
		return false;
	}
}


RecordNode* ProcessorGraph::getRecordNode() {
	
	Node* node = getNodeForId(RECORD_NODE_ID);
	return (RecordNode*) node->getProcessor();

}

SourceNode* ProcessorGraph::getSourceNode() {

	if (SOURCE_NODE_ID != 0) {
		Node* node = getNodeForId(SOURCE_NODE_ID);
		return (SourceNode*) node->getProcessor();
	} else {
		return 0;
	}

}

XmlElement* ProcessorGraph::createNodeXml (GenericProcessor* const processor)
{

	if (processor == 0)
		return 0;
	
	XmlElement* e = new XmlElement("PROCESSOR");
	e->setAttribute (T("id"), (int) processor->getNodeId());
	e->setAttribute (T("name"), processor->getName());

	int sourceId = -1;
	int destId = -1;

	if (processor->getSourceNode() != 0)
		sourceId = processor->getSourceNode()->getNodeId();
	
	if (processor->getDestNode() != 0)
		destId = processor->getDestNode()->getNodeId();

	e->setAttribute (T("dest"), destId);
	e->setAttribute (T("source"), sourceId);

	// XmlElement* state = new XmlElement ("STATE");

 //    MemoryBlock m;
 //    node->getProcessor()->getStateInformation (m);
 //    state->addTextElement (m.toBase64Encoding());
 //    e->addChildElement (state);

 	return e;

}

const String ProcessorGraph::saveState(const File& file) 
{

	XmlElement* xml = new XmlElement("PROCESSORGRAPH");
	
	if (currentNodeId > 100) {

		GenericProcessor* processor = (GenericProcessor*) getNodeForId(currentNodeId)->getProcessor();
		
		xml->addChildElement (createNodeXml (processor));

		GenericProcessor* newProcessor = processor->getSourceNode();

		while (newProcessor != 0) {
			xml->addChildElement (createNodeXml (newProcessor));
			newProcessor = newProcessor->getSourceNode();
		}

		newProcessor = processor->getDestNode();

		while (newProcessor != 0) {
			xml->addChildElement (createNodeXml (newProcessor));
			newProcessor = newProcessor->getDestNode();
		}

		// int i;
		// for (i = 0; i < getNumNodes(); i++)
		// {
			
		// }	

		// for (i = 0; i < getNumConnections(); i++)
		// {
		// 	const AudioProcessorGraph::Connection* const fc = getConnection(i);

		// 	XmlElement* e = new XmlElement ("CONNECTION");

		// 	e->setAttribute (T("sourceProcessor"), (int) fc->sourceNodeId);
		// 	e->setAttribute (T("sourceChannel"), fc->sourceChannelIndex);
		// 	e->setAttribute (T("destProcessor"), (int) fc->destNodeId);
		// 	e->setAttribute (T("destChannel"), fc->destChannelIndex);

		// 	xml->addChildElement (e);

		// }

		String error;

		std::cout << "Saving processor graph." << std::endl;

		if (! xml->writeToFile (file, String::empty))
			error = "Couldn't write to file";
		
		delete xml;
		return error;

	} else {
		return "No nodes found, not saving.";
	}
}

const String ProcessorGraph::loadState(const File& file) 
{
	std::cout << "Loading processor graph." << std::endl;
	
	XmlDocument doc (file);
	XmlElement* xml = doc.getDocumentElement();

	if (xml == 0 || ! xml->hasTagName (T("PROCESSORGRAPH")))
	{
		delete xml;
		return "Not a valid file.";
	}

	String description;// = T(" ");
	int nextNodeId;

	GenericProcessor* sourceNode = 0;
	GenericProcessor* destNode = 0;

	forEachXmlChildElementWithTagName (*xml, e, T("PROCESSOR"))
	{
		if (e->getIntAttribute (T("source")) == -1) { // source node
			description = e->getStringAttribute (T("name"));
			nextNodeId = e->getIntAttribute (T("dest"));
			break;
		}
	}

	do {

		GenericEditor* editor = (GenericEditor*) createNewProcessor(description,
										 UI->getViewport(),
										 sourceNode,
										 destNode);

		UI->getViewport()->addEditor(editor);
		sourceNode = (GenericProcessor*) editor->getProcessor();

		forEachXmlChildElementWithTagName (*xml, e, T("PROCESSOR"))
		{	
			if (e->getIntAttribute (T("id")) == nextNodeId) { // source node
				description = e->getStringAttribute (T("name"));
				nextNodeId = e->getIntAttribute (T("dest"));
				break;
			}
		}

	} while (nextNodeId != -1);

	// add the last one:
	GenericEditor* editor = (GenericEditor*) createNewProcessor(description,
								UI->getViewport(),
								sourceNode,
								destNode);

	UI->getViewport()->addEditor(editor);

	delete xml;

	return "Everything went ok.";

}