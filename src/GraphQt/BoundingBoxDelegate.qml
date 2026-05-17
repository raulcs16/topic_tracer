pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root

    required property int index
    required property int id
    required property rect rect
    property string label: ""

    property color border_color: "red"
    property int border_width: 2

    Text {
        text: root.label ? root.label : root.id
        color: "#fff"
        font.bold: true
        font.pointSize: 24
        anchors.bottom: box.top
        anchors.left: box.left
        anchors.bottomMargin: 5
    }
    Rectangle {
        id: box
        x: root.rect.x
        y: root.rect.y
        width: root.rect.width
        height: root.rect.height
        color: "transparent"
        border.color: root.border_color
        border.width: root.border_width
    }
}
