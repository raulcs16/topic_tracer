pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root
    anchors.fill: parent
    focus: true

    property real zoom: 1.0
    property real minZoom: 0.2
    property real maxZoom: 3.0
    property int panStep: 30
    default property alias content: contentItem.data

    // This is the container we scale + translate
    Item {
        id: contentItem
        scale: root.zoom
        transformOrigin: Item.TopLeft

        DragHandler {
            id: pan
            target: contentItem
        }
    }

    // Pinch zoom (touchscreen)
    PinchArea {
        anchors.fill: parent
        pinch.target: contentItem
        onPinchUpdated: {
            let newZoom = Math.min(root.maxZoom, Math.max(root.minZoom, root.zoom * parent.scale));
            root.zoom = newZoom;
        }
    }

    // Mouse wheel zoom (desktop)
    Keys.onPressed: event => {
        if (event.key === Qt.Key_Plus || event.key === Qt.Key_Equal) {
            zoom = Math.min(maxZoom, zoom * 1.1);
            event.accepted = true;
        } else if (event.key === Qt.Key_Minus) {
            zoom = Math.max(minZoom, zoom * 0.9);
            event.accepted = true;
        }
    }

    // Keyboard panning
    Keys.onUpPressed: contentItem.y += panStep
    Keys.onDownPressed: contentItem.y -= panStep
    Keys.onLeftPressed: contentItem.x += panStep
    Keys.onRightPressed: contentItem.x -= panStep

    Component.onCompleted: root.forceActiveFocus()
}
