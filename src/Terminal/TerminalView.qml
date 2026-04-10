pragma ComponentBehavior: Bound
import QtQuick

import TerminalQt

Rectangle {
    id: terminalRoot
    clip: true

    required property TerminalListModel model
    property alias suggestion: inputField.suggestion
    signal commandEntered(string cmd)
    //border
    Rectangle {
        anchors.top: parent.top
        width: parent.width
        height: 1 // Thickness of the border
        color: '#73ffffff'
    }

    state: (historyList.contentHeight + inputField.height) > terminalRoot.height ? "fixed" : "growing"

    ListView {
        id: historyList
        width: parent.width
        delegate: EntryDelegate {}
        clip: true
        model: terminalRoot.model

        // Disable scroll when growing, enable when fixed
        interactive: terminalRoot.state === "fixed"

        onCountChanged: Qt.callLater(historyList.positionViewAtEnd)
    }

    TerminalInput {
        id: inputField
        width: parent.width
        onAccepted: cmd => terminalRoot.commandEntered(cmd)
    }

    // 3. Define the two states
    states: [
        State {
            name: "growing"
            AnchorChanges {
                target: historyList
                anchors.top: terminalRoot.top
            }
            AnchorChanges {
                target: inputField
                anchors.top: historyList.bottom
            }
            PropertyChanges {
                historyList.height: historyList.contentHeight
                historyList.interactive: false
            }
        },
        State {
            name: "fixed"
            AnchorChanges {
                target: inputField
                anchors.bottom: terminalRoot.bottom
            }
            AnchorChanges {
                target: historyList
                anchors.top: terminalRoot.top
                anchors.bottom: inputField.top
            }
            // height is automatically handled by the top/bottom anchors
        }
    ]

    // Smooth transition between growing and fixed
    transitions: Transition {
        AnchorAnimation {
            duration: 150
            easing.type: Easing.OutQuad
        }
    }
}
