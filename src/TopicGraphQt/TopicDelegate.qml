pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import TopicGraph
import Styles

Item {
    id: root
    required property TopicListModel model

    required property int index
    required property int topicId
    required property string topicName
    required property bool pending
    required property int flags

    readonly property bool hover: (flags & ENUMS.StateFlag.Hovered) !== 0
    readonly property bool selected: (flags & ENUMS.StateFlag.Selected) !== 0
    readonly property bool editMode: (flags & ENUMS.StateFlag.EditMode) !== 0

    signal contextMenuRequested(int index, point pos)

    height: 30
    width: ListView.view.width

    //background
    Rectangle {
        visible: true
        anchors.fill: parent
        color: Colors.selected
        opacity: root.selected ? 0.6 : root.hover ? 0.2 : 0.0
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        cursorShape: Qt.PointingHandCursor

        onEntered: root.model.addFlags(root.index, ENUMS.StateFlag.Hovered)
        onExited: root.model.removeFlags(root.index, ENUMS.StateFlag.Hovered)

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: mouse => {
            if (mouse.button == Qt.RightButton)
                root.contextMenuRequested(root.index, Qt.point(mouse.x, mouse.y));
            else {
                const mods = mouse.modifiers;
                const meta = mods & Qt.MetaModifier || mods & Qt.ControlModifier;

                if (meta) {
                    root.model.toggleSelect(root.index);
                } else if (mods & Qt.ShiftModifier) {
                    root.model.rangeSelect(root.index);
                } else {
                    root.model.selectIndex(root.index);
                }
            }
        }
    }

    //STATES
    states: [
        State {
            name: "view"
            when: !root.editMode
            PropertyChanges {
                editor.visible: false
                editor.focus: false
            }
            PropertyChanges {
                textLabel.visible: true
            }
        },
        State {
            name: "edit"
            when: root.editMode

            PropertyChanges {
                editor.visible: true
            }
            PropertyChanges {
                textLabel.visible: false
            }
            StateChangeScript {
                script: editor.forceActiveFocus()
            }
        }
    ]
    Text {
        id: textLabel
        text: root.topicName
        font.pointSize: 16
        font.weight: Font.DemiBold
        color: root.pending ? Colors.primary : Colors.text_secondary
        Layout.fillWidth: true
        elide: Text.ElideRight
        anchors {
            left: parent.left
            leftMargin: 20
            verticalCenter: parent.verticalCenter
        }
    }
    TextField {
        id: editor
        anchors.fill: parent
        focus: root.editMode
        anchors.leftMargin: 20
        placeholderText: root.topicName
        font.pointSize: 16
        color: Colors.text_secondary

        background: Rectangle {
            color: Colors.primary
            border.width: 2
            border.color: Colors.accent
        }

        Keys.onReturnPressed: {
            const newName = text.trim();
            if (newName.length > 0)
                root.model.editItem(root.index, newName);
            root.model.removeFlags(root.index, ENUMS.StateFlag.EditMode);
        }
        Keys.onEscapePressed: {
            root.model.removeFlags(root.index, ENUMS.StateFlag.EditMode);
            text = root.topicName; // revert
        }
    }
    Component.onCompleted: {}
}
