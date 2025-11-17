import QtQuick

Canvas {
    property real viewWidth: 0
    property real viewHeight: 0
    property int headSize: 10

    required property double sourceX
    required property double sourceY
    required property double targetX
    required property double targetY
    required property var bends
    required property int flags

    readonly property bool hover: (flags & ENUMS.StateFlag.Hovered) !== 0
    readonly property bool highlight: (flags & ENUMS.StateFlag.InPath) !== 0

    property color color: highlight ? "yellow" : hover ? '#4cb4e1' : '#ffffff'

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
        context.lineWidth = hover ? 2 : 1;
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
        var fromX = bends.length > 0 ? (viewWidth / 3 + bends[bends.length - 2].x - x) : (sX - x);
        var fromY = bends.length > 0 ? (viewHeight / 3 + bends[bends.length - 2].y - y) : (sY - y);
        var toX = bends.length > 0 ? (viewWidth / 3 + bends[bends.length - 1].x - x) : (tX - x);
        var toY = bends.length > 0 ? (viewHeight / 3 + bends[bends.length - 1].y - y) : (tY - y);

        // Draw arrowhead like the original style
        var angle = Math.atan2(toY - fromY, toX - fromX);
        context.lineTo(toX - headSize * Math.cos(angle - Math.PI / 8), toY - headSize * Math.sin(angle - Math.PI / 8));
        context.moveTo(toX, toY);
        context.lineTo(toX - headSize * Math.cos(angle + Math.PI / 8), toY - headSize * Math.sin(angle + Math.PI / 8));
        //pop
        context.stroke();
        context.restore();
    }
}
