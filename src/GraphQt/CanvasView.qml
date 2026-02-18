pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root
    anchors.fill: parent
    focus: true
    clip: true

    property real zoom: 1.0
    property real minZoom: 0.2
    property real maxZoom: 3.0
    property int panStep: 30
    property bool gridOn: false
    default property alias content: contentItem.data

    Canvas {
        id: gridCanvas
        anchors.fill: parent
        z: -1 // Behind panContainer
        visible: root.gridOn
        onVisibleChanged: gridCanvas.requestPaint()
        onPaint: {
            if (!root.gridOn)
                return;
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height); // Clear previous frame

            // Set grid style
            ctx.lineWidth = 1;
            ctx.strokeStyle = '#54efe6e6'; // Very subtle white (10% opacity)

            // Calculate the current grid size based on zoom
            // We want the grid to grow and shrink with the content
            let size = 20 * root.zoom;

            // This is the "magic":
            // We use the modulo (%) operator to find the remainder of the pan.
            // This makes the grid lines 'wrap around' so they never run out.
            let offsetX = panContainer.x % size;
            let offsetY = panContainer.y % size;

            ctx.beginPath();

            // Draw Vertical Lines
            for (let x = offsetX; x < width; x += size) {
                ctx.moveTo(x, 0);
                ctx.lineTo(x, height);
            }

            // Draw Horizontal Lines
            for (let y = offsetY; y < height; y += size) {
                ctx.moveTo(0, y);
                ctx.lineTo(width, y);
            }

            ctx.stroke();
        }

        // We need to tell the Canvas to redraw whenever you pan or zoom
        Connections {
            target: panContainer
            function onXChanged() {
                gridCanvas.requestPaint();
            }
            function onYChanged() {
                gridCanvas.requestPaint();
            }
        }
        Connections {
            target: root
            function onZoomChanged() {
                gridCanvas.requestPaint();
            }
        }
    }
    Item {
        id: panContainer
        width: childrenRect.width
        height: childrenRect.height

        Item {
            id: contentItem
            scale: root.zoom
            transformOrigin: Item.Center

            Rectangle {
                width: 10
                height: 10
                radius: 100
                color: "red"
                opacity: 0.5
                x: (root.width / 2) - (width / 2)
                y: (root.height / 2) - (height / 2)
            }
        }
    }

    function resetView() {
        panContainer.x = 0;
        panContainer.y = 0;
        root.zoom = 1.0;
    }
    DragHandler {
        id: dragHandler
        target: panContainer // Moves the container, not the scaled content
    }
    WheelHandler {
        onWheel: event => {
            let zoomFactor = event.angleDelta.y > 0 ? 1.1 : 0.9;
            root.zoom = Math.min(root.maxZoom, Math.max(root.minZoom, root.zoom * zoomFactor));
        }
    }
    PinchArea {
        anchors.fill: parent
        pinch.target: contentItem
        onPinchUpdated: {
            let newZoom = Math.min(root.maxZoom, Math.max(root.minZoom, root.zoom * parent.scale));
            root.zoom = newZoom;
        }
    }

    Keys.onPressed: event => {
        // Zoom logic
        if (event.key === Qt.Key_Plus || event.key === Qt.Key_Equal) {
            root.zoom = Math.min(maxZoom, root.zoom * 1.1);
            event.accepted = true;
        } else if (event.key === Qt.Key_Minus) {
            root.zoom = Math.max(minZoom, root.zoom * 0.9);
            event.accepted = true;
        }

        // Pan logic (Relative to zoom)
        // We multiply by (1/zoom) if you want panning speed to feel
        // consistent regardless of how far zoomed in you are.
        let adjustedStep = panStep / root.zoom;

        if (event.key === Qt.Key_K) {
            panContainer.y += adjustedStep;
            event.accepted = true;
        }
        if (event.key === Qt.Key_J) {
            panContainer.y -= adjustedStep;
            event.accepted = true;
        }
        if (event.key === Qt.Key_H) {
            panContainer.x += adjustedStep;
            event.accepted = true;
        }
        if (event.key === Qt.Key_L) {
            panContainer.x -= adjustedStep;
            event.accepted = true;
        }

        // Reset view with '0' key
        if (event.key === Qt.Key_0) {
            resetView();
            event.accepted = true;
        }
        if (event.key == Qt.Key_1) {
            root.gridOn = !root.gridOn;

            event.accepted = true;
        }
    }
    TapHandler {
        onTapped: root.forceActiveFocus()
    }
}
