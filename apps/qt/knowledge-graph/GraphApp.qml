pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts

import Graph
import Views

Item {
    id: root
    width: 800
    height: 600

    ViewPortDriver {
        id: view_driver
    }

    GraphController {
        id: controller
        viewDriver: view_driver
    }

    RowLayout {
        anchors.fill: parent
        spacing: 1
        ViewPort {
            Layout.fillWidth: true
            Layout.fillHeight: true
            driver: view_driver
            GraphView {
                id: graph
                nodeModel: GraphStore.nodeList
                edgeModel: GraphStore.edgeList
                boxModel: GraphStore.boxList
                nodeDelegate: NodeDelegate {
                    onMouseOver: controller.nodeHovered(itemId)
                    onMouseExit: controller.nodeUnHovered(itemId)
                    onClicked: controller.nodeSelected(itemId)
                }
                edgeDelegate: EdgeSpline {}
                boxDelegate: BoundingBoxDelegate {}
            }
        }

        LabelListView {
            id: listView
            Layout.fillHeight: true
            Layout.preferredWidth: 250
            model: GraphStore.nodeList
            delegate: LabelDelegate {
                onMouseOver: controller.nodeHovered(itemId)
                onMouseExit: controller.nodeUnHovered(itemId)
                onClicked: controller.nodeSelected(itemId)
            }
        }
    }

    Shortcut {
        sequence: "1"
        onActivated: controller.toggleViewGrid()
    }
}
