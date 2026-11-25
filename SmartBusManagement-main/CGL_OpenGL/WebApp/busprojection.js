const canvas = document.getElementById("busProjectionCanvas");

if (canvas) {
    const ctx = canvas.getContext("2d");
    const W = canvas.width;
    const H = canvas.height;

    function point(x, y) {
        return {
            x: (x + 1) * W / 2,
            y: (1 - y) * H / 2
        };
    }

    function quad(vertices, color) {
        ctx.fillStyle = color;
        ctx.beginPath();
        vertices.forEach((v, i) => {
            const p = point(v[0], v[1]);
            if (i === 0) ctx.moveTo(p.x, p.y);
            else ctx.lineTo(p.x, p.y);
        });
        ctx.closePath();
        ctx.fill();
    }

    function wheel(cx, cy, radius) {
        const p = point(cx, cy);
        ctx.beginPath();
        ctx.arc(p.x, p.y, radius * W / 2, 0, Math.PI * 2);
        ctx.fillStyle = "#1a1a1a";
        ctx.fill();
    }

    function drawBus() {
        ctx.fillStyle = "#ffffff";
        ctx.fillRect(0, 0, W, H);

        // Bus body
        quad([
            [-0.7, -0.3],
            [0.7, -0.3],
            [0.7, 0.3],
            [-0.7, 0.3]
        ], "#007fff");

        // Windows
        const windows = [
            [-0.55, -0.25],
            [-0.15, 0.15],
            [0.25, 0.55]
        ];

        windows.forEach(([left, right]) => {
            quad([
                [left, 0],
                [right, 0],
                [right, 0.2],
                [left, 0.2]
            ], "#cce6ff");
        });

        // Wheels
        wheel(-0.45, -0.35, 0.12);
        wheel(0.45, -0.35, 0.12);
    }

    drawBus();
}
