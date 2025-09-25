pragma ComponentBehavior: Bound
import QtQuick
//import QtQuick.Controls
import QtQuick.Layouts

import Styles // Assumes a file named Styles.qml exists

Item {
    id: root

    height: parent.height
    width: parent.width

    required property AbstractItemModel listModel
    required property bool viewInteraction
    property alias topicView: topicView

    // Main ListView for displaying the topics
    ListView {
        id: topicView
        anchors.fill: parent

        model: root.listModel
        delegate: readDelegate

        orientation: ListView.Vertical
        spacing: 5
        //color boder on interactive / when user is focused on this view
    }

    Component {
        id: readDelegate

        Rectangle {
            id: delegateRect

            required property string topicName
            property ItemView listView: ListView.view

            height: 20
            width: parent.width

            color: "transparent"

            Text {
                text: delegateRect.topicName
                font.pointSize: 16
                font.weight: Font.DemiBold
                color: Colors.text_secondary
                Layout.fillWidth: true
                elide: Text.ElideRight
                anchors {
                    left: parent.left
                    leftMargin: 20
                    verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
