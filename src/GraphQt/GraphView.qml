pragma ComponentBehavior: Bound
import QtQuick

import Graph

Item {
    id: root
    required property Graph model

    // property Component edgeDelegate: EdgeSpline {}

    // Repeater {
    //     model: root.model.edges
    //     delegate: Loader {
    //         property var model: modelData
    //         sourceComponent: root.edgeDelegate
    //     }
    // }

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
            x: posy * 5 * index
            y: posx
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
