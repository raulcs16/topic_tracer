import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import GraphControllers
import TerminalQt

ApplicationWindow {
    id: app
    width: 2074 * .8
    height: 1296 * .8
    title: qsTr("Terminal Sandbox")
    visible: true

    required property TerminalController controller
    required property TerminalListModel model
    Item {
        id: viewport
        focus: true
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TerminalView {
            id: terminalView
            model: app.model
            // Now Layout attached properties work cleanly!
            Layout.fillWidth: true
            Layout.preferredHeight: isOpen ? 300 : 75

            // Declare local properties used by your event handlers
            property bool isOpen: true
            property string suggestion: ""

            // Hardcoding colors/borders for sandbox isolation if Colors module isn't loaded
            // Replace with your 'Colors.primary' / 'Colors.accent' assets as needed
            color: "#1e1e1e"
            border.width: 2
            border.color: app.activeFocusItem === terminalView ? "#00FFCC" : "transparent"

            Behavior on Layout.preferredHeight {
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.OutCubic
                }
            }

            // Input handlers communicating with your Controller API
            onCommandEntered: cmd => {
                if (cmd.length === 0)
                    return;
                app.controller.executeCommand(cmd);
            }

            onTabPressed: txt => {
                if (txt.length === 0)
                    return;
                app.controller.handleSuggestion(txt);
            }

            // Catch the C++ signal broadcast
            Connections {
                target: app.controller
                function onSuggestionReady(hint) {
                    terminalView.suggestion = hint;
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
