# Automatically generated makefile, created by the Jucer
# Don't edit this file! Your changes will be overwritten when you re-save the Jucer project!

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(TARGET_ARCH),)
  TARGET_ARCH := -march=native
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) -D__STDC_LIMIT_MACROS
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L"/usr/X11R6/lib/" -L"../../JuceLibraryCode/bin" -lfreetype -lpthread -lrt -lX11 -lGL -lGLU -lXinerama -lasound -lftdi -lftgl
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2"
  TARGET := arte
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS) -D__STDC_LIMIT_MACROS
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -L"/usr/X11R6/lib/" -L"../../JuceLibraryCode/bin" -lfreetype -lpthread -lrt -lX11 -lGL -lGLU -lXinerama -lasound -lftdi -lftgl
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2"
  TARGET := arte
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/datapacket_e79ff33c.o \
  $(OBJDIR)/netcom_7ad18ece.o \
  $(OBJDIR)/Bessel_63b88b81.o \
  $(OBJDIR)/Biquad_479045c5.o \
  $(OBJDIR)/Butterworth_a752fb81.o \
  $(OBJDIR)/Cascade_7ab417a7.o \
  $(OBJDIR)/ChebyshevI_f2af4063.o \
  $(OBJDIR)/ChebyshevII_649cfe90.o \
  $(OBJDIR)/Custom_e24db5a8.o \
  $(OBJDIR)/Design_3ed2fd75.o \
  $(OBJDIR)/Documentation_c5f0da2d.o \
  $(OBJDIR)/Elliptic_8ad9a5b9.o \
  $(OBJDIR)/Filter_e4029a2f.o \
  $(OBJDIR)/Legendre_89451537.o \
  $(OBJDIR)/Param_13628c80.o \
  $(OBJDIR)/PoleFilter_e04f707.o \
  $(OBJDIR)/RBJ_418b612d.o \
  $(OBJDIR)/RootFinder_239a995f.o \
  $(OBJDIR)/State_22979684.o \
  $(OBJDIR)/AudioComponent_521bd9c9.o \
  $(OBJDIR)/LfpDisplayNode_30ef22fd.o \
  $(OBJDIR)/Splitter_6e27a57b.o \
  $(OBJDIR)/DataWindow_83ce6754.o \
  $(OBJDIR)/LfpDisplayCanvas_4a58e87e.o \
  $(OBJDIR)/OpenGLCanvas_3c775a41.o \
  $(OBJDIR)/SpikeDetector_300d85e7.o \
  $(OBJDIR)/FileReader_18023b0e.o \
  $(OBJDIR)/AudioNode_94606ff3.o \
  $(OBJDIR)/EventNode_95c842b7.o \
  $(OBJDIR)/SignalGeneratorEditor_3223ac0a.o \
  $(OBJDIR)/LfpDisplayEditor_99d75351.o \
  $(OBJDIR)/SourceNodeEditor_8d813122.o \
  $(OBJDIR)/SplitterEditor_c2199b6c.o \
  $(OBJDIR)/SpikeDetectorEditor_2fb63eeb.o \
  $(OBJDIR)/AudioEditor_fb2c6555.o \
  $(OBJDIR)/FilterEditor_dfe1f39d.o \
  $(OBJDIR)/GenericEditor_becb2ad6.o \
  $(OBJDIR)/FileReaderThread_933ea08.o \
  $(OBJDIR)/okFrontPanelDLL_158af498.o \
  $(OBJDIR)/NetworkThread_276b0d43.o \
  $(OBJDIR)/FPGAThread_a8dc34ed.o \
  $(OBJDIR)/DataBuffer_6ae4f549.o \
  $(OBJDIR)/IntanThread_32126b71.o \
  $(OBJDIR)/DataThread_b2a47a13.o \
  $(OBJDIR)/RecordNode_2b7a1a2.o \
  $(OBJDIR)/SignalGenerator_a9cf4806.o \
  $(OBJDIR)/ResamplingNode_27a58a6b.o \
  $(OBJDIR)/FilterNode_817e9c9.o \
  $(OBJDIR)/SourceNode_c2d6336c.o \
  $(OBJDIR)/GenericProcessor_733760aa.o \
  $(OBJDIR)/ProcessorGraph_68b34a0b.o \
  $(OBJDIR)/CustomLookAndFeel_53a8fcdb.o \
  $(OBJDIR)/InfoLabel_a2051bf4.o \
  $(OBJDIR)/DataViewport_2cf95d2c.o \
  $(OBJDIR)/Configuration_6915cd84.o \
  $(OBJDIR)/MessageCenter_748a1cca.o \
  $(OBJDIR)/FilterViewport_47880e3a.o \
  $(OBJDIR)/FilterList_10c1d1b2.o \
  $(OBJDIR)/ControlPanel_a895ede3.o \
  $(OBJDIR)/UIComponent_d667ba37.o \
  $(OBJDIR)/MainWindow_499ac812.o \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/JuceLibraryCode1_682c927f.o \
  $(OBJDIR)/JuceLibraryCode2_683aaa00.o \
  $(OBJDIR)/JuceLibraryCode3_6848c181.o \
  $(OBJDIR)/JuceLibraryCode4_6856d902.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking arte
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning arte
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

