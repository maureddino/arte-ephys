/*
  ==============================================================================

    ControlPanel.cpp
    Created: 1 May 2011 2:57:48pm
    Author:  jsiegle

  ==============================================================================
*/

#include "ControlPanel.h"
#include <stdio.h>


PlayButton::PlayButton()
	: DrawableButton (T("PlayButton"), DrawableButton::ImageFitted)
{

		DrawablePath normal, over, down;

        Path p;
        p.addTriangle (0.0f, 0.0f, 0.0f, 20.0f, 18.0f, 10.0f);
        normal.setPath (p);
        normal.setFill (Colours::lightgrey);
        normal.setStrokeThickness (0.0f);

        over.setPath (p);
        over.setFill (Colours::black);
        over.setStrokeFill (Colours::black);
        over.setStrokeThickness (5.0f);

        down.setPath (p);
        down.setFill (Colours::pink);
        down.setStrokeFill (Colours::pink);
        down.setStrokeThickness (5.0f);

        setImages (&normal, &over, &over);
        setBackgroundColours(Colours::darkgrey, Colours::yellow);
        setClickingTogglesState (true);
        setTooltip ("Start/stop acquisition");
}

PlayButton::~PlayButton()
{	
}

RecordButton::RecordButton()
	: DrawableButton (T("RecordButton"), DrawableButton::ImageFitted)
{

		DrawablePath normal, over, down;

        Path p;
        p.addEllipse (0.0,0.0,20.0,20.0);
        normal.setPath (p);
        normal.setFill (Colours::lightgrey);
        normal.setStrokeThickness (0.0f);

        over.setPath (p);
        over.setFill (Colours::black);
        over.setStrokeFill (Colours::black);
        over.setStrokeThickness (5.0f);

        setImages (&normal, &over, &over);
        setBackgroundColours(Colours::darkgrey, Colours::red);
        setClickingTogglesState (true);
        setTooltip ("Start/stop writing to disk");
}

RecordButton::~RecordButton()
{	
}


CPUMeter::CPUMeter() : Label(T("CPU Meter"),"0.0"), cpu(0.5f)
{
}

CPUMeter::~CPUMeter()
{
}

void CPUMeter::updateCPU(float usage) {
	cpu = usage;
}

void CPUMeter::paint(Graphics& g)
{
	g.fillAll(Colours::grey);
	
	g.setColour(Colours::orange);
	g.fillRect(0.0f,0.0f,getWidth()*cpu,float(getHeight()));

	g.setColour(Colours::black);
	g.drawRect(0,0,getWidth(),getHeight(),1);

}


DiskSpaceMeter::DiskSpaceMeter()
{
}


DiskSpaceMeter::~DiskSpaceMeter()
{
}

void DiskSpaceMeter::updateDiskSpace(float percent)
{
	diskFree = percent;
}

void DiskSpaceMeter::paint(Graphics& g)
{

	g.fillAll(Colours::grey);
	
	g.setColour(Colours::lightgrey);
	g.fillRect(0.0f,0.0f,getWidth()*diskFree,float(getHeight()));

	g.setColour(Colours::black);
	g.drawRect(0,0,getWidth(),getHeight(),1);
	
}

Clock::Clock() : Label(T("Clock"),"00:00.00")
{
}

Clock::~Clock()
{
}

ControlPanel::ControlPanel(ProcessorGraph* graph_, AudioComponent* audio_) : 
			graph (graph_), audio(audio_)
{

	playButton = new PlayButton();
	playButton->addListener (this);
	addAndMakeVisible(playButton);

	recordButton = new RecordButton();
	recordButton->addListener (this);
	addAndMakeVisible(recordButton);

	masterClock = new Clock();
	addAndMakeVisible(masterClock);

	cpuMeter = new CPUMeter();
	addAndMakeVisible(cpuMeter);

	diskMeter = new DiskSpaceMeter();
	addAndMakeVisible(diskMeter);

}

ControlPanel::~ControlPanel()
{
	deleteAllChildren();
	graph = 0;
}

void ControlPanel::resized()
{
	int w = getWidth();
	int h = getHeight();

	if (playButton != 0)
		playButton->setBounds(w-h*5,0,h,h);
	
	if (recordButton != 0)
		recordButton->setBounds(w-h*4,0,h,h);

	if (masterClock != 0)
		masterClock->setBounds(w-h*3,0,h*2,h);
	
	if (cpuMeter != 0)
		cpuMeter->setBounds(20,h/4,h*4,h/2);

	if (diskMeter != 0)
		diskMeter->setBounds(150,h/4,h*4,h/2);
}

void ControlPanel::buttonClicked(Button* button) 

{
	if (button == recordButton)
	{
		std::cout << "Record button pressed." << std::endl;
		if (recordButton->getToggleState())
		{
			playButton->setToggleState(true,true);

			if (audio->callbacksAreActive()) {
			//	audio->endCallbacks();
			}

			graph->getRecordNode()->setParameter(1,10.0f); // turn on recording

		} else {

			//audio->endCallbacks();
			graph->getRecordNode()->setParameter(0,10.0f); // turn off recording

		}

	} else if (button == playButton) {
		std::cout << "Play button pressed." << std::endl;
		if (!playButton->getToggleState())
		{
			recordButton->setToggleState(false,true);
		}

	}

	if (playButton->getToggleState())
	{

		if (!audio->callbacksAreActive()) 
			audio->beginCallbacks();

	} else {

		if (audio->callbacksAreActive())
			audio->endCallbacks();

	}

}

void ControlPanel::actionListenerCallback(const String & msg)
{
	//std::cout << "Message Received." << std::endl;
	cpuMeter->updateCPU(audio->getCpuUsage());
	cpuMeter->repaint();

	diskMeter->updateDiskSpace(graph->getRecordNode()->getFreeSpace());
	diskMeter->repaint();

}