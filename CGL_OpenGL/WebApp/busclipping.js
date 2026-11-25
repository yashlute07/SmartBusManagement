(function () {
const canvas = document.getElementById("busClippingCanvas");

if (canvas) {
    const ctx = canvas.getContext("2d");

    const xmin = 150, ymin = 100;
    const xmax = 650, ymax = 400;

    const LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;

    function computeCode(x, y) {
        let code = 0;
        if (x < xmin) code |= LEFT;
        else if (x > xmax) code |= RIGHT;
        if (y < ymin) code |= TOP;
        else if (y > ymax) code |= BOTTOM;
        return code;
    }

    function clipLine(x1, y1, x2, y2) {
        let code1 = computeCode(x1, y1);
        let code2 = computeCode(x2, y2);

        while (true) {
            if (!(code1 | code2)) return [x1, y1, x2, y2];
            if (code1 & code2) return null;

            const codeOut = code1 ? code1 : code2;
            let x, y;

            if (codeOut & TOP) {
                if (y2 === y1) return null;
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            } else if (codeOut & BOTTOM) {
                if (y2 === y1) return null;
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            } else if (codeOut & RIGHT) {
                if (x2 === x1) return null;
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            } else {
                if (x2 === x1) return null;
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (codeOut === code1) {
                x1 = x; y1 = y;
                code1 = computeCode(x1, y1);
            } else {
                x2 = x; y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    function drawLine(x1, y1, x2, y2, color, width = 3) {
        ctx.beginPath();
        ctx.moveTo(x1, y1);
        ctx.lineTo(x2, y2);
        ctx.strokeStyle = color;
        ctx.lineWidth = width;
        ctx.stroke();
    }

    function drawClipping() {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.fillStyle = "#ffffff";
        ctx.fillRect(0, 0, canvas.width, canvas.height);

        const lines = [
            [50, 50, 750, 450],
            [50, 250, 750, 250],
            [50, 450, 750, 50]
        ];

        // Original lines
        lines.forEach(line => drawLine(...line, "#ef4444", 2));

        // Clipping window
        ctx.strokeStyle = "#2563eb";
        ctx.lineWidth = 4;
        ctx.strokeRect(xmin, ymin, xmax - xmin, ymax - ymin);

        // Clipped lines
        lines.forEach(line => {
            const result = clipLine(...line);
            if (result) drawLine(...result, "#16a34a", 4);
        });

        ctx.fillStyle = "#1e293b";
        ctx.font = "16px Arial";
        ctx.fillText("Cohen-Sutherland Line Clipping", 220, 35);
    }

    drawClipping();
}

})();
