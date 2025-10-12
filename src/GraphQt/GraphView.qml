pragma ComponentBehavior: Bound
import QtQuick

import Graph

Item {
    id: root

    width: childrenRect.width
    height: childrenRect.height

    required property GraphController controller

    Repeater {
        model: root.controller.edges
        delegate: EdgeSpline {}
    }

    Repeater {
        model: root.controller.nodes
        delegate: Item {
            required property int index
            required property int id
            required property double posx
            required property double posy
            x: posx
            y: posy
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
                text: parent.id
                color: "white"
                anchors.top: node.bottom
                anchors.horizontalCenter: node.horizontalCenter
                anchors.topMargin: 4
                font.pointSize: 12
            }
        }
    }
}
