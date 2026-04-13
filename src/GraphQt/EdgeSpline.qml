import QtQuick

Canvas {
    property int headSize: 10

    required property edgePos position
    required property int flags
    required property int edgeType

    readonly property bool hover: (flags & ENUMS.StateFlag.Hovered) !== 0
    readonly property bool highlight: (flags & ENUMS.StateFlag.InPath) !== 0
    readonly property bool hidden: (flags & ENUMS.StateFlag.Hidden) !== 0
    readonly property double sourceX: position.source.x
    readonly property double sourceY: position.source.y
    readonly property double targetX: position.target.x
    readonly property double targetY: position.target.y
    readonly property var bends: position.bends

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
    function lineWidth() {
        let lw = 1;
        if (edgeType === ENUMS.EdgeType.Composes)
            lw = 3;
        if (hover)
            lw *= 2;
        return lw;
    }
    x: minX
    y: minY
    width: Math.max(1, maxX - minX) // Ensure at least 1px width
    height: Math.max(1, maxY - minY)
    antialiasing: true

    onColorChanged: requestPaint()
    onFlagsChanged: requestPaint()
    onPositionChanged: requestPaint()

    onPaint: {
        var context = getContext('2d');
        context.save();
        context.clearRect(0, 0, width, height);

        context.globalAlpha = opacityFactor;
        context.strokeStyle = color;
        context.lineWidth = lineWidth();

        if (edgeType === ENUMS.EdgeType.Implements) {
            context.setLineDash([5, 5]); // Dashed line for Implementation
        }

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
        context.setLineDash([]);
        var angle = Math.atan2(dy, dx);
        var headX = stopX - x;
        var headY = stopY - y;
        let flareSize = headSize * 1.2;

        context.beginPath();
        if (edgeType === ENUMS.EdgeType.Aggregates) {
            context.moveTo(headX, headY);
            context.lineTo(headX - flareSize * Math.cos(angle - 0.5), headY - flareSize * Math.sin(angle - 0.5));
            context.lineTo(headX - (flareSize * 2) * Math.cos(angle), headY - (flareSize * 2) * Math.sin(angle));
            context.lineTo(headX - flareSize * Math.cos(angle + 0.5), headY - flareSize * Math.sin(angle + 0.5));
            context.closePath();
            context.fillStyle = "#1a1a1a"; // Hollow background
            context.fill();
            context.stroke();
        } else {
            let sharpnessAngle = Math.PI / 6; // Wider corners (30 degrees)
            context.moveTo(headX, headY);

            // Point 1: Left back corner (flared wider)
            let x1 = headX - flareSize * Math.cos(angle - sharpnessAngle);
            let y1 = headY - flareSize * Math.sin(angle - sharpnessAngle);
            context.lineTo(x1, y1);

            // Point 2: Right back corner (flared wider)
            let x2 = headX - flareSize * Math.cos(angle + sharpnessAngle);
            let y2 = headY - flareSize * Math.sin(angle + sharpnessAngle);
            context.lineTo(x2, y2);

            // Connect back to the tip
            context.closePath();

            if (edgeType === ENUMS.EdgeType.Implements || edgeType == ENUMS.EdgeType.Injects) {
                // Hollow Triangle for Interfaces
                context.fillStyle = "#1a1a1a"; // Background color
                context.fill();
                context.stroke();
            } else {
                // Solid Arrowhead for Concrete/Composed
                context.fillStyle = color;
                context.fill();
            }
        }
        context.restore();
    }
}
