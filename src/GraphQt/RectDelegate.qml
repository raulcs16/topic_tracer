pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root

    required property int index
    required property int rectId

    required property double rectX
    required property double rectY
    required property double rectW
    required property double rectH

    property color border_color: "red"
    property int border_width: 2

    Rectangle {
        x: root.rectX
        y: root.rectY
        width: root.rectW
        height: root.rectH
        color: "transparent"
        border.color: root.border_color
        border.width: root.border_width
    }
}
