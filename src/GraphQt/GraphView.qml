pragma ComponentBehavior: Bound
import QtQuick

import Graph

Item {
    id: root

    required property GraphController controller
    property real viewWidth: 0
    property real viewHeight: 0

    width: viewWidth
    height: viewHeight

    Repeater {
        model: root.controller.edges
        delegate: EdgeSpline {
            viewWidth: root.viewWidth
            viewHeight: root.viewHeight
        }
    }

    Repeater {
        model: root.controller.nodes
        delegate: Item {
            required property int index
            required property int id
            required property string label
            required property double posx
            required property double posy
            x: (root.viewWidth / 2) + posx
            y: (root.viewHeight / 2) + posy
            Rectangle {
                id: node
                width: 20
                height: 20
                x: -width / 2
                y: -height / 2
                color: '#dee0e7'
                radius: 100
                border.width: 1
                border.color: "#af9476"
            }
            Text {
                text: parent.label
                color: "white"
                anchors.top: node.bottom
                anchors.horizontalCenter: node.horizontalCenter
                anchors.topMargin: 4
                font.pointSize: 12
            }
        }
    }
}
