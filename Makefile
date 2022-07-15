CXX := g++
CXXFLAGS := -c -Wall -O3

TARGET_DIR := bin/
TARGET_FILE := $(TARGET_DIR)EmuShell

CPFLAGS := 
LDFLAGS := -lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image

DIRS := 
DIRS += bin/
DIRS += bin/Release/
DIRS += bin/ReleaseLinux/
DIRS += obj/
DIRS += obj/Common/
DIRS += obj/Nes/
DIRS += obj/Nes/Mappers/
DIRS += obj/Nes/Palettes/
DIRS += obj/Nes/Rom/
DIRS += obj/Nes/SoundChannels/
DIRS += obj/Providers/
DIRS += obj/Providers/Blip_Buffer/
DIRS += obj/Providers/Timer/
DIRS += obj/XMLParser/

SRCS :=
CPPS := $(subst ./,obj/,$(shell find . -regex ^.*?\.cpp$))
OBJS := $(subst .cpp,.o,$(CPPS))
MKDIR := mkdir -p $(DIRS)

INCLUDES := 
LINKERS := 

ifeq ($(CXX), clang++)
	CXXFLAGS += --std=c++11
endif

ifeq ($(CXX), g++)
	CXXFLAGS += -std=gnu++11
endif

ifeq ($(OS), WINDOWS)
# make 3 levels for search ...
	CPPS = $(subst ./,obj/,$(wildcard *.cpp)) $(subst ./,obj/,$(wildcard */*.cpp)) $(subst ./,obj/,$(wildcard */*/*.cpp))
	SRCS = $(subst .cpp,.o,$(CPPS))
	OBJS = $(patsubst %,obj/%,$(SRCS))
	TARGET_DIR = bin/Release/
	TARGET_FILE = $(TARGET_DIR)EmuShell.exe
	CPFLAGS += -w -Wl,-subsystem,windows
	OBJS += obj/Resources.o
	MKDIR = @mkdir $(subst /,\,$(DIRS)) ||: 
	INCLUDES += -IC:/SDL2-2.0.3/i686-w64-mingw32/include
	INCLUDES += -IC:/SDL2-2.0.3/i686-w64-mingw32/include/SDL2
	INCLUDES += -IC:/SDL2_image-2.0.0-mingw/i686-w64-mingw32/include
	INCLUDES += -IC:/SDL2_ttf-2.0.12-mingw/i686-w64-mingw32/include
	LINKERS += -LC:/SDL2-2.0.3/i686-w64-mingw32/lib
	LINKERS += -LC:/SDL2_image-2.0.0-mingw/i686-w64-mingw32/lib
	LINKERS += -LC:/SDL2_ttf-2.0.12-mingw/i686-w64-mingw32/lib
# I don't know why, but inserting LINKERS and INCLUDES in the obj/%.o patterns won't work, only work this way :(
	CXXFLAGS += -IC:/SDL2-2.0.3/i686-w64-mingw32/include
	CXXFLAGS += -IC:/SDL2-2.0.3/i686-w64-mingw32/include/SDL2
	CXXFLAGS += -IC:/SDL2_image-2.0.0-mingw/i686-w64-mingw32/include
	CXXFLAGS += -IC:/SDL2_ttf-2.0.12-mingw/i686-w64-mingw32/include
	CXXFLAGS += -LC:/SDL2-2.0.3/i686-w64-mingw32/lib
	CXXFLAGS += -LC:/SDL2_image-2.0.0-mingw/i686-w64-mingw32/lib
	CXXFLAGS += -LC:/SDL2_ttf-2.0.12-mingw/i686-w64-mingw32/lib
else
	TARGET_DIR := bin/ReleaseLinux/
	TARGET_FILE := $(TARGET_DIR)EmuShell
	INCLUDES = 
	LINKERS = 
endif

all: dir $(OBJS)
	@echo Creating executable $@
	$(CXX) $(OBJS) $(CPFLAGS) $(INCLUDES) $(LINKERS) $(LDFLAGS) -o $(TARGET_FILE)
	
dir:
	$(MKDIR)

obj/%.o: %.cpp
	@echo Building $@
	@$(CXX) $(CXXFLAGS) $< -o $@

obj/Resources.o: Resources.rc
	@windres.exe -J rc -O coff -i Resources.rc -o $@
	
clean:
	@$(RM) $(OBJS)