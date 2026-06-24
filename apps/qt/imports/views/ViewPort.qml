pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: viewport
    clip: true
    required property ViewPortDriver driver

    property real centerX: driver.centerX
    property real centerY: driver.centerY
    property real zoom: driver.zoom
    property real panStep: driver.panStep
    property bool gridOn: driver.gridOn

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
    default property alias content: contentItem.data
    readonly property real offsetX: (width / 2) - (centerX * zoom)
    readonly property real offsetY: (height / 2) - (centerY * zoom)
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
