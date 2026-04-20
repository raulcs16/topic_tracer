.Phony: all clean build compile graph show test-off test-on

-include build/metadata.mk

.DEFAULT_GOAL := compile
BUILD_DIR := build
DIST_DIR := dist
REL_DIR := releases
GRAPH_DOT := $(BUILD_DIR)/graph.dot
GRAPH_IMG := $(BUILD_DIR)/dependencyGraph.png
DMG_OUT := $(REL_DIR)/$(APP_NAME)_v$(APP_VERSION).dmg

clean:
	cmake --build $(BUILD_DIR) --target clean
	find app/ src/ -type f -name "*.ini" -exec rm -f {} +

build:
	cmake -S . -B $(BUILD_DIR)

compile:
	cmake --build $(BUILD_DIR) 

-v:
	cmake --build $(BUILD_DIR) -v

coverage-off:
	cmake -S . -B $(BUILD_DIR) -DCOVERAGE=ON

test-off:
	cmake -S . -B $(BUILD_DIR) -DTESTING=OFF

test-on:
	cmake -S . -B $(BUILD_DIR) -DTESTING=ON

debug:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

rel-deb:	
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=RelWithDebInfo

release:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release

install:
	cmake --install $(BUILD_DIR) --prefix $(DIST_DIR)

mac-sign:
	codesign --force --deep --sign - $(DIST_DIR)/$(APP_NAME).app

dmg: install mac-sign
	 mkdir -p $(REL_DIR)
	 hdiutil create -volname "$(APP_NAME) $(APP_VERSION)" -srcfolder $(DIST_DIR) -ov -format UDZO $(DMG_OUT)

mac-icon:
	cd app/resources; iconutil -c icns tt.iconset

graph: 
	cmake -S . -B $(BUILD_DIR) --graphviz=$(GRAPH_DOT)
	dot -Tpng $(GRAPH_DOT) -o $(GRAPH_IMG)

ctest: 
	cd $(BUILD_DIR); ctest

show:
	open $(GRAPH_IMG)

run:
	./build/app/$(APP_NAME).app/Contents/MacOS/$(APP_NAME)

coverage:
	cmake -S . -B $(BUILD_DIR) -DCOVERAGE=ON
	cmake --build $(BUILD_DIR) --target coverage