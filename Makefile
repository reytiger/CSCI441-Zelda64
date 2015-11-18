
# Windows builds
ifeq ($(OS), Windows_NT)
	GENERATOR ?= "MinGW Makefiles"
	INCLUDE_PATH += "C:/Strawberry/c/include/;Z:/CSCI441GFx/include"
	LIBRARY_PATH += "Z:/CSCI441GFx/lib;C:/Strawberry/c/lib/"

# Mac and Linux builds
else
	GENERATOR ?= "Unix Makefiles"
	INCLUDE_PATH += "$(HOME)/"
	LIBRARY_PATH += "$(HOME)/lib"
endif

BINARY=a7
BUILD_DIR="build-lab"

all: $(BINARY)

$(BINARY): $(BUILD_DIR)
	@$(MAKE) --quiet -C $(BUILD_DIR) Assignment
	@cp -v $</Assignment $(BINARY)

$(BUILD_DIR):
	@mkdir -vp $@
	cd $@ && \
		cmake -G $(GENERATOR) -Wno-dev \
		-DUSE_CLANG_FORMAT=OFF \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INCLUDE_PATH=$(INCLUDE_PATH) \
		-DCMAKE_LIBRARY_PATH=$(LIBRARY_PATH) \
		..

clean:
	@$(MAKE) -C $(BUILD_DIR) clean --quiet

# For when you really mean it.
clean2:
	@rm -rf $(BUILD_DIR)/*
	@rm -rvf $(BUILD_DIR)

.PHONY: clean clean2
