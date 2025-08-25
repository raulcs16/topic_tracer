.Phony: all clean build compile graph show tests

.DEFAULT_GOAL := compile
BUILD_DIR := build
GRAPH_DOT := $(BUILD_DIR)/graph.dot
GRAPH_IMG := $(BUILD_DIR)/dependencyGraph.png

clean:
	rm -rf $(BUILD_DIR)
	find app/ src/ -type f -name "*.ini" -exec rm -f {} +

build:
	cmake -S . -B $(BUILD_DIR)

compile:
	cmake --build $(BUILD_DIR)
	
graph: 
	cmake -S . -B $(BUILD_DIR) --graphviz=$(GRAPH_DOT)
	dot -Tpng $(GRAPH_DOT) -o $(GRAPH_IMG)

show:
	open $(GRAPH_IMG)

test:
	./$(BUILD_DIR)/tests/unit_tests -v normal
