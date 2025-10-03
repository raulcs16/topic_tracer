import QtQuick

Canvas {
    required property double sourceX
    required property double sourceY
    required property double targetX
    required property double targetY
    required property var bends

    property color color: '#ffffff'

    property int minX: Math.min(sourceX, targetX)
    property int minY: Math.min(sourceY, targetY)
    property int maxX: Math.max(sourceX, targetX)
    property int maxY: Math.max(sourceY, targetY)
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
            if (i == 0) {
                context.moveTo(bends[i].x - x, bends[i].y - y);
            } else {
                context.lineTo(bends[i].x - x, bends[i].y - y);
            }
        }
        // Draw head.
        context.stroke();
        context.restore();
    }
}
