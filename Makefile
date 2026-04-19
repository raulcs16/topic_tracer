.Phony: all clean build compile graph show test-off test-on

.DEFAULT_GOAL := compile
BUILD_DIR := build
GRAPH_DOT := $(BUILD_DIR)/graph.dot
GRAPH_IMG := $(BUILD_DIR)/dependencyGraph.png

clean:
	cmake --build $(BUILD_DIR) --target clean
	find app/ src/ -type f -name "*.ini" -exec rm -f {} +

build:
	cmake -S . -B $(BUILD_DIR)

compile:
	cmake --build $(BUILD_DIR) 

-v:
	cmake --build $(BUILD_DIR) -v

test-off:
	cmake -S .B $(BUILD_DIR) -DTESTING=OFF

test-on:
	cmake -S .B $(BUILD_DIR) -DTESTING=ON

debug:
	cmake -S .B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug

rel-deb:	
	cmake -S .B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=RelWithDebInfo

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
	./build/app/TopicTracerEx.app/Contents/MacOS/TopicTracerEx

coverage:
	cmake -S . -B $(BUILD_DIR) -DCOVERAGE=ON
	cmake --build $(BUILD_DIR) --target coverage