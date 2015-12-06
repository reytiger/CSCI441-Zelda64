
# Windows builds
ifeq ($(OS), Windows_NT)
	GENERATOR ?= "MinGW Makefiles"
	INCLUDE_PATH += "C:/Strawberry/c/include/;Z:/CSCI441GFx/include"
	LIBRARY_PATH += "Z:/CSCI441GFx/lib;C:/Strawberry/c/lib/"

# Mac and Linux builds
else
	GENERATOR ?= "Unix Makefiles"
	INCLUDE_PATH += "/Users/jpaone/Desktop/include;$(HOME)/;$(HOME)/LIBS/include"
	LIBRARY_PATH += "/Users/jpaone/Desktop/lib;$(HOME)/lib;$(HOME)/LIBS/lib"
endif

BINARY=keyToTheKingdom
BUILD_DIR="build-lab"

all: $(BINARY)

$(BINARY): $(BUILD_DIR)
	@$(MAKE) --quiet -C $(BUILD_DIR)
	@cp -v $</keyToTheKingdom $(BINARY)

$(BUILD_DIR):
	@mkdir -vp $@
	cd $@ && \
		cmake -G $(GENERATOR) -Wno-dev \
		-DUSE_CLANG_FORMAT=OFF \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INCLUDE_PATH=$(INCLUDE_PATH) \
		-DCMAKE_LIBRARY_PATH=$(LIBRARY_PATH) \
		-DSOIL_INCLUDE_DIR=$(HOME)/LIBS/include/SOIL \
		-DSOIL_LIBRARY=$(HOME)/LIBS/lib/libSOIL.a \
		..

clean:
	@$(MAKE) -C $(BUILD_DIR) clean --quiet

# For when you really mean it.
clean2:
	@rm -rf $(BUILD_DIR)/*
	@rm -rvf $(BUILD_DIR)

.PHONY: clean clean2
