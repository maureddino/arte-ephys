/*
  ==============================================================================

    DisplayNode.cpp
    Created: 7 May 2011 5:07:43pm
    Author:  jsiegle

  ==============================================================================
*/


#include "DisplayNode.h"
#include "ResamplingNode.h"
#include <stdio.h>

DisplayNode::DisplayNode(const String name_, int* nSamps, int nChans,
	 const CriticalSection& lock_, int id, DataViewport* dv)
	: GenericProcessor(name_, nSamps, nChans, lock_, id),
	  dataViewport(dv)

{
	
}

DisplayNode::~DisplayNode()
{
}

AudioProcessorEditor* DisplayNode::createEditor()
{

	Visualizer* visualizer = new Visualizer(this, viewport, dataViewport);

	GenericProcessor* source = (GenericProcessor*) getSourceNode();

	visualizer->setBuffers(source->getContinuousBuffer(),source->getEventBuffer());
	visualizer->setUIComponent(getUIComponent());
	visualizer->setConfiguration(config);

	setEditor(visualizer);
	
	std::cout << "Creating visualizer." << std::endl;
	return visualizer;

}
