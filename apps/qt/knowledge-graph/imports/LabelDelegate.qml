pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Item {
    id: root

    required property int itemId
    required property string label
    required property bool selected
    required property bool editMode
    required property bool hovered

    signal contextMenuRequested(point pos)
    signal mouseOver
    signal mouseExit
    signal clicked

    height: 30
    width: parent.width
    //background
    Rectangle {
        visible: true
        anchors.fill: parent
        color: root.selected ? "#000" : "#222"
        opacity: root.selected ? 0.6 : root.hovered ? 0.2 : 0.0
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onEntered: root.mouseOver()
        onExited: root.mouseExit()

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: mouse => {
            if (mouse.button == Qt.RightButton)
                root.contextMenuRequested(Qt.point(mouse.x, mouse.y));
            else {
                root.clicked();
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
        text: root.label
        font.pointSize: 16
        font.weight: Font.DemiBold
        color: "#fff"
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
        placeholderText: root.label
        font.pointSize: 16
        color: "#00ffff"

        background: Rectangle {
            color: "#002200"
            border.width: 2
            border.color: "#222"
        }
    }
}
