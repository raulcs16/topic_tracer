pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic // Keep Basic for compatibility if necessary

import Styles
import TopicList
import TopicGraph

Item {
    id: root

    height: parent.height
    width: parent.width

    TopicGraphController{
        id: _controller
    }
    TopicListModel {
        id: topicListModel
        controller:_controller
        isAddingNewTopic: false
        currentIndex: 1
    }

    Menu {
        id: listMenu
        background: MenuBackground {}
        MenuItemCard {
            text: qsTr("add topic")
            onTriggered: {
                topicListModel.isAddingNewTopic = true;
                topicView.contentY = topicView.contentHeight - topicView.height;
            }
        }
    }

    Menu {
        id: delegateMenu
        property int clickedIndex: -1
        background: MenuBackground {}
        MenuItemCard {
            text: qsTr("Rename...")
            onTriggered: {
                topicListModel.editingIndex = delegateMenu.clickedIndex;
            }
        }
        MenuItemCard {
            text: qsTr("Delete")
            onTriggered: {
                topicListModel.removeItem(delegateMenu.clickedIndex);
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
        currentIndex: model.currentIndex

        focus: true
        footer: addTopicInput
        highlight: itemHighlight

        MouseArea {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.contentItem.bottom
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
            visible: topicListModel.isAddingNewTopic
            height: visible ? 30 : 0
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
                        topicListModel.currentIndex = topicListModel.rowCount() - 1;
                    }
                    text = "";
                    topicListModel.isAddingNewTopic = false;
                    topicView.focus = true;
                }
                onFocusChanged: {
                    if (!focus) {
                        topicListModel.isAddingNewTopic = false;
                        topicView.focus = true;
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
            required property bool pending
            required property int index

            property ItemView listView: ListView.view

            height: 30
            width: listView.width

            color: "transparent"

            Rectangle {
                id: hoverBackground
                anchors.fill: parent
                color: Colors.selected
                opacity: 0.0
                z: -1
            }

            TextField {
                id: editor
                visible: topicListModel.editingIndex === delegateRect.index

                anchors.fill: parent
                anchors.leftMargin: 20
                placeholderText: delegateRect.topicName
                font.pointSize: 16
                color: Colors.text_secondary

                background: Rectangle {
                    color: Colors.primary
                    border.width: 2
                    border.color: Colors.accent
                }

                Keys.onReturnPressed: {
                    let newName = text.trim();
                    if (newName.length > 0 && newName !== delegateRect.topicName) {
                        topicListModel.editItem(delegateRect.index, newName);
                    }
                    topicListModel.editingIndex = -1;
                    topicView.focus = true;
                }

                onFocusChanged: {
                    if (!focus) {
                        topicListModel.editingIndex = -1;
                    }
                }
            }
            Text {
                visible: topicListModel.editingIndex !== delegateRect.index
                text: delegateRect.topicName
                font.pointSize: 16
                font.weight: Font.DemiBold
                color: delegateRect.pending? Colors.primary:Colors.text_secondary
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

    component MenuItemCard: MenuItem {
        id: rootItem
        background: Rectangle {
            color: "transparent"
            implicitWidth: 150
            implicitHeight: 30
        }
        contentItem: Text {
            text: rootItem.text
            font.pixelSize: 16
            font.weight: Font.DemiBold
            color: "white"
            verticalAlignment: Text.AlignVCenter
            leftPadding: 12
        }
    }
    component MenuBackground: Rectangle {
        radius: 6
        color: Colors.secondary
        border.color: Colors.accent
        border.width: 1
        implicitWidth: 200
    }
}
