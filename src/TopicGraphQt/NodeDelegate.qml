pragma ComponentBehavior: Bound
import QtQuick

import TopicGraph

Item {
    id: root
    property real viewWidth: 0
    property real viewHeight: 0

    required property int index
    required property int id
    required property string label
    required property double posx
    required property double posy
    required property int flags

    readonly property bool hover: (flags & ENUMS.StateFlag.Hovered) !== 0
    readonly property bool selected: (flags & ENUMS.StateFlag.Selected) !== 0

    x: (viewWidth / 3) + posx
    y: (viewHeight / 3) + posy
    Rectangle {
        id: node
        width: 20
        height: 20
        x: -width / 2
        y: -height / 2
        color: root.selected ? "#9806f3" : '#dee0e7'
        radius: 100
        border.width: 2
        border.color: root.hover ? '#9806f3' : "#af9476"
    }
    Text {
        text: root.label
        color: "white"
        anchors.top: node.bottom
        anchors.horizontalCenter: node.horizontalCenter
        anchors.topMargin: 4
        font.pointSize: root.hover ? 16 : 12
    }
}
