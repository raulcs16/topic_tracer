pragma ComponentBehavior: Bound
import QtQuick

import Graph

Item {
    id: root

    width: childrenRect.width
    height: childrenRect.height

    required property Graph model

    // property Component edgeDelegate: EdgeSpline {}

    Repeater {
        model: root.model.edges
        delegate: EdgeSpline {}
    }

    Repeater {
        model: root.model.nodes
        delegate: Rectangle {
            required property int index
            required property string node
            required property double posx
            required property double posy
            required property double w
            required property double h
            width: w
            height: h
            x: posx - w / 2
            y: posy - h / 2
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
