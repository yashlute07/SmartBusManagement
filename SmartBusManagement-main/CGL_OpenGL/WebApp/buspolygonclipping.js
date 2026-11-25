(function () {
    const canvas = document.getElementById("busPolygonClippingCanvas");
    if (!canvas) return;

    const ctx = canvas.getContext("2d");

    const xmin = 150, ymin = 100;
    const xmax = 650, ymax = 400;

    const originalPolygon = [
        [80, 160],
        [380, 40],
        [720, 180],
        [600, 460],
        [250, 440]
    ];

    function clipBoundary(points, inside, intersect) {
        const output = [];
        if (points.length === 0) return output;

        let previous = points[points.length - 1];

        for (const current of points) {
            const prevInside = inside(previous);
            const currInside = inside(current);

            if (currInside) {
                if (!prevInside) output.push(intersect(previous, current));
                output.push(current);
            } else if (prevInside) {
                output.push(intersect(previous, current));
            }

            previous = current;
        }

        return output;
    }

    function sutherlandHodgman(polygon) {
        let result = polygon;

        // Left boundary
        result = clipBoundary(
            result,
            p => p[0] >= xmin,
            (a, b) => {
                const t = (xmin - a[0]) / (b[0] - a[0]);
                return [xmin, a[1] + t * (b[1] - a[1])];
            }
        );

        // Right boundary
        result = clipBoundary(
            result,
            p => p[0] <= xmax,
            (a, b) => {
                const t = (xmax - a[0]) / (b[0] - a[0]);
                return [xmax, a[1] + t * (b[1] - a[1])];
            }
        );

        // Top boundary
        result = clipBoundary(
            result,
            p => p[1] >= ymin,
            (a, b) => {
                const t = (ymin - a[1]) / (b[1] - a[1]);
                return [a[0] + t * (b[0] - a[0]), ymin];
            }
        );

        // Bottom boundary
        result = clipBoundary(
            result,
            p => p[1] <= ymax,
            (a, b) => {
                const t = (ymax - a[1]) / (b[1] - a[1]);
                return [a[0] + t * (b[0] - a[0]), ymax];
            }
        );

        return result;
    }

    function drawPolygon(points, color, fill = false, dashed = false) {
        if (points.length < 3) return;

        ctx.beginPath();
        ctx.moveTo(points[0][0], points[0][1]);

        for (let i = 1; i < points.length; i++) {
            ctx.lineTo(points[i][0], points[i][1]);
        }

        ctx.closePath();
        ctx.strokeStyle = color;
        ctx.lineWidth = 3;
        ctx.setLineDash(dashed ? [8, 5] : []);
        ctx.stroke();

        if (fill) {
            ctx.fillStyle = "rgba(22, 163, 74, 0.25)";
            ctx.fill();
        }

        ctx.setLineDash([]);
    }

    function draw() {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.fillStyle = "#ffffff";
        ctx.fillRect(0, 0, canvas.width, canvas.height);

        ctx.fillStyle = "#1e293b";
        ctx.font = "20px Arial";
        ctx.fillText("Sutherland-Hodgman Polygon Clipping", 190, 35);

        // Original polygon
        drawPolygon(originalPolygon, "#ef4444", false, true);

        // Clipping window
        ctx.strokeStyle = "#2563eb";
        ctx.lineWidth = 4;
        ctx.strokeRect(xmin, ymin, xmax - xmin, ymax - ymin);

        // Clipped polygon
        const clippedPolygon = sutherlandHodgman(originalPolygon);
        drawPolygon(clippedPolygon, "#16a34a", true);

        ctx.fillStyle = "#334155";
        ctx.font = "15px Arial";
        ctx.fillText("Red: Original Polygon | Green: Clipped Polygon | Blue: Window", 100, 485);
    }

    draw();
})();
