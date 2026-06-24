import QtQuick

import Views

import "GraphEngine.js" as Engine

QtObject {
    id: controller
    required property ViewPortDriver viewDriver

    function toggleViewGrid() {
        viewDriver.toggleGrid();
    }
    function nodeHovered(nodeId) {
        const index = Engine.findIndex(nodeId);
        if (index === -1)
            return;
        GraphStore.nodeList.get(index).hovered = true;
    }
    function nodeUnHovered(nodeId) {
        const node = Engine.findNode(nodeId);
        if (node === undefined)
            return;
        node.hovered = false;
    }
    function nodeSelected(nodeId) {
        const node = Engine.findNode(nodeId);
        if (node == undefined)
            return;
        node.selected = true;
    }
}
