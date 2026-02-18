pragma ComponentBehavior: Bound
import QtQuick

import GraphQt

Item {
    id: root

    required property NodeListModel model

    required property int index
    required property int topicId
    required property string label
    required property double posx
    required property double posy
    required property int flags
    required property int heatScore

    readonly property bool hover: (flags & ENUMS.StateFlag.Hovered) !== 0
    readonly property bool selected: (flags & ENUMS.StateFlag.Selected) !== 0
    readonly property bool highlight: (flags & ENUMS.StateFlag.InPath) !== 0
    readonly property bool hidden: (flags & ENUMS.StateFlag.Hidden) !== 0

    function heatColor(heat) {
        if (heat <= 0)
            return Qt.rgba(0.9, 0.9, 0.9, 1.0);
        let t = Math.min(heat / 100, 1);
        t = Math.sqrt(t);
        let r = 0.8 * t + 0.2;
        let g = 0.5 * (1 - t);
        let b = 0.2 * (1 - t);
        return Qt.rgba(r, g, b, 1.0);
    }
    function borderColor() {
        let color = "#af9476";
        if (root.highlight) {
            color = "yellow";
        }
        if (root.hover) {
            color = '#f37100';
        }
        if (root.selected) {
            color = '#1a06f3';
        }
        return color;
    }
    width: 20
    height: 20
    x: posx - width / 2
    y: posy - height / 2
    Rectangle {
        id: node
        anchors.fill: parent
        color: root.heatColor(root.heatScore)
        opacity: root.hidden ? 0.1 : 1
        radius: 100
        border.width: root.selected ? 3 : 2
        border.color: root.borderColor()
    }
    MouseArea {
        z: 100
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onEntered: root.model.hoverRequested(root.topicId, true)
        onExited: root.model.hoverRequested(root.topicId, false)
        onClicked: mouse => {
            if (mouse.button == Qt.RightButton)
            //root.contextMenuRequested(root.index, Qt.point(mouse.x, mouse.y));
            {} else {
                const mods = mouse.modifiers;
                const meta = mods & Qt.MetaModifier || mods & Qt.ControlModifier;

                if (meta) {
                    root.model.toggleSelectionRequest(root.topicId);
                } else {
                    root.model.selectRequested(root.topicId);
                }
            }
        }
    }
    Text {
        text: root.label
        color: "white"
        anchors.top: node.bottom
        anchors.horizontalCenter: node.horizontalCenter
        anchors.topMargin: 4
        font.pointSize: root.hover | root.selected ? 16 : 12
    }
    Behavior on x {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }

    Behavior on y {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
}
