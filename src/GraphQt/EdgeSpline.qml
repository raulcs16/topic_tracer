import QtQuick

Canvas {
    required property double sourceX
    required property double sourceY
    required property double targetX
    required property double targetY
    required property var bends

    property int headSize: 10
    property color color: '#ffffff'

    property int minX: Math.min(sourceX, targetX)
    property int minY: Math.min(sourceY, targetY)
    property int maxX: Math.max(sourceX, targetX)
    property int maxY: Math.max(sourceY, targetY)
    x: minX - headSize / 2
    y: minY - headSize / 2
    width: maxX - minX + headSize + 1
    height: maxY - minY + headSize + 1
    antialiasing: true
    onHeadSizeChanged: requestPaint()
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
        var fromX = bends[0].x - x;
        var fromY = bends[0].y - y;
        var toX = bends[bends.length - 1].x - x;
        var toY = bends[bends.length - 1].y - y;
        var angle = Math.atan2(toY - fromY, toX - fromX);
        context.lineTo(toX - headSize * Math.cos(angle - Math.PI / 8), toY - headSize * Math.sin(angle - Math.PI / 8));
        context.moveTo(toX, toY);
        context.lineTo(toX - headSize * Math.cos(angle + Math.PI / 8), toY - headSize * Math.sin(angle + Math.PI / 8));
        // Pop.
        context.stroke();
        context.restore();
    }
}
