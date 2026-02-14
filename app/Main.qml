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
                border.color: app.focusItem == viewport ? Colors.accent : "transparent"

                Text {
                    text: `v${app.major}.${app.minor}.${app.patch}`
                    font.bold: true
                    font.pointSize: 14
                    color: "#F5F5F5"
                    padding: 10
                }

                ViewPort {
                    id: viewport
                    anchors.fill: parent
                    clip: true
                    GraphView {
                        edgeModel: topic_controller.edgeListModel
                        nodeModel: topic_controller.nodeListModel
                        rectModel: topic_controller.rectListModel
                        anchors.fill: parent
                        property var highlightedNode: null
                        property var highlightedEdge: null
                        viewWidth: main_content.width
                        viewHeight: main_content.height
                    }
                    Connections {
                        target: topic_controller.rectListModel
                        function onSceneBoundsChanged() {
                            let rect = topic_controller.rectListModel.sceneBounds;
                            viewport.fitArea(rect.x, rect.y, rect.width, rect.height);
                        }
                    }
                    Keys.onPressed: event => {
                        if (event.key === Qt.Key_Plus || event.key === Qt.Key_Equal) {
                            viewport.zoomIn();
                            event.accepted = true;
                        } else if (event.key === Qt.Key_Minus) {
                            viewport.zoomOut();
                            event.accepted = true;
                        }
                        if (event.key === Qt.Key_K) {
                            viewport.panUp();
                            event.accepted = true;
                        }
                        if (event.key === Qt.Key_J) {
                            viewport.panDown();
                            event.accepted = true;
                        }
                        if (event.key === Qt.Key_H) {
                            viewport.panLeft();
                            event.accepted = true;
                        }
                        if (event.key === Qt.Key_L) {
                            viewport.panRight();
                            event.accepted = true;
                        }
                        if (event.key === Qt.Key_0) {
                            viewport.resetView();
                            event.accepted = true;
                        }
                        if (event.key == Qt.Key_1) {
                            viewport.toggleGrid();
                            event.accepted = true;
                        }
                        if (event.key === Qt.Key_X) {
                            // Test: Fit to a random 400x400 area in the world
                            let randomX = Math.random() * 1000 - 500;
                            let randomY = Math.random() * 1000 - 500;
                            console.log("Fitting to random area at: ", randomX, randomY);

                            viewport.fitArea(randomX, randomY, 400, 400);
                            event.accepted = true;
                        }
                    }
                    MouseArea {
                        enabled: app.focusItem != viewport
                        anchors.fill: parent
                        propagateComposedEvents: true
                        onClicked: {
                            app.focusItem = viewport;
                            viewport.forceActiveFocus();
                        }
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
                        onFocusChanged: {
                            if (focus) {
                                app.focusItem = commandInput;
                            }
                        }
                        Keys.onReturnPressed: {
                            topic_controller.executeCommand(commandInput.text.trim());
                            commandInput.text = "";
                        }
                        Keys.onTabPressed: {
                            commandInput.text = topic_controller.getAutoComplete(commandInput.text.trim());
                        }
                        Keys.onEscapePressed: {
                            app.focusItem = viewport;
                            viewport.forceActiveFocus();
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
        app.focusItem = viewport;
    }
}
