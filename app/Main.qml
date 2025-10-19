import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Styles
import TopicList
import App
import Graph
import TopicGraph

ApplicationWindow {
    id: app
    width: 2074 * .8
    height: 1296 * .8
    title: qsTr("Topic Tracer")
    visible: true

    function parseCommand(cmd) {
        const parts = cmd.split(" ");
        const command = parts[0];
        const args = parts.slice(1);

        switch (command) {
        case "touch":
            if (args.length < 1) {
                console.warn("Usage: touch <name>");
                return;
            }
            for (let i = 0; i < args.length; i++) {
                const topicName = args[i];
                topic_controller.createTopic(topicName);
            }
            break;
        case "join":
            if (args.length < 2 || args.length > 2) {
                console.warn("Usage: join <topicA> <topicB>");
                return;
            }
            const topicA = args[0];
            const topicB = args[1];
            topic_controller.join(topicA, topicB);
            break;
        default:
            console.warn("Unknown command:", command);
        }
    }
    Shortcut {
        sequence: [":"]
        onActivated: commandInput.focus = true
    }

    GraphController {
        id: graph_cotnroller
    }
    TopicGraphController {
        id: topic_controller
        topicListModel: topicListView.topicListModel
        graphController: graph_cotnroller
    }
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Main content + status bar
        ColumnLayout {
            spacing: 0
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                id: main_content
                color: Colors.primary
                Layout.fillWidth: true
                Layout.fillHeight: true

                CanvasView {
                    anchors.fill: parent
                    clip: true
                    focus: true
                    GraphView {
                        controller: graph_cotnroller
                        anchors.fill: parent
                        property var highlightedNode: null
                        property var highlightedEdge: null
                        viewWidth: main_content.width
                        viewHeight: main_content.height
                    }
                }
            }

            Rectangle {
                id: command_interface
                color: Colors.accent
                Layout.fillWidth: true
                Layout.preferredHeight: 75

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 6

                    Text {
                        id: prompt
                        text: "$"
                        font.bold: true
                        font.pointSize: 20
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                    }

                    TextField {
                        id: commandInput
                        Layout.fillWidth: true
                        color: "white"
                        font.pointSize: 20
                        font.bold: true
                        background: Rectangle {
                            color: "transparent"
                        }

                        Keys.onReturnPressed: {
                            const cmd = commandInput.text.trim();
                            if (cmd.length === 0)
                                return;
                            app.parseCommand(cmd);
                            commandInput.text = "";
                        }
                    }
                }
            }
        }

        // Sidebar
        ColumnLayout {
            spacing: 0
            Layout.preferredWidth: 300 // fixed width
            Layout.maximumWidth: 350
            Layout.fillHeight: true

            Rectangle {
                color: Colors.secondary
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                Text {
                    text: "Explorer"
                    font.pointSize: 16
                    font.bold: Font.Light
                    color: Colors.text_primary
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                }
            }

            Rectangle {
                color: Colors.secondary
                Layout.fillWidth: true
                Layout.fillHeight: true

                TopicListView {
                    id: topicListView
                    anchors.fill: parent
                    anchors.topMargin: 15
                }
            }
        }
    }
}
