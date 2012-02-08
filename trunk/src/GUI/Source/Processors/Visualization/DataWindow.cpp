/*
  ==============================================================================

    DataWindow.cpp
    Created: 8 Feb 2012 1:10:13pm
    Author:  jsiegle

  ==============================================================================
*/

#include "DataWindow.h"


DataWindow::DataWindow(Button* cButton)
	: DocumentWindow ("Stream Window", 
					  Colours::black, 
					  DocumentWindow::allButtons),
	  controlButton(cButton)

{
	centreWithSize(300,200);
	setUsingNativeTitleBar(true);
	setResizable(true,true);
	setTitleBarHeight(40);
}

DataWindow::~DataWindow()
{
	setContentComponent (0);
}

void DataWindow::closeButtonPressed()
{
	setVisible(false);
	controlButton->setToggleState(false,false);
}