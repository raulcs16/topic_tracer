pragma ComponentBehavior: Bound
import QtQuick

import Terminal

Item {
    id: root
    width: TerminalConfig.minWidth
    height: TerminalConfig.minHeight
    visible: true
    TerminalController {
        id: controller
    }

    Rectangle {
        anchors.fill: parent
        color: "#1e1e1e" // Standard dark terminal background
    }

    // 2. Use a Column to neatly stack them without overlaps
    Column {
        anchors.fill: parent
        spacing: 5

        TerminalView {
            id: historyList
            width: parent.width
            height: parent.height - inputField.height - parent.spacing
            model: TerminalStore.historyList
            delegate: EntryDelegate {}
            clip: true // Prevents text from bleeding out of the view bounds
        }

        TerminalInput {
            id: inputField
            width: parent.width
            height: 40 // Give it a tangible height!
            onAccepted: cmd => controller.executeCmd(cmd)
            onTabPressed: text => controller.autoComplete(text)
            suggestion: TerminalStore.suggestion
        }
    }
}
