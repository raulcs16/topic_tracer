import QtQuick

Canvas {
    property real viewWidth: 0
    property real viewHeight: 0
    required property double sourceX
    required property double sourceY
    required property double targetX
    required property double targetY
    required property var bends

    property color color: '#ffffff'

    // Compute translated coordinates (centered)
    property real sX: (viewWidth / 3) + sourceX
    property real sY: (viewHeight / 3) + sourceY
    property real tX: (viewWidth / 3) + targetX
    property real tY: (viewHeight / 3) + targetY

    // Compute bounding box (so the Canvas knows what to paint)
    property real minX: Math.min(sX, tX)
    property real minY: Math.min(sY, tY)
    property real maxX: Math.max(sX, tX)
    property real maxY: Math.max(sY, tY)
    x: minX
    y: minY
    width: maxX - minX
    height: maxY - minY
    antialiasing: true
    onColorChanged: requestPaint()
    onPaint: {
        var context = getContext('2d');
        // Push.
        context.save();
        context.clearRect(0, 0, width, height);
        context.strokeStyle = color;
        context.lineWidth = 1;
        context.beginPath();
        // Draw line.
        for (var i = 0; i < bends.length; ++i) {
            const px = (viewWidth / 3) + bends[i].x - x;
            const py = (viewHeight / 3) + bends[i].y - y;
            if (i === 0)
                context.moveTo(px, py);
            else
                context.lineTo(px, py);
        }
        // Draw head.
        context.stroke();
        context.restore();
    }
}
