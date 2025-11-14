pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic // Keep Basic for compatibility if necessary

import Styles
import TopicGraph

Item {
    id: root

    height: parent.height
    width: parent.width

    required property TopicListModel model

    Menu {
        id: listMenu
        background: MenuBackground {}
        MenuItemCard {
            text: qsTr("add topic")
            onTriggered: {
                root.model.isAddingNewTopic = true;
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
                root.model.addFlags(delegateMenu.clickedIndex, ENUMS.StateFlag.EditMode);
            }
        }
        MenuItemCard {
            text: qsTr("Delete")
            onTriggered: {
                root.model.removeItem(delegateMenu.clickedIndex);
            }
        }
    }

    ListView {
        id: topicView
        anchors.fill: parent
        bottomMargin: 30
        model: root.model
        delegate: TopicDelegate {
            model: root.model
            onContextMenuRequested: (i, pos) => {
                delegateMenu.clickedIndex = i;
                const global = mapToItem(topicView, pos.x, pos.y);
                delegateMenu.x = global.x;
                delegateMenu.y = global.y;
                delegateMenu.open();
            }
        }
        orientation: ListView.Vertical
        focus: true
        footer: addTopicInput

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
            visible: root.model.isAddingNewTopic
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
                        root.model.addItem(topicName);
                        root.model.currentIndex = root.model.rowCount() - 1;
                    }
                    text = "";
                    root.model.isAddingNewTopic = false;
                    topicView.focus = true;
                }
                Keys.onEscapePressed: {
                    console.log("Escaping..");
                    root.model.isAddingNewTopic = false;
                }
                onFocusChanged: {
                    if (!focus) {
                        root.model.isAddingNewTopic = false;
                        topicView.focus = true;
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
