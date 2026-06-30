pragma ComponentBehavior: Bound
import QtQuick

Canvas {
    id: canvas

    property int headSize: 10

    required property point source
    required property point target
    required property list<point> bends

    required property int lineWidth
    required property bool dashed
    required property bool diamondTip
    required property bool hollowTip

    required property bool hovered
    required property bool highlighted
    required property bool hidden

    signal mouseOver
    signal mouseExit
    signal clicked

    readonly property double sourceX: source.x
    readonly property double sourceY: source.y
    readonly property double targetX: target.x
    readonly property double targetY: target.y

    property color color: highlighted ? "yellow" : hovered ? '#4cb4e1' : '#ffffff'
    property real opacityFactor: hidden ? 0.1 : 1.0

    // Compute translated coordinates (centered)
    property real sX: sourceX
    property real sY: sourceY
    property real tX: targetX
    property real tY: targetY
    property int nodeRadius: 10

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
    onHoveredChanged: requestPaint()
    onHighlightedChanged: requestPaint()

    // --- Interactive Mouse Testing Functions ---

    // 1. Shared structural path layout used for both painting and hit-testing
    function buildPath(context) {
        context.beginPath();

        // Start at Source
        context.moveTo(sX - x, sY - y);

        // Draw through intermediate bends
        for (var i = 0; i < bends.length; ++i) {
            context.lineTo(bends[i].x - x, bends[i].y - y);
        }

        // Calculate the "Gap" for the target node
        var lastX = bends.length > 0 ? bends[bends.length - 1].x : sX;
        var lastY = bends.length > 0 ? bends[bends.length - 1].y : sY;

        var dx = tX - lastX;
        var dy = tY - lastY;
        var dist = Math.sqrt(dx * dx + dy * dy);

        // Stop the line 'nodeRadius' pixels before the actual center
        var stopX = tX - (dx / dist) * nodeRadius;
        var stopY = tY - (dy / dist) * nodeRadius;

        context.lineTo(stopX - x, stopY - y);
    }
    function distanceToSegment(px, py, x1, y1, x2, y2) {
        let dx = x2 - x1;
        let dy = y2 - y1;

        // Segment length squared
        let l2 = dx * dx + dy * dy;
        if (l2 === 0)
            return Math.sqrt((px - x1) * (px - x1) + (py - y1) * (py - y1));

        // Projection factor (clamped between 0 and 1)
        let t = ((px - x1) * dx + (py - y1) * dy) / l2;
        t = Math.max(0, Math.min(1, t));

        // Find the closest point on the segment
        let closestX = x1 + t * dx;
        let closestY = y1 + t * dy;

        // Distance from mouse to closest point
        let distX = px - closestX;
        let distY = py - closestY;
        return Math.sqrt(distX * distX + distY * distY);
    }
    // 2. Math collision detection utilizing the canvas context math engine
    function isMouseOverLine(mouseX, mouseY) {
        // 1. Build an ordered array of all points on the path (translated to canvas local space)
        let points = [];
        points.push(Qt.point(sX - x, sY - y));

        for (let i = 0; i < bends.length; ++i) {
            points.push(Qt.point(bends[i].x - x, bends[i].y - y));
        }

        // Target calculation (stopping at nodeRadius)
        let lastX = bends.length > 0 ? bends[bends.length - 1].x : sX;
        let lastY = bends.length > 0 ? bends[bends.length - 1].y : sY;
        let dx = tX - lastX;
        let dy = tY - lastY;
        let dist = Math.sqrt(dx * dx + dy * dy);

        if (dist > 0) {
            let stopX = tX - (dx / dist) * nodeRadius;
            let stopY = tY - (dy / dist) * nodeRadius;
            points.push(Qt.point(stopX - x, stopY - y));
        } else {
            points.push(Qt.point(tX - x, tY - y));
        }

        // 2. Check the distance from the mouse to every segment in the path
        let threshold = Math.max(10, lineWidth * 2); // 10px invisible hit target padding

        for (let j = 0; j < points.length - 1; ++j) {
            let p1 = points[j];
            let p2 = points[j + 1];

            let d = distanceToSegment(mouseX, mouseY, p1.x, p1.y, p2.x, p2.y);
            if (d <= threshold) {
                return true; // Mouse is close enough to this segment!
            }
        }

        return false;
    }
    // 3. Signal coordinator hook for MouseArea
    function checkHoverState(mouseX, mouseY) {
        let currentlyOver = isMouseOverLine(mouseX, mouseY);
        if (currentlyOver && !hovered) {
            canvas.mouseOver();
        } else if (!currentlyOver && hovered) {
            canvas.mouseExit();
        }
    }

    // --- Canvas Painter ---

    onPaint: {
        var context = getContext('2d');
        context.save();
        context.clearRect(0, 0, width, height);

        context.globalAlpha = opacityFactor;
        context.strokeStyle = color;
        context.lineWidth = lineWidth;

        if (dashed) {
            context.setLineDash([5, 5]); // Dashed line for Implementation
        }

        // Generate and stroke the structural line path
        buildPath(context);
        context.stroke();

        // Draw the Arrowhead at the shortened point
        context.setLineDash([]);

        // Recalculate endpoint geometry for arrow tracking
        var lastX = bends.length > 0 ? bends[bends.length - 1].x : sX;
        var lastY = bends.length > 0 ? bends[bends.length - 1].y : sY;
        var dx = tX - lastX;
        var dy = tY - lastY;
        var dist = Math.sqrt(dx * dx + dy * dy);

        var stopX = tX - (dx / dist) * nodeRadius;
        var stopY = tY - (dy / dist) * nodeRadius;
        var angle = Math.atan2(dy, dx);
        var headX = stopX - x;
        var headY = stopY - y;
        let flareSize = headSize * 1.2;

        context.beginPath();
        if (diamondTip) {
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

            if (hollowTip) {
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

    // --- Interaction Layer Sub-item ---

    MouseArea {
        id: interactionArea
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true

        onPositionChanged: mouse => canvas.checkHoverState(mouse.x, mouse.y)
        onExited: canvas.checkHoverState(-1, -1)

        onClicked: mouse => {
            if (canvas.isMouseOverLine(mouse.x, mouse.y)) {
                canvas.clicked();
            } else {
                mouse.accepted = false; // Allow clicks to miss the line and pass through to underlying elements
            }
        }
    }
}
