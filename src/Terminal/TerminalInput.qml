pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls

Item {
    id: root
    height: 35
    signal accepted(string text)
    property string suggestion: ""

    function contentWidthOf(txt) {
        return txtMetrics.width;
    }

    TextField {
        id: mainInput
        anchors.fill: parent
        focus: true
        color: "white"
        font.bold: true
        font.pointSize: 20
        background: Rectangle {
            color: "transparent"
        }

        // The "Ghost" Suggestion Layer
        Text {
            x: mainInput.leftPadding + root.contentWidthOf(mainInput.text)
            y: mainInput.topPadding
            text: root.suggestion.startsWith(mainInput.text) ? root.suggestion.substring(mainInput.text.length) : ""
            color: "#6272a4"
            font.bold: true
            font.pointSize: 20
            visible: mainInput.text.length > 0
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

    TextMetrics {
        id: txtMetrics
        font: mainInput.font
        text: mainInput.text
    }
}
