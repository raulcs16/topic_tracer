import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Styles
import TopicList

ApplicationWindow {
    id: app
    width: 2074
    height: 1296
    title: qsTr("Topic Tracer")
    visible: true

    property string activeView: "main_content"

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
                focus: true
                MouseArea {
                    anchors.fill: parent
                    onClicked: app.activeView = "main_content"
                }
            }

            Rectangle {
                id: status_bar
                color: Colors.accent
                Layout.fillWidth: true
                Layout.preferredHeight: 75
            }
        }

        // Sidebar
        ColumnLayout {
            spacing: 0
            Layout.preferredWidth: 300 // fixed width
            Layout.maximumWidth: 500
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

                border.width: app.activeView === "topic_list" ? 3 : 0

                border.color: Colors.accent
                TopicListView {
                    id: topicList
                    anchors.fill: parent
                    anchors.topMargin: 15

                    // MouseArea {
                    //     anchors.fill: parent
                    //     enabled: app.activeView != "topic_list"
                    //     onClicked: app.activeView = "topic_list"
                    // }
                }
            }
        }
    }
    Component.onCompleted: {
        main_content.forceActiveFocus();
    }
}
