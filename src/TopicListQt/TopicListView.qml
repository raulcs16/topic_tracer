pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls // Use standard Controls for Menu/TextField
import QtQuick.Controls.Basic // Keep Basic for compatibility if necessary

import Styles
import TopicList

Item {
    id: root

    height: parent.height
    width: parent.width

    property bool isAddingNewTopic: false

    TopicListModel {
        id: topicListModel
    }

    Menu {
        id: listMenu
        background: Rectangle {
            radius: 6
            color: Colors.secondary
            border.color: Colors.accent
            border.width: 1
            implicitWidth: 200
        }
        MenuItem {
            id: menuItem
            text: qsTr("add topic")
            onTriggered: {
                root.isAddingNewTopic = true;
                topicView.contentY = topicView.contentHeight - topicView.height;
            }
            background: Rectangle {
                color: "transparent"
                implicitWidth: 150
                implicitHeight: 30
            }
            contentItem: Text {
                text: menuItem.text
                font.pixelSize: 16
                font.weight: Font.DemiBold
                color: "white"
                verticalAlignment: Text.AlignVCenter
                leftPadding: 12
            }
        }
    }

    Menu {
        id: delegateMenu
        property int clickedIndex: -1
        background: Rectangle {
            radius: 6
            color: Colors.secondary
            border.color: Colors.accent
            border.width: 1
            implicitWidth: 200
        }
        MenuItem {
            id: delegateItem
            text: qsTr("Rename...")
            onTriggered:
            //should make the current item editable
            {
                topicView.editingIndex = delegateMenu.clickedIndex;
            }
            background: Rectangle {
                color: "transparent"
                implicitWidth: 150
                implicitHeight: 30
            }
            contentItem: Text {
                text: delegateItem.text
                font.pixelSize: 16
                font.weight: Font.DemiBold
                color: "white"
                verticalAlignment: Text.AlignVCenter
                leftPadding: 12
            }
        }
    }

    ListView {
        id: topicView
        anchors.fill: parent
        bottomMargin: 30
        model: topicListModel
        delegate: topicDelegate
        orientation: ListView.Vertical
        focus: true
        property int editingIndex: -1

        footer: addTopicInput

        highlight: itemHighlight

        MouseArea {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.contentItem.bottom // 👈 start after last delegate
                bottom: parent.bottom
            }
            visible: topicView.contentHeight < topicView.height
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: mouse => {
                if (mouse.button == Qt.RightButton) {
                    const pos = mapToItem(topicView, mouse.x, mouse.y);
                    listMenu.x = pos.x;
                    listMenu.y = pos.y;
                    listMenu.open();
                }
            }
        }
    }

    Component {
        id: addTopicInput

        Item {
            // Only visible when the "add topic" menu item is clicked
            visible: root.isAddingNewTopic
            height: visible ? 30 : 0 // Collapse the height when not visible
            width: parent.width

            TextField {
                id: newTopicTextField
                anchors.fill: parent
                font.pointSize: 16
                anchors.leftMargin: 20
                color: Colors.text_secondary
                background: Rectangle {
                    color: Colors.secondary
                    border {
                        width: 2
                        color: Colors.accent
                    }
                }

                Keys.onReturnPressed: {
                    let topicName = text.trim();
                    if (topicName.length > 0) {
                        topicListModel.addItem(topicName);
                        topicView.currentIndex = topicListModel.rowCount() - 1;
                    }
                    text = "";
                    root.isAddingNewTopic = false;
                    topicView.focus = true;
                }
                onFocusChanged: {
                    if (!focus) {
                        root.isAddingNewTopic = false;
                        topicView.focus = true; // Restore focus to the ListView
                    }
                }
            }
        }
    }

    Component {
        id: topicDelegate

        Rectangle {
            id: delegateRect

            required property string topicName
            required property int index

            property ItemView listView: ListView.view

            height: 20
            width: parent.width

            color: "transparent"

            Rectangle {
                id: hoverBackground
                anchors.fill: parent
                color: Colors.selected
                opacity: 0.0
                z: -1 // make sure it’s behind the text
            }

            TextField {
                id: editor
                visible: topicView.editingIndex === delegateRect.index // Show if this index is being edited

                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20

                text: delegateRect.topicName // Initial value is the current topic name
                font.pointSize: 16
                color: Colors.text_secondary

                background: Rectangle {
                    color: Colors.secondary
                    border.width: 2
                    border.color: Colors.accent
                }

                Keys.onReturnPressed: {
                    let newName = text.trim();
                    if (newName.length > 0 && newName !== delegateRect.topicName) {
                        topicListModel.editItem(delegateRect.index, newName);
                    }
                    // Exit editing mode regardless
                    topicView.editingIndex = -1;
                    topicView.focus = true;
                }

                onFocusChanged: {
                    if (!focus) {
                        // Exiting edit mode saves the current value (or simply exits)
                        topicView.editingIndex = -1;
                    }
                }
            }
            Text {
                visible: topicView.editingIndex !== delegateRect.index
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
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                cursorShape: Qt.PointingHandCursor
                onEntered: hoverBackground.opacity = 0.2
                onExited: hoverBackground.opacity = 0.0

                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: mouse => {
                    if (mouse.button == Qt.RightButton) {
                        delegateMenu.clickedIndex = delegateRect.index;
                        const pos = mapToItem(topicView, mouse.x, mouse.y);
                        delegateMenu.x = pos.x;
                        delegateMenu.y = pos.y;
                        delegateMenu.open();
                    } else {
                        delegateRect.listView.currentIndex = delegateRect.index;
                    }
                }
            }
        }
    }

    Component {
        id: itemHighlight
        Rectangle {
            border.color: "black"
            color: Colors.selected
            opacity: 0.40
        }
    }
}
