EXE = app.exe
IMGUI_DIR = ../
UNAME_S := $(shell uname -s)

SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)imgui.cpp $(IMGUI_DIR)imgui_demo.cpp $(IMGUI_DIR)imgui_draw.cpp $(IMGUI_DIR)imgui_tables.cpp $(IMGUI_DIR)imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)backends/imgui_impl_glfw.cpp $(IMGUI_DIR)backends/imgui_impl_opengl3.cpp
SOURCES += newtonMethod.cpp app.cpp

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
#OBJS = $(SOURCES:%.cpp = test/%.o)

LINUX_GL_LIBS = -lGL

CXXFLAGS  = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)backends -L/usr/include -L/usr/local/include 
CXXFLAGS += -lgmp -lmpfr -lboost_filesystem -lboost_system -lboost_program_options
CXXFLAGS += -g -w -Wall -Wformat

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE) 
	@echo running App...
	@./$(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
