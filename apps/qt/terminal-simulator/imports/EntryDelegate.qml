pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts

Item {
    id: root

    // Bind width directly to the ListView's width safely
    width: ListView.view ? ListView.view.width : 200
    // Set the height based on what the layout naturally needs
    height: layout.implicitHeight

    required property string text
    required property int type

    readonly property var colors: ({
            0: "#f2f2f2" // Bright off-white (Default)
            ,
            1: "#f25c5c" // Red (Errors)
            ,
            2: "#5cf25c" // Green (Success)
            ,
            3: "#f2ca5c" // Yellow (Warnings)
            ,
            4: "#5cbef2" // Blue
            ,
            5: "#c05cf2" // Magenta
            ,
            6: "#5cf2ea" // Cyan
            ,
            7: "#8c8c8c" // Muted Gray
        })

    RowLayout {
        id: layout
        // Anchor to fill the wrapper, but apply your side padding safely here
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 15

        Text {
            // Keeps the prompt symbol aligned to the top if text wraps
            Layout.alignment: Qt.AlignTop
            text: root.type === 0 ? ">" : " "
            color: root.colors[0]
            font.pixelSize: 20
            font.bold: true
            font.family: "monospace" // Optional: gives a uniform terminal width
        }

        Text {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            text: root.text
            color: root.colors[root.type] ?? root.colors[0]
            font.pixelSize: 20
            font.bold: true
            wrapMode: Text.Wrap
            font.family: "monospace"
        }
    }
}
