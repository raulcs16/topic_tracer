pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts

import Styles

RowLayout {
    id: root
    width: parent.width
    anchors.leftMargin: 10 // Overall side padding
    spacing: 15

    required property int index
    required property string text
    required property int type

    readonly property var colors: ({
            0: Colors.terminal_command,
            1: Colors.terminal_out,
            2: Colors.terminal_error,
            3: Colors.terminal_hint,
            4: Colors.terminal_success
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
