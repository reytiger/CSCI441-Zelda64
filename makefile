CODEGEN=-g -O2
WARNING=-Wall -Wextra -Wno-unused-parameter -fsignaling-nans
INCPATH=-Iinclude
CXXFLAGS=-std=c++11 $(INCPATH) $(CODEGEN) $(WARNING)

# Windows builds
ifeq ($(OS), Windows_NT)
	LD_FLAGS=$(LIBPATH) -lglut -lopengl32 -lglu32

# Mac builds
else ifeq ($(shell uname), Darwin)
	LD_FLAGS=$(LIqBPATH) -framework GLUT -framework OpenGL -framework Cocoa

# Linux and all other builds
else
	LD_FLAGS=$(LIBPATH) -lglut -lGL -lGLU -lm
endif

BINARY=guildWars
SOURCES=$(wildcard source/*/*.cpp)  $(wildcard source/*.cpp)
HEADERS=$(wildcard include/*/*.hpp) $(wildcard include/*.hpp)
OBJECTS=$(SOURCES:source/%.cpp=%.o)

all: $(BINARY)

format:
	-clang-format $< $(HEADERS) -i
	-clang-format $< $(SOURCES) -i

$(BINARY): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LD_FLAGS)

clean:
	@rm -vf $(OBJECTS) $(BINARY)

%.o: source/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

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

.PHONY: clean files
