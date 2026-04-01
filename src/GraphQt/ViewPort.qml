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
    Behavior on centerX {
        NumberAnimation {
            duration: 400
            easing.type: Easing.OutCubic
        }
    }
    Behavior on centerY {
        NumberAnimation {
            duration: 400
            easing.type: Easing.OutCubic
        }
    }
    Behavior on zoom {
        NumberAnimation {
            duration: 400
            easing.type: Easing.OutCubic
        }
    }
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

    function fitArea(rectX, rectY, rectW, rectH,nodeX,nodeY) {
        const minZoom = 0.15; // Don't let clusters get smaller than this
        const maxZoom = 1.2;  // Don't zoom in more than this
        const paddingBuffer = 60;
    
        const usableWidth = Math.max(width - (paddingBuffer * 2), 1);
        const usableHeight = Math.max(height - (paddingBuffer * 2), 1);

        // Calculate zoom needed to see the whole cluster
        let clusterZoomX = usableWidth / Math.max(rectW, 1);
        let clusterZoomY = usableHeight / Math.max(rectH, 1);
        let idealClusterZoom = Math.min(clusterZoomX, clusterZoomY);

        // 2. Apply the "Focus" Logic
        // If the cluster zoom is too small (meaning the cluster is huge), 
        // we clamp the zoom and shift the center towards the specific node.
        if (idealClusterZoom < minZoom) {
            zoom = minZoom;
            // When cluster is too big, center directly on the node
            centerX = nodeX;
            centerY = nodeY;
        } else {
            // Cluster fits reasonably well
            zoom = Math.min(idealClusterZoom, maxZoom);

            // Center on the cluster middle
            centerX = rectX + (rectW / 2);
            centerY = rectY + (rectH / 2);
        }
        // 1. Calculate the center of the target cluster
        // centerX = rectX + (rectW / 2);
        // centerY = rectY + (rectH / 2);

        // const minWorldWidth = 500;
        // const minWorldHeight = 500;

        // // 2. Ensure we don't zoom in too far on tiny clusters
        // let effectiveW = Math.max(rectW, minWorldWidth);
        // let effectiveH = Math.max(rectH, minWorldHeight);

        // // 3. Define explicit padding in pixels (e.g., 40px on all sides)
        // const paddingBuffer = 60;
        // const usableWidth = width - (paddingBuffer * 2);
        // const usableHeight = height - (paddingBuffer * 2);

        // // 4. Calculate zoom for both axes based on usable space
        // let zoomX = usableWidth / effectiveW;
        // let zoomY = usableHeight / effectiveH;

        // // 5. Use Math.min to ensure the "most restrictive" dimension
        // // forces the other to stay within bounds.
        // zoom = Math.min(zoomX, zoomY);
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
