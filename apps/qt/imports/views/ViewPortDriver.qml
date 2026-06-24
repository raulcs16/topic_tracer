import QtQuick

QtObject {

    property real centerX: 0
    property real centerY: 0
    property real zoom: 1.0
    property real panStep: 30
    property bool gridOn: false

    readonly property real adjustedStep: panStep / zoom

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

    function fitArea(rectX, rectY, rectW, rectH, xpos, ypos, viewWidth, viewHeight) {
        const padding = 100;
        const minReadableZoom = 0.7;
        const maxZoom = 1.2;

        const clusterCenterX = rectX + (rectW / 2);
        const clusterCenterY = rectY + (rectH / 2);

        let zoomX = (viewWidth - padding * 2) / Math.max(rectW, 1);
        let zoomY = (viewHeight - padding * 2) / Math.max(rectH, 1);
        let clusterZoom = Math.min(zoomX, zoomY);

        if (clusterZoom < minReadableZoom) {
            zoom = minReadableZoom;
            centerX = xpos;
            centerY = ypos;
        } else {
            zoom = Math.min(clusterZoom, maxZoom);
            centerX = clusterCenterX;
            centerY = clusterCenterY;
        }
    }
}
