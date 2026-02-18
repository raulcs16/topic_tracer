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

    property double paddingX: 0
    property double paddingY: 0
    property color border_color: "red"
    property int border_width: 2

    Rectangle {
        x: root.rectX - (root.paddingX / 2)
        y: root.rectY - (root.paddingY / 2)
        width: root.rectW + root.paddingX
        height: root.rectH + root.paddingY
        color: "transparent"
        border.color: root.border_color
        border.width: root.border_width
    }
}
