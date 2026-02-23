import QtQuick

Canvas {
    property int headSize: 10

    required property double sourceX
    required property double sourceY
    required property double targetX
    required property double targetY
    required property var bends
    required property int flags

    readonly property bool hover: (flags & ENUMS.StateFlag.Hovered) !== 0
    readonly property bool highlight: (flags & ENUMS.StateFlag.InPath) !== 0
    readonly property bool hidden: (flags & ENUMS.StateFlag.Hidden) !== 0

    property color color: highlight ? "yellow" : hover ? '#4cb4e1' : '#ffffff'
    property real opacityFactor: hidden ? 0.1 : 1.0
    // Compute translated coordinates (centered)
    property real sX: sourceX
    property real sY: sourceY
    property real tX: targetX
    property real tY: targetY
    property int nodeRadius: 10
    // Compute bounding box (so the Canvas knows what to paint)
    // Compute bounding box including all bend points
    property real minX: {
        let val = Math.min(sX, tX);
        for (let p of bends)
            val = Math.min(val, p.x);
        return val - headSize - nodeRadius; // Add padding for arrowhead
    }
    property real minY: {
        let val = Math.min(sY, tY);
        for (let p of bends)
            val = Math.min(val, p.y);
        return val - headSize - nodeRadius;
    }
    property real maxX: {
        let val = Math.max(sX, tX);
        for (let p of bends)
            val = Math.max(val, p.x);
        return val + headSize + nodeRadius;
    }
    property real maxY: {
        let val = Math.max(sY, tY);
        for (let p of bends)
            val = Math.max(val, p.y);
        return val + headSize + nodeRadius;
    }
    x: minX
    y: minY
    width: Math.max(1, maxX - minX) // Ensure at least 1px width
    height: Math.max(1, maxY - minY)
    antialiasing: true
    onColorChanged: requestPaint()
    onFlagsChanged: requestPaint()
    onPaint: {
        var context = getContext('2d');
        context.save();
        context.clearRect(0, 0, width, height);

        context.globalAlpha = opacityFactor;
        context.strokeStyle = color;
        context.lineWidth = hover ? 2 : 1;
        context.beginPath();

        // 1. Start at Source (Ideally offset by nodeRadius here too)
        context.moveTo(sX - x, sY - y);

        // 2. Draw through intermediate bends
        for (var i = 0; i < bends.length; ++i) {
            context.lineTo(bends[i].x - x, bends[i].y - y);
        }

        // 3. Calculate the "Gap" for the target node
        var lastX = bends.length > 0 ? bends[bends.length - 1].x : sX;
        var lastY = bends.length > 0 ? bends[bends.length - 1].y : sY;

        var dx = tX - lastX;
        var dy = tY - lastY;
        var dist = Math.sqrt(dx * dx + dy * dy);

        // The logic: NewTarget = Target - (UnitVector * nodeRadius)
        // We stop the line 'nodeRadius' pixels before the actual center
        var stopX = tX - (dx / dist) * nodeRadius;
        var stopY = tY - (dy / dist) * nodeRadius;

        // 4. Draw the line to the shortened point
        context.lineTo(stopX - x, stopY - y);
        context.stroke();

        // 5. Draw the Arrowhead at the shortened point
        var angle = Math.atan2(dy, dx);
        var headX = stopX - x;
        var headY = stopY - y;

        context.beginPath();
        context.moveTo(headX, headY);
        context.lineTo(headX - headSize * Math.cos(angle - Math.PI / 8), headY - headSize * Math.sin(angle - Math.PI / 8));
        context.moveTo(headX, headY);
        context.lineTo(headX - headSize * Math.cos(angle + Math.PI / 8), headY - headSize * Math.sin(angle + Math.PI / 8));
        context.stroke();

        context.restore();
    }
}
