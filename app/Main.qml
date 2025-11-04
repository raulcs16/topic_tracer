import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Styles
import App
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
            {
                if (args.length < 1) {
                    console.warn("Usage: touch <name>");
                    return;
                }
                for (let i = 0; i < args.length; i++) {
                    const topicName = args[i];
                    topic_controller.createTopic(topicName);
                }
                break;
            }
        case "join":
            {
                if (args.length < 2 || args.length > 2) {
                    console.warn("Usage: join <topicA> <topicB>");
                    return;
                }
                let topicA = args[0];
                let topicB = args[1];
                topic_controller.join(topicA, topicB);
                break;
            }
        case "remove":
            {
                if (args.length < 1) {
                    console.warn("Usage: remove <name>");
                    return;
                }
                const topic = args[0];
                topic_controller.deleteTopic(topic);
                break;
            }
        case "no":
            {
                if (args.length < 3) {
                    return;
                }
                if (args[0] != "join")
                    return;
                let topicA = args[1];
                let topicB = args[2];
                topic_controller.noJoin(topicA, topicB);
                break;
            }
        case "mv":
            {
                if (args.length < 2) {
                    return;
                }
                let topic = args[0];
                let newName = args[1];
                topic_controller.rename(topic, newName);
                break;
            }
        case "layout":
            {
                if (args.length < 1)
                    return;
                let layout = args[0];
                switch (layout) {
                case "default":
                    topic_controller.defaultLayout();
                    break;
                case "directed":
                    topic_controller.directedLayout();
                    break;
                case "circular":
                    topic_controller.circularLayout();
                    break;
                case "tree":
                    topic_controller.treeLayout();
                    break;
                case "planar":
                    topic_controller.planarLayout();
                    break;
                case "multi":
                    topic_controller.multiLayout();
                    break;
                default:
                    break;
                }
                break;
            }
        default:
            console.warn("Unknown command:", command);
        }
    }
    Shortcut {
        sequence: [":"]
        onActivated: commandInput.focus = true
    }

    TopicGraphController {
        id: topic_controller
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
                        edgeModel: topic_controller.edgeListModel
                        nodeModel: topic_controller.nodeListModel
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
                    model: topic_controller.topicListModel
                    anchors.fill: parent
                    anchors.topMargin: 15
                }
            }
        }
    }
}
