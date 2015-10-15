FLAGS := -std=c++11 -g -Os -fmax-errors=5
Wwarnings := -Wall -Wextra
Wno_warnings := -Wno-char-subscripts -Wno-deprecated-declarations -Wno-unused-variable -Wno-narrowing

# Any directory in 'include' is fair game.
INCPATH := -Iinclude -Iext -Iext/yaml/include -Iext/fmod/inc
LIBPATH := -Lext/fmod/lib/x86_64

CXXFLAGS += $(INCPATH) $(FLAGS) $(Wwarnings) $(Wno_warnings)

YAML_LIB=libyaml-cpp.a

# This needs to be a MinGW or Visual Studio generator on Windows.
YAML_CMAKE_GEN="Unix Makefiles"

# Windows builds
ifeq ($(OS), Windows_NT)
	LD_FLAGS += $(LIBPATH) -lglut -lopengl32 -lglu32

# Mac builds
else ifeq ($(shell uname), Darwin)
	LD_FLAGS += $(LIBPATH) -framework GLUT -framework OpenGL -framework Cocoa

# Linux and all other builds
else
	LD_FLAGS += $(LIBPATH) -lglut -lGL -lGLU -lm -L./ext -lyaml-cpp -lfmod
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

ext/$(YAML_LIB):
	@mkdir -p object/yaml/
	+cd object/yaml/ \
		&& cmake ../../ext/yaml/ -G $(YAML_CMAKE_GEN) -DYAML_CPP_BUILD_TOOLS=OFF -DBoost_INCLUDE_DIR=../../ext\
		&& make --quiet \
		&& cp $(YAML_LIB) ../../ext

format:
	-@clang-format $(HEADERS) -i
	-@clang-format $(SOURCES) -i

$(BINARY): $(OBJECTS) ext/$(YAML_LIB)
	$(CXX) -o $@ $(OBJECTS) $(LD_FLAGS)

clean:
	@rm -vf $(YAML_LIB)
	@rm -vfr object/yaml
	@echo
	@rm -vf $(OBJECTS) $(BINARY) depend.mk
	@rm -vfr object
	@echo

object/%.o: source/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

depend:
	@echo "Generating depedencies"
	@$(CXX) $(CXXFLAGS) $(INCPATH) -MM $(SOURCES) > depend.mk
	@echo "Done generating depedencies"

# Same as above, but to stdout and as a tree.
depend-tree:
	$(CXX) $(CXXFLAGS) $(INCPATH) -MM $(SOURCES) -H 2>&1

dep.png:
	cinclude2dot \--include /usr/include/c++/4.8,/usr/include,include,\include/$(find include -type f -printf "%f,") \
	| dot -Tpng -o dep.png

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

	@echo CXXFLAGS:
	@printf " %s\n" $(CXXFLAGS)

.PHONY: clean files format dep.png

sinclude depends.mk
