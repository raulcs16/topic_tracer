pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root

    required property int index
    required property int id
    required property rect rect

    property color border_color: "red"
    property int border_width: 2

    Rectangle {
        x: root.rect.x
        y: root.rect.y
        width: root.rect.width
        height: root.rect.height
        color: "transparent"
        border.color: root.border_color
        border.width: root.border_width
    }
}
