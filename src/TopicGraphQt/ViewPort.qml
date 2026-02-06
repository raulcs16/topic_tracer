pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: viewport
    anchors.fill: parent
    clip: true
    // --- Control Interface (The Source of Truth) ---
    property real centerX: 0
    property real centerY: 0
    property real zoom: 1.0
    property real panStep: 30

    // --- State ---
    property bool gridOn: false
    default property alias content: contentItem.data

    // --- Camera Math ---
    // These calculate where the top-left of the world (0,0) should be
    // in order to keep centerX/Y in the middle of the screen.
    readonly property real offsetX: (width / 2) - (centerX * zoom)
    readonly property real offsetY: (height / 2) - (centerY * zoom)
    readonly property real adjustedStep: panStep / viewport.zoom

    // --- Viewport Control Functions ---
    function resetView() {
        centerX = 0;
        centerY = 0;
        zoom = 1.0;
    }

    function panRight() {
        centerX += adjustedStep;
    }
    function panLeft() {
        centerX -= adjustedStep;
    }
    function panUp() {
        centerY -= adjustedStep;
    }
    function panDown() {
        centerY += adjustedStep;
    }

    function zoomIn() {
        zoom *= 1.1;
    }
    function zoomOut() {
        zoom *= 0.9;
    }
    function toggleGrid() {
        gridOn = !gridOn;
    }

    function fitArea(rectX, rectY, rectW, rectH) {
        // 1. Center the camera on the middle of the provided bounding box
        centerX = rectX + (rectW / 2);
        centerY = rectY + (rectH / 2);

        // 2. Calculate zoom to fit with 10% padding
        let padding = 0.9;
        let zoomX = (width / rectW) * padding;
        let zoomY = (height / rectH) * padding;

        zoom = Math.min(zoomX, zoomY);
    }

    // --- Grid Rendering ---
    Canvas {
        id: gridCanvas
        anchors.fill: parent
        z: -1
        visible: viewport.gridOn
        onVisibleChanged: gridCanvas.requestPaint()
        onPaint: {
            if (!viewport.gridOn)
                return;
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
            ctx.lineWidth = 1;
            ctx.strokeStyle = '#54efe6e6';

            let size = 20 * viewport.zoom;
            let startX = viewport.offsetX % size;
            let startY = viewport.offsetY % size;

            ctx.beginPath();
            for (let x = startX; x < width; x += size) {
                ctx.moveTo(x, 0);
                ctx.lineTo(x, height);
            }
            for (let y = startY; y < height; y += size) {
                ctx.moveTo(0, y);
                ctx.lineTo(width, y);
            }
            ctx.stroke();
        }

        Connections {
            target: viewport
            function onOffsetXChanged() {
                gridCanvas.requestPaint();
            }
            function onOffsetYChanged() {
                gridCanvas.requestPaint();
            }
            function onZoomChanged() {
                gridCanvas.requestPaint();
            }
        }
    }

    // --- World Container ---
    Item {
        id: panContainer
        // The container follows the camera math automatically
        x: viewport.offsetX
        y: viewport.offsetY

        Item {
            id: contentItem
            scale: viewport.zoom
            // TopLeft ensures world coordinate (0,0) is our anchor for scaling
            transformOrigin: Item.TopLeft

            // World Origin Marker (True 0,0)
            Rectangle {
                width: 10
                height: 10
                radius: 5
                color: "red"
                opacity: 0.5
                // Center the dot on the 0,0 point
                x: -5
                y: -5
                z: 999
            }
        }
    }
}
