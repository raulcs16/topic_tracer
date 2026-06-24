.PHONY: all clean build compile core qt-on qt-off apps-on apps-off main-on main-off coverage-off test-off test-on debug rel-deb release install mac-sign dmg mac-icon graph ctest show

-include build/metadata.mk

.DEFAULT_GOAL := compile
BUILD_DIR := build
DIST_DIR  := dist
REL_DIR   := releases
GRAPH_DOT := $(BUILD_DIR)/graph.dot
GRAPH_IMG := $(BUILD_DIR)/dependencyGraph.png
DMG_OUT   := $(REL_DIR)/$(APP_NAME)_v$(APP_VERSION).dmg


clean:
	cmake --build $(BUILD_DIR) --target clean
	find app/ src/ -type f -name "*.ini" -exec rm -f {} +

build:
	cmake -S . -B $(BUILD_DIR)

compile:
	cmake --build $(BUILD_DIR)

-v:
	cmake --build $(BUILD_DIR) -v

core:
	cmake -S . -B $(BUILD_DIR) -DQT=OFF -DAPPS=OFF -DMAIN=OFF

qt-on:
	cmake -S . -B $(BUILD_DIR) -DQT=ON -DAPSS=OFF -DMAIN=OFF

qt-off:
	cmake -S . -B $(BUILD_DIR) -DQT=OFF

apps-on:
	cmake -S . -B $(BUILD_DIR) -DQT=ON -DAPPS=ON

apps-off:
	cmake -S . -B $(BUILD_DIR) -DAPPS=OFF

main-on:
	cmake -S . -B $(BUILD_DIR) -DQT=ON -DAPPS=ON -DMAIN=On

main-off:
	cmake -S . -B $(BUILD_DIR) -DMAIN=OFF

coverage-off:
	cmake -S . -B $(BUILD_DIR) -DCOVERAGE=OFF

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
	cd app/resources && iconutil -c icns tt.iconset

graph:
	cmake -S . -B $(BUILD_DIR) --graphviz=$(GRAPH_DOT)
	dot -Tpng $(GRAPH_DOT) -o $(GRAPH_IMG)

ctest:
	cd $(BUILD_DIR) && ctest

show:
	open $(GRAPH_IMG)

# chmod +x rml; export PATH=".:$PATH"%