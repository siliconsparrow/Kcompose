
TARGET = Kcompose
LIBS = -lasound -pthread `pkg-config gtkmm-3.0 --libs`
BUILDPATH = build
GTKMM_INC = $(shell pkg-config gtkmm-3.0 --cflags-only-I)
GTKMM_OPT = $(shell pkg-config gtkmm-3.0 --cflags-only-other)

SOURCES = \
	AlsaMidiIn.cpp \
	PcMidiKeyboard.cpp \
	AlsaPlayback.cpp \
	MiniFmSynth.cpp \
	GuiMessage.cpp \
	Gui.cpp \
	KComposeWindow.cpp \
	main.cpp

INCLUDES = -I. \
	$(GTKMM_INC)
	

CFLAGS = -O0 -g3 -pthread $(GTKMM_OPT) $(INCLUDES)
	
CXXFLAGS = $(CFLAGS)
	
CXX = g++
OBJS = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(notdir $(SOURCES)))))

all: $(BUILDPATH) $(addprefix $(BUILDPATH)/,$(OBJS))
	g++ -o $(BUILDPATH)/$(TARGET) $(addprefix $(BUILDPATH)/,$(OBJS)) $(LIBS)

clean:
	rm -Rf $(BUILDPATH)
#	rm $(addprefix $(BUILDPATH)/,$(OBJS))
#	rm $(BUILDPATH)/$(TARGET)
#	rmdir $(BUILDPATH)

$(BUILDPATH):
	mkdir $(BUILDPATH)
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@F) $<

$(BUILDPATH)/%.o: %.cpp
	$(CC) $(CFLAGS) $(BUILDFLAGS) -c -o $(BUILDPATH)/$(@F) $<

$(BUILDPATH)/%.o: %.c
	$(CC) $(CFLAGS) $(BUILDFLAGS) -c -o $(BUILDPATH)/$(@F) $<
