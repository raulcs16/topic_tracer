pragma ComponentBehavior: Bound
import QtQuick

import TopicGraph

Item {
    id: root

    required property NodeListModel nodeModel
    required property EdgeListModel edgeModel

    property real viewWidth: 0
    property real viewHeight: 0

    width: viewWidth
    height: viewHeight

    Repeater {
        model: root.edgeModel
        delegate: EdgeSpline {
            viewWidth: root.viewWidth
            viewHeight: root.viewHeight
        }
    }

    Repeater {
        model: root.nodeModel
        delegate: Item {
            required property int index
            required property int id
            required property string label
            required property double posx
            required property double posy
            required property bool highlighted
            x: (root.viewWidth / 3) + posx
            y: (root.viewHeight / 3) + posy
            Rectangle {
                id: node
                width: 20
                height: 20
                x: -width / 2
                y: -height / 2
                color: '#dee0e7'
                radius: 100
                border.width: 2
                border.color: parent.highlighted ? '#9806f3' : "#af9476"
            }
            Text {
                text: parent.label
                color: "white"
                anchors.top: node.bottom
                anchors.horizontalCenter: node.horizontalCenter
                anchors.topMargin: 4
                font.pointSize: parent.highlighted ? 16 : 12
            }
        }
    }
}
