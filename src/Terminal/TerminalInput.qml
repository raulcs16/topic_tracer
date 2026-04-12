pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

Item {
    id: root
    height: 35
    signal accepted(string text)
    property string suggestion: ""
    property alias text: mainInput.text

    function contentWidthOf(txt) {
        return txtMetrics.width;
    }
    Row {
        id: layout
        anchors.fill: parent
        anchors.leftMargin: 10 // Overall side padding
        spacing: 15 // This creates the "even" space between > and the cursor |

        Text {
            id: prompt
            text: ">"
            font.bold: true
            font.pointSize: 20
            color: "white"
            anchors.verticalCenter: parent.verticalCenter
        }

        TextField {
            id: mainInput
            width: parent.width - prompt.width - layout.spacing - (layout.anchors.leftMargin * 2)
            height: parent.height
            focus: true
            color: "white"
            font.bold: true
            font.pointSize: 20
            verticalAlignment: TextInput.AlignVCenter

            // Remove default padding to ensure cursor starts exactly at the start of the field
            leftPadding: 0
            rightPadding: 0

            background: Rectangle {
                color: "transparent"
            }

            // The "Ghost" Suggestion Layer
            Text {
                // Offset by the width of the current text
                x: root.contentWidthOf(mainInput.text)
                anchors.verticalCenter: parent.verticalCenter

                text: root.suggestion.startsWith(mainInput.text) ? root.suggestion.substring(mainInput.text.length) : ""
                color: "#6272a4" // Dracula-style comment color for ghosts
                font: mainInput.font
                visible: mainInput.text.length > 0
                opacity: 0.7
            }

            Keys.onTabPressed: event => {
                if (root.suggestion !== "" && root.suggestion.startsWith(text)) {
                    text = root.suggestion;
                    event.accepted = true;
                }
            }

            onAccepted: {
                root.accepted(text);
                text = "";
            }
        }
    }
    TextMetrics {
        id: txtMetrics
        font: mainInput.font
        text: mainInput.text
    }
}
