CODEGEN=-g -O2
WARNING=-Wall -Wextra -Wno-unused-parameter
INCPATH=-Iinclude
CXXFLAGS=-std=c++11 $(INCPATH) $(CODEGEN) $(WARNING)

# Windows builds
ifeq ($(OS), Windows_NT)
	LD_FLAGS=$(LIBPATH) -lglut -lopengl32 -lglu32

# Mac builds
else ifeq ($(shell uname), Darwin)
	LD_FLAGS=$(LIBPATH) -framework GLUT -framework OpenGL -framework Cocoa

# Linux and all other builds
else
	LD_FLAGS=$(LIBPATH) -lglut -lGL -lGLU -lm
endif

# Thanks, SO!
# http://stackoverflow.com/a/12959694
# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

BINARY  := guildWars
SOURCES := $(strip $(call rwildcard, source/,*.cpp))
HEADERS := $(strip $(call rwildcard, include/,*.hpp))
OBJECTS := $(strip $(addprefix object/, $(SOURCES:source/%.cpp=%.o)))

all: format depend $(BINARY)

format:
	-clang-format $(HEADERS) -i
	-clang-format $(SOURCES) -i

$(BINARY): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LD_FLAGS)

clean:
	@rm -vf $(OBJECTS) $(BINARY) depends.mk

object/%.o: source/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

depend:
	$(CXX) $(INCPATH) -MM $(SOURCES) > depend.mk

# This is only for debugging and record keeping.
files:
	@printf "BINARY: %s\n" $(BINARY)
	@echo

	@echo Source:
	@printf " %s\n" $(SOURCES)
	@echo

	@echo Header:
	@printf " %s\n" $(HEADERS)
	@echo

	@echo Object:
	@printf " %s\n" $(OBJECTS)
	@echo

.PHONY: clean files format

sinclude depends.mk
