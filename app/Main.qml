import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Styles
import TopicGraph

ApplicationWindow {
    id: app
    width: 2074 * .8
    height: 1296 * .8
    title: qsTr("Topic Tracer")
    visible: true

    required property int major
    required property int minor
    required property int patch
    property Item focusItem: null

    TopicGraphController {
        id: topic_controller
    }
    function parseCommand(cmd) {
        if (cmd.length == 0)
            return;
        const parts = cmd.split(" ");
        const command = parts[0];
        const args = parts.slice(1);

        switch (command) {
        case "clear":
            topic_controller.clearAll();
            break;
        case "save":
            {
                if (args.length < 1) {
                    console.warn("Usage: save <file>");
                    return;
                }
                if (args[0].length == 0) {
                    break;
                }
                topic_controller.save(args[0]);
                break;
            }
        case "load":
            if (args.length < 1) {
                console.warn("Usage: load <file>");
                return;
            }
            if (args[0].length == 0)
                break;
            topic_controller.load(args[0]);
            break;
        case "touch":
            {
                if (args.length < 1) {
                    console.warn("Usage: touch <name>");
                    return;
                }
                // let joinConcept = null;
                // if (args[args.length - 1][0] == ">") {
                //     joinConcept = args[args.length - 1].substring(1); // remove the ">" prefix
                //     args.pop();
                // }

                for (let i = 0; i < args.length; i++) {
                    const topicName = args[i];
                    if (topicName.length == 0)
                        continue;
                    topic_controller.createTopic(topicName);
                    // if (joinConcept) {
                    //     topic_controller.join(topicName, joinConcept, ENUMS.EdgeType.Example);
                    // }
                }

                break;
            }
        case "join":
            {
                if (args.length < 2 || args.length > 3) {
                    console.warn("Usage: join <topicA> <topicB>");
                    return;
                }
                let type = ENUMS.EdgeType.ComposedOf;
                if (args[0] === "-ComposedOf") {
                    type = ENUMS.EdgeType.ComposedOf;
                    args.shift(); // Remove the flag from args
                }
                if (args[0] === "-DependsOn") {
                    type = ENUMS.EdgeType.DependsOn;
                    args.shift(); // Remove the flag from args
                }
                if (args[0] === "-AlternativeTo") {
                    type = ENUMS.EdgeType.AlternativeTo;
                    args.shift(); // Remove the flag from args
                }
                if (args[0] === "-RelatedTo") {
                    type = ENUMS.EdgeType.RelatedTo;
                    args.shift(); // Remove the flag from args
                }
                let topicA = args[0];
                let topicB = args[1];
                topic_controller.join(topicA, topicB, type);
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
                if (args.length < 1) {
                    return;
                }
                if (args[0] == "join") {
                    if (args.length < 3)
                        return;
                    let topicA = args[1];
                    let topicB = args[2];
                    topic_controller.noJoin(topicA, topicB);
                } else if (args[0] == "path") {
                    topic_controller.noPath();
                }
                break;
            }
        case "path":
            {
                if (args.length < 2) {
                    return;
                }
                let topicA = args[0];
                let topicB = args[1];
                topic_controller.path(topicA, topicB);
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
        default:
            console.warn("Unknown command:", command);
        }
    }
    Shortcut {
        sequence: [":"]
        onActivated: commandInput.focus = true
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
                border.width: 2
                border.color: app.focusItem == canvas ? Colors.accent : "transparent"

                Text {
                    text: `v${app.major}.${app.minor}.${app.patch}`
                    font.bold: true
                    font.pointSize: 14
                    color: "#F5F5F5"
                    padding: 10
                }

                CanvasView {
                    id: canvas
                    anchors.fill: parent
                    clip: true
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
                MouseArea {
                    enabled: app.focusItem != canvas
                    anchors.fill: parent
                    propagateComposedEvents: true
                    onClicked: {
                        app.focusItem = canvas;
                        canvas.forceActiveFocus();
                    }
                }
            }

            Rectangle {
                id: command_interface
                color: Colors.accent
                Layout.fillWidth: true
                Layout.preferredHeight: 75
                border.width: 2
                border.color: app.focusItem == commandInput ? Colors.primary : "transparent"
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
                        // onTextChanged: topic_controller.updateBuffer(commandInput.text)
                        onFocusChanged: {
                            if (focus) {
                                app.focusItem = commandInput;
                            }
                        }
                        Keys.onReturnPressed: {
                            const cmd = commandInput.text.trim();
                            if (cmd.length === 0)
                                return;
                            app.parseCommand(cmd);
                            commandInput.text = "";
                        }
                        // Keys.onTabPressed: {
                        //     if (commandInput.text.length == 0)
                        //         return;
                        //     let suggestion = topic_controller.handleAutoComplete();
                        //     commandInput.text = suggestion;
                        // }
                        Keys.onEscapePressed: {
                            app.focusItem = canvas;
                            canvas.forceActiveFocus();
                            commandInput.focus = false;
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
                border.width: 2
                border.color: app.focusItem == topicListView ? Colors.accent : "transparent"
                TopicListView {
                    id: topicListView
                    model: topic_controller.topicListModel
                    anchors.fill: parent
                    anchors.topMargin: 15
                }
                MouseArea {
                    enabled: app.focusItem != topicListView
                    anchors.fill: parent
                    propagateComposedEvents: true
                    onClicked: {
                        app.focusItem = topicListView;
                        topicListView.forceActiveFocus();
                    }
                }
            }
        }
    }
    Component.onCompleted: {
        app.focusItem = canvas;
    }
}
