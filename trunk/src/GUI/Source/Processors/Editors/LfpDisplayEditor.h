/*
  ==============================================================================

    LfpDisplayEditor.h
    Created: 8 Feb 2012 12:56:43pm
    Author:  jsiegle

  ==============================================================================
*/

#ifndef __LFPDISPLAYEDITOR_H_3438800D__
#define __LFPDISPLAYEDITOR_H_3438800D__

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "GenericEditor.h"
#include "../../UI/UIComponent.h"
#include "../../UI/DataViewport.h"
#include "../Visualization/DataWindow.h"

class FilterViewport;
class DataViewport;
class DataWindow;

class SelectorButton : public DrawableButton
{
	public:
		SelectorButton();
		~SelectorButton();	
};

class LfpDisplayEditor : public GenericEditor,
				   	     public Button::Listener,
				   	     public Slider::Listener
{
public:
	LfpDisplayEditor (GenericProcessor*, FilterViewport*, DataViewport*);
	~LfpDisplayEditor();

	void buttonClicked (Button* button);
	void setBuffers (AudioSampleBuffer*, MidiBuffer*);
	void setUIComponent (UIComponent* ui) {UI = ui;}

	void sliderValueChanged (Slider* slider);

private:	
	
	ScopedPointer <DataWindow> dataWindow;

	Slider* timebaseSlider;
	Slider* displayGainSlider;

	SelectorButton* windowSelector;
	SelectorButton* tabSelector;

	AudioSampleBuffer* streamBuffer;
	MidiBuffer* eventBuffer;
	UIComponent* UI;
	DataViewport* dataViewport;

	int tabIndex;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LfpDisplayEditor);

};

#endif  // __LFPDISPLAYEDITOR_H_3438800D__