$(OBJDIR)/datapacket_e79ff33c.o: ../../Source/Network/datapacket.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling datapacket.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/netcom_7ad18ece.o: ../../Source/Network/netcom.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling netcom.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Bessel_63b88b81.o: ../../Source/Dsp/Bessel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Bessel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Biquad_479045c5.o: ../../Source/Dsp/Biquad.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Biquad.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Butterworth_a752fb81.o: ../../Source/Dsp/Butterworth.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Butterworth.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Cascade_7ab417a7.o: ../../Source/Dsp/Cascade.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Cascade.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ChebyshevI_f2af4063.o: ../../Source/Dsp/ChebyshevI.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ChebyshevI.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ChebyshevII_649cfe90.o: ../../Source/Dsp/ChebyshevII.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ChebyshevII.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Custom_e24db5a8.o: ../../Source/Dsp/Custom.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Custom.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Design_3ed2fd75.o: ../../Source/Dsp/Design.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Design.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Documentation_c5f0da2d.o: ../../Source/Dsp/Documentation.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Documentation.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Elliptic_8ad9a5b9.o: ../../Source/Dsp/Elliptic.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Elliptic.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Filter_e4029a2f.o: ../../Source/Dsp/Filter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Filter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Legendre_89451537.o: ../../Source/Dsp/Legendre.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Legendre.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Param_13628c80.o: ../../Source/Dsp/Param.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Param.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PoleFilter_e04f707.o: ../../Source/Dsp/PoleFilter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PoleFilter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RBJ_418b612d.o: ../../Source/Dsp/RBJ.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RBJ.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RootFinder_239a995f.o: ../../Source/Dsp/RootFinder.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RootFinder.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/State_22979684.o: ../../Source/Dsp/State.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling State.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioComponent_521bd9c9.o: ../../Source/Audio/AudioComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LfpDisplayNode_30ef22fd.o: ../../Source/Processors/LfpDisplayNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LfpDisplayNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Splitter_6e27a57b.o: ../../Source/Processors/Utilities/Splitter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Splitter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DataWindow_83ce6754.o: ../../Source/Processors/Visualization/DataWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DataWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LfpDisplayCanvas_4a58e87e.o: ../../Source/Processors/Visualization/LfpDisplayCanvas.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LfpDisplayCanvas.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OpenGLCanvas_3c775a41.o: ../../Source/Processors/Visualization/OpenGLCanvas.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling OpenGLCanvas.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SpikeDetector_300d85e7.o: ../../Source/Processors/SpikeDetector.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SpikeDetector.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FileReader_18023b0e.o: ../../Source/Processors/FileReader.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FileReader.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioNode_94606ff3.o: ../../Source/Processors/AudioNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EventNode_95c842b7.o: ../../Source/Processors/EventNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EventNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SignalGeneratorEditor_3223ac0a.o: ../../Source/Processors/Editors/SignalGeneratorEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SignalGeneratorEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LfpDisplayEditor_99d75351.o: ../../Source/Processors/Editors/LfpDisplayEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LfpDisplayEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SourceNodeEditor_8d813122.o: ../../Source/Processors/Editors/SourceNodeEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SourceNodeEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SplitterEditor_c2199b6c.o: ../../Source/Processors/Editors/SplitterEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SplitterEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SpikeDetectorEditor_2fb63eeb.o: ../../Source/Processors/Editors/SpikeDetectorEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SpikeDetectorEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioEditor_fb2c6555.o: ../../Source/Processors/Editors/AudioEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FilterEditor_dfe1f39d.o: ../../Source/Processors/Editors/FilterEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FilterEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GenericEditor_becb2ad6.o: ../../Source/Processors/Editors/GenericEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GenericEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FileReaderThread_933ea08.o: ../../Source/Processors/DataThreads/FileReaderThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FileReaderThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/okFrontPanelDLL_158af498.o: ../../Source/Processors/DataThreads/okFrontPanelDLL.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling okFrontPanelDLL.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/NetworkThread_276b0d43.o: ../../Source/Processors/DataThreads/NetworkThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling NetworkThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FPGAThread_a8dc34ed.o: ../../Source/Processors/DataThreads/FPGAThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FPGAThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DataBuffer_6ae4f549.o: ../../Source/Processors/DataThreads/DataBuffer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DataBuffer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/IntanThread_32126b71.o: ../../Source/Processors/DataThreads/IntanThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling IntanThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DataThread_b2a47a13.o: ../../Source/Processors/DataThreads/DataThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DataThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RecordNode_2b7a1a2.o: ../../Source/Processors/RecordNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RecordNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SignalGenerator_a9cf4806.o: ../../Source/Processors/SignalGenerator.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SignalGenerator.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ResamplingNode_27a58a6b.o: ../../Source/Processors/ResamplingNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ResamplingNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FilterNode_817e9c9.o: ../../Source/Processors/FilterNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FilterNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SourceNode_c2d6336c.o: ../../Source/Processors/SourceNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SourceNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GenericProcessor_733760aa.o: ../../Source/Processors/GenericProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GenericProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ProcessorGraph_68b34a0b.o: ../../Source/Processors/ProcessorGraph.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ProcessorGraph.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CustomLookAndFeel_53a8fcdb.o: ../../Source/UI/CustomLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CustomLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/InfoLabel_a2051bf4.o: ../../Source/UI/InfoLabel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling InfoLabel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DataViewport_2cf95d2c.o: ../../Source/UI/DataViewport.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DataViewport.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Configuration_6915cd84.o: ../../Source/UI/Configuration.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Configuration.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MessageCenter_748a1cca.o: ../../Source/UI/MessageCenter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MessageCenter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FilterViewport_47880e3a.o: ../../Source/UI/FilterViewport.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FilterViewport.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FilterList_10c1d1b2.o: ../../Source/UI/FilterList.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FilterList.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ControlPanel_a895ede3.o: ../../Source/UI/ControlPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ControlPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UIComponent_d667ba37.o: ../../Source/UI/UIComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling UIComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainWindow_499ac812.o: ../../Source/MainWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/JuceLibraryCode1_682c927f.o: ../../JuceLibraryCode/JuceLibraryCode1.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling JuceLibraryCode1.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/JuceLibraryCode2_683aaa00.o: ../../JuceLibraryCode/JuceLibraryCode2.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling JuceLibraryCode2.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/JuceLibraryCode3_6848c181.o: ../../JuceLibraryCode/JuceLibraryCode3.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling JuceLibraryCode3.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/JuceLibraryCode4_6856d902.o: ../../JuceLibraryCode/JuceLibraryCode4.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling JuceLibraryCode4.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
