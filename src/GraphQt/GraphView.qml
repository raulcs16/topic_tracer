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
        delegate: Rectangle {
            required property int index
            required property string node
            required property double posx
            required property double posy
            // required property double w
            // required property double h
            width: 20
            height: 20
            x: posx - width / 2
            y: posy - height / 2
            color: '#dee0e7'
            radius: 4
            border.width: 1
            border.color: "#af9476"
            Text {
                anchors.centerIn: parent
                text: parent.node
                color: "black"
            }
        }
    }
}
