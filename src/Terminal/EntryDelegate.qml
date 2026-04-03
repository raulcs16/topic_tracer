pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts

RowLayout {
    id: root
    width: parent.width
    spacing: 8

    required property int index
    required property string text
    required property int type

    readonly property var colors: ({
            0: "#50fa7b" // Command (Green)
            ,
            1: "#f8f8f2" // Output (White)
            ,
            2: "#ff5555" // Error (Red)
            ,
            3: "#6272a4" // Hint (Gray)
            ,
            4: "#8be9fd" // Success (Cyan)
        })

    Text {
        text: root.type === 0 ? ">" : " "
        color: root.colors[0]
        font.pixelSize: 20
        font.bold: true
    }

    Text {
        Layout.fillWidth: true
        text: root.text
        color: root.colors[root.type]
        font.pixelSize: 20
        font.bold: true
        wrapMode: Text.Wrap
    }
}
