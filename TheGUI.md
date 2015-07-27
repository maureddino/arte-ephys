# Introduction #

In trunk/src/GUI is a preliminary version of the GUI. Right now it has extremely basic functionality: streaming in data from an outside source (currently the Intan dev board), displaying the data with OpenGL, and listening to the incoming input in real time through the computer’s default audio outputs.

The good news is that the framework should allow us to scale things up quickly. Adding new components (such as a disk writer, a spike viewer, a filter editor, etc.) will be very straightforward, thanks to the JUCE library used to create the GUI.

# About JUCE #

The GUI is built on top of the JUCE library (http://www.rawmaterialsoftware.com/), an open-source C++ library with a ton of useful classes. I found its GUI tools to be extremely intuitive—everything is structured around components, which act as containers for other components (such as text boxes or buttons) and can listen for actions performed on those components. But this is not fundamentally different from what other GUI libraries do (although I found JUCE's syntax to be easier to grasp than others I’ve tried). The real advantage of JUCE is the fact that it was originally developed to build audio processing applications. Classes for creating audio processors, connecting them together, and making sure they process their data in the right order can be used pretty much out-of-the-box, of course substituting neural data for audio data.

Other advantages of JUCE are the great documentation, the well-written source code, an active developer community, and the ease of cross-platform compilation. It’s lightweight (it takes less than 2 minutes to compile the whole thing), yet includes an incredibly wide range of classes.

Disadvantages include the fact that there’s only one available tutorial for JUCE: http://homepages.wmich.edu/~a5winfield/juce/JuceTutorial.pdf. It’s very basic, and only covers a small fraction of JUCE’s functionality. But the many demos included in the standard download make up for this. Reading through the source code of these demos was my primary means for learning how to use the library. Definitely check these out before diving in.

In general, though, the more I use JUCE, the more I like it. I think it’s ideal for our project in a number of ways, and was a breath of fresh air after trying out a number of other GUI libraries (Qt, GTK+, wxWidgets, FLTK, openFrameworks, clutter…)

# Dependencies #

The JUCE source code is included in the JuceLibraryCode folder of the GUI directory, so you don’t even have to download JUCE if you don’t want to. However, if you want to add any files or dependencies to the GUI part of the project, it’s best to do this through the “Jucer” application, which automatically generates makefiles for Linux, OS X, and Windows, all in one go. You can **probably** edit the makefile yourself, but I wouldn’t recommend it. To install, download JUCE from the sourceforge site, cd to juce/extras/Jucer (experimental)/Builds/Linux/ and type “make” Open the executable in the “build” folder, then open “arte.jucer” file from within the Jucer. You can either add new blank code files by right-clicking on a subdirectory (recommended), or add existing files by doing the same. Make sure you save the project before you try to compile, otherwise the makefile won’t be updated.

To communicate with the Intan dev board (and to build the GUI in its current form), you’ll need to install libftdi-dev, an open-source library for interfacing with FTDI chips via USB. This library depends on libusb, which should already be installed on Ubuntu.

Before building the GUI, I needed to install these libraries (following a clean install of Ubuntu 10.10):

build-essential

freeglut3-dev

libfreetype6-dev

libxinerama-dev

libxcursor-dev

libasound2-dev


All of these can be installed with apt-get (e.g. $ sudo apt-get -y install libfreetype6-dev)

# General logic of the whole thing #

For a beautiful flowchart of the main classes in the current project, check out trunk/docs/GUI\_structure.pdf

As it stands, classes are organized into three groups:

1. UI – classes that display something, or allow the user to interact with the program

2. Audio – classes that interface with the computer’s audio hardware, for audio monitoring purposes, and for generating the callbacks necessary for funnel data through the processing pipeline. This class is responsible for setting the all-important buffer size.

3. Processors – the meat of the application; these classes communicate with whatever data source is being used, handle filtering, resampling for display and audio monitoring, and write data to disk. They all inherent from JUCE’s AudioProcessor class, and live inside an AudioProcessorGraph. Disregard the fact that most of these classes were originally created to handle audio data—they are general enough to handle any sort of high-sample-rate data that needs to be processed in real time, which is exactly what we’re dealing with. Ultimately, I envision Processor classes becoming the basis for a wide range of drag-and-drop signal processing tools, but we are not quite at this point yet.

The most important class at the moment is the DataThread, located within “SourceNode.h”. The DataThread is responsible for communicating with the data source (in this case the Intan dev board, but eventually the NIDAQ boards) and writing all incoming data into a FIFO buffer. Whenever the SourceNode receives a callback, it copies all available data from this FIFO into another buffer, which is then passed on to the next processor. This is a rather quick and dirty way to do things, as it can't deal with errors well, but it works for now.

In theory, to make the application compatible with another data source, you’ll only need to update two methods:
> DataThread::updateBuffer() -- asks the data source to fill a small buffer with available data
> DataThread::sortData() -- sorts the data (if the bits are all mixed up), and puts it into the FIFO

When SourceNode::processBlock() is called, it copies the data from the FIFO to the main processing buffer.

# What’s implemented so far #

Main.cpp -- starts up the application and creates the main window

MainWindow.cpp -- owns UIComponent, AudioComponent, and ProcessorGraph

The following objects are created by, and owned by, the main window:

UIComponent.cpp -- holds all the UI components, such as displays, buttons, labels, sliders, tabs, etc., etc.

AudioComponent.cpp -- deals with audio hardware, and generates the callbacks that funnel data through the application

ProcessorGraph.cpp -- generates the graph that determines how data flows through the system

The various UI components and Processor classes should be pretty self-explanatory.

# Immediate plans #

1. Add disk writing. This should be pretty straightforward, as JUCE has a lot of classes for this. We do need to decide on a data format at some point, but that’s not an immediate concern.

2. Add tetrode functionality, including spike detection and display.

3. Add interactive sliders for changing filter parameters, adjusting the timebase and range of the display, etc.

4. Make it possible to save and load configurations.

Once these items are checked off, the GUI will replicate the basic functionality of the current AD system, and should be useable for collecting data. Obviously we’ll need to rigorously test its reliability before using it for anything mission-critical, but that’s the fun part, right?