import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import TerminalQt

ApplicationWindow {
    id: app
    width: 2074 * .8
    height: 1296 * .8
    title: qsTr("Terminal Sandbox")
    visible: true

    required property TerminalController controller

    Item {
        id: viewport
        focus: true
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TerminalView {
            id: terminalView
            controller: app.controller
            // Now Layout attached properties work cleanly!
            Layout.fillWidth: true
            Layout.preferredHeight: isOpen ? 300 : 75

            // Declare local properties used by your event handlers
            property bool isOpen: true

            color: "#1e1e1e"
            border.width: 2
            border.color: app.activeFocusItem === terminalView ? "#00FFCC" : "transparent"

            Behavior on Layout.preferredHeight {
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.OutCubic
                }
            }

            // Return focus to fallback element when escaping
            Keys.onEscapePressed: {
                viewport.forceActiveFocus();
            }
        }
        // Spacer element to push the terminal view to the top or bottom layout block
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
