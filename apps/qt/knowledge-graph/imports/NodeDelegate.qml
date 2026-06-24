pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root

    required property int itemId
    required property string label
    required property point position
    required property double heatScore

    required property bool hidden
    required property bool selected
    required property bool editMode
    required property bool hovered
    required property bool highlighted

    signal onMouseOver
    signal onMouseExit
    signal onSelected
    signal onContextMenu(point pos)

    readonly property double posx: position.x
    readonly property double posy: position.y

    function heatColor(heat) {
        if (heat <= 0.001)
            return Qt.rgba(0.9, 0.9, 0.9, 1.0); // Dark grey pops better on black

        let t = Math.pow(Math.max(0, Math.min(heat, 1.0)), 0.8);
        let r = 0, g = 0, b = 0;

        if (t <= 0.25) {
            // Blue → Cyan
            r = 0;
            g = Math.round(255 * (t / 0.25));
            b = 255;
        } else if (t <= 0.5) {
            // Cyan → Green
            r = 0;
            g = 255;
            b = Math.round(255 * (1 - (t - 0.25) / 0.25));
        } else if (t <= 0.75) {
            // Green → Yellow
            r = Math.round(255 * ((t - 0.5) / 0.25));
            g = 255;
            b = 0;
        } else {
            // Yellow → Red
            r = 255;
            g = Math.round(255 * (1 - (t - 0.75) / 0.25));
            b = 0;
        }

        return Qt.rgba(r, g, b, 1.0);
    }
    function borderColor() {
        let color = "#af9476";
        if (root.highlighted) {
            color = "yellow";
        }
        if (root.hovered) {
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
        onEntered: root.onMouseOver()
        onExited: root.onMouseExit()
        onClicked: mouse => {
            if (mouse.button == Qt.RightButton)
                root.onContextMenu(Qt.point(mouse.x, mouse.y));
            else {
                const mods = mouse.modifiers;
                const meta = mods & Qt.MetaModifier || mods & Qt.ControlModifier;
                root.onSelected();
            }
        }
    }
    Text {
        text: root.label
        color: "white"
        anchors.top: node.bottom
        anchors.horizontalCenter: node.horizontalCenter
        anchors.topMargin: 4
        font.pointSize: root.hovered | root.selected ? 16 : 12
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
