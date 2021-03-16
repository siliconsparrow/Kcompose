
TARGET = synthtest
LIBS = -lasound
BUILDPATH = build

SOURCES = \
	AlsaMidiIn.cpp \
	AlsaPlayback.cpp \
	main.cpp

INCLUDES = -I. \

CFLAGS = -O0 -g3
	
CXXFLAGS = $(CFLAGS)
	
CXX = g++
OBJS = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(notdir $(SOURCES)))))

all: $(BUILDPATH) $(addprefix $(BUILDPATH)/,$(OBJS))
	g++ -o $(BUILDPATH)/$(TARGET) $(addprefix $(BUILDPATH)/,$(OBJS)) $(LIBS)

clean:
	rm $(addprefix $(BUILDPATH)/,$(OBJS))
	rm $(BUILDPATH)/$(TARGET)
	rmdir $(BUILDPATH)

$(BUILDPATH):
	mkdir $(BUILDPATH)
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $(@F) $<

$(BUILDPATH)/%.o: %.cpp
	$(CC) $(CFLAGS) $(BUILDFLAGS) -c -o $(BUILDPATH)/$(@F) $<

$(BUILDPATH)/%.o: %.c
	$(CC) $(CFLAGS) $(BUILDFLAGS) -c -o $(BUILDPATH)/$(@F) $<
