import * as THREE from "https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js";

const container = document.getElementById("busAnimation3D");

if (!container) {
    console.error("Bus animation container not found.");
} else {

    const scene = new THREE.Scene();
    scene.background = new THREE.Color(0x9bd3f5);
    scene.fog = new THREE.Fog(0x9bd3f5, 22, 65);

    const camera = new THREE.PerspectiveCamera(
        50,
        container.clientWidth / 480,
        0.1,
        100
    );

    camera.position.set(9, 7, 11);
    camera.lookAt(0, 1, 0);

    const renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
    renderer.setSize(container.clientWidth, 480);
    renderer.shadowMap.enabled = true;
    renderer.shadowMap.type = THREE.PCFSoftShadowMap;
    renderer.outputColorSpace = THREE.SRGBColorSpace;

    container.innerHTML = "";
    container.appendChild(renderer.domElement);

    // LIGHTING
    scene.add(new THREE.HemisphereLight(0xffffff, 0x667788, 2));

    const sunlight = new THREE.DirectionalLight(0xffffff, 2.5);
    sunlight.position.set(-8, 14, 8);
    sunlight.castShadow = true;
    scene.add(sunlight);

    // MATERIALS
    const blue = new THREE.MeshStandardMaterial({
        color: 0x0878c9,
        roughness: 0.4,
        metalness: 0.2
    });

    const darkBlue = new THREE.MeshStandardMaterial({ color: 0x06436e });
    const glass = new THREE.MeshStandardMaterial({
        color: 0x9ce5ff,
        metalness: 0.3,
        roughness: 0.15
    });

    const black = new THREE.MeshStandardMaterial({ color: 0x171717 });
    const rubber = new THREE.MeshStandardMaterial({ color: 0x222222 });
    const silver = new THREE.MeshStandardMaterial({
        color: 0xbfc8d0,
        metalness: 0.8,
        roughness: 0.25
    });

    // ROAD
    const road = new THREE.Mesh(
        new THREE.BoxGeometry(12, 0.15, 100),
        new THREE.MeshStandardMaterial({ color: 0x30343b })
    );
    road.position.y = -0.12;
    road.receiveShadow = true;
    scene.add(road);

    // ROAD SIDE GRASS
    const grass = new THREE.Mesh(
        new THREE.BoxGeometry(50, 0.12, 100),
        new THREE.MeshStandardMaterial({ color: 0x4c9b48 })
    );
    grass.position.y = -0.22;
    grass.receiveShadow = true;
    scene.add(grass);

    // ROAD LANE MARKINGS
    const laneMarks = [];

    for (let i = 0; i < 18; i++) {
        const mark = new THREE.Mesh(
            new THREE.BoxGeometry(0.13, 0.025, 1.7),
            new THREE.MeshStandardMaterial({ color: 0xffffff })
        );

        mark.position.set(0, -0.025, -42 + i * 5);
        scene.add(mark);
        laneMarks.push(mark);
    }

    // BUS GROUP
    const bus = new THREE.Group();
    scene.add(bus);

    function addBox(parent, width, height, depth, material, x, y, z) {
        const mesh = new THREE.Mesh(
            new THREE.BoxGeometry(width, height, depth),
            material
        );

        mesh.position.set(x, y, z);
        mesh.castShadow = true;
        mesh.receiveShadow = true;
        parent.add(mesh);
        return mesh;
    }

    // BUS LOWER BODY
    addBox(bus, 2.5, 1.0, 5.0, blue, 0, 1.05, 0);

    // BUS UPPER BODY
    addBox(bus, 2.4, 1.35, 4.5, blue, 0, 2.15, 0.05);

    // FRONT WINDSCREEN
    addBox(bus, 2.05, 0.9, 0.06, glass, 0, 2.35, -2.23);

    // FRONT LOWER PANEL
    addBox(bus, 2.2, 0.28, 0.08, darkBlue, 0, 1.55, -2.53);

    // FRONT GRILL
    addBox(bus, 0.9, 0.22, 0.04, silver, 0, 1.35, -2.58);

    // SIDE WINDOWS
    for (let i = 0; i < 5; i++) {
        const z = -1.65 + i * 0.78;

        addBox(bus, 0.035, 0.78, 0.65, glass, -1.215, 2.3, z);
        addBox(bus, 0.035, 0.78, 0.65, glass, 1.215, 2.3, z);
    }

    // ROOF
    addBox(bus, 2.3, 0.12, 4.4, darkBlue, 0, 2.9, 0.05);

    // FRONT HEADLIGHTS
    const headlightMaterial = new THREE.MeshStandardMaterial({
        color: 0xffffcc,
        emissive: 0xffee88,
        emissiveIntensity: 1.5
    });

    addBox(bus, 0.38, 0.22, 0.08, headlightMaterial, -0.85, 1.35, -2.58);
    addBox(bus, 0.38, 0.22, 0.08, headlightMaterial, 0.85, 1.35, -2.58);

    // WHEELS
    const wheels = [];

    function createWheel(x, z) {
        const wheel = new THREE.Group();
        wheel.position.set(x, 0.55, z);

        const tire = new THREE.Mesh(
            new THREE.CylinderGeometry(0.48, 0.48, 0.24, 32),
            rubber
        );

        tire.rotation.z = Math.PI / 2;
        tire.castShadow = true;
        wheel.add(tire);

        const hub = new THREE.Mesh(
            new THREE.CylinderGeometry(0.22, 0.22, 0.255, 24),
            silver
        );

        hub.rotation.z = Math.PI / 2;
        wheel.add(hub);

        bus.add(wheel);
        wheels.push(wheel);
    }

    createWheel(-1.25, -1.55);
    createWheel(1.25, -1.55);
    createWheel(-1.25, 1.55);
    createWheel(1.25, 1.55);

    // ROAD SIDE TREES
    const trees = [];

    const trunkMaterial = new THREE.MeshStandardMaterial({ color: 0x704326 });
    const leafMaterial = new THREE.MeshStandardMaterial({ color: 0x16833b });

    for (let i = 0; i < 12; i++) {
        for (const side of [-1, 1]) {
            const tree = new THREE.Group();

            const trunk = new THREE.Mesh(
                new THREE.CylinderGeometry(0.12, 0.18, 1.4, 8),
                trunkMaterial
            );
            trunk.position.y = 0.7;
            tree.add(trunk);

            const leaves = new THREE.Mesh(
                new THREE.ConeGeometry(0.8, 2.1, 8),
                leafMaterial
            );
            leaves.position.y = 2.1;
            tree.add(leaves);

            tree.position.set(side * 8, -0.1, -35 + i * 7);
            scene.add(tree);
            trees.push(tree);
        }
    }

    // ANIMATION CONTROLS
    let isPaused = false;
    let speed = 1;
    let animationTime = 0;

    const pauseButton = document.getElementById("busAnimPause");
    const speedControl = document.getElementById("busAnimSpeed");
    const status = document.getElementById("busAnimStatus");

    if (pauseButton) {
        pauseButton.addEventListener("click", () => {
            isPaused = !isPaused;
            pauseButton.textContent = isPaused ? "Resume" : "Pause";
            if (status) status.textContent = isPaused ? "Animation Paused" : "Bus is Moving";
        });
    }

    if (speedControl) {
        speedControl.addEventListener("input", () => {
            speed = Number(speedControl.value);
            if (status) status.textContent = "Bus Speed: " + speed.toFixed(1) + "x";
        });
    }

    // MAIN ANIMATION LOOP
    const clock = new THREE.Clock();

    function animate() {
        requestAnimationFrame(animate);

        const delta = Math.min(clock.getDelta(), 0.05);

        if (!isPaused) {
            animationTime += delta * speed;

            // WHEEL ROTATION
            wheels.forEach(wheel => {
                wheel.rotation.x += delta * speed * 5;
            });

            // BUS MOVEMENT EFFECT
            bus.position.y = Math.sin(animationTime * 5) * 0.035;
            bus.rotation.y = Math.sin(animationTime * 1.5) * 0.025;

            // MOVING ROAD MARKINGS
            laneMarks.forEach(mark => {
                mark.position.z += delta * speed * 12;

                if (mark.position.z > 48) {
                    mark.position.z = -42;
                }
            });

            // MOVING TREES
            trees.forEach(tree => {
                tree.position.z += delta * speed * 12;

                if (tree.position.z > 45) {
                    tree.position.z = -38;
                }
            });
        }

        renderer.render(scene, camera);
    }

    animate();

    // RESPONSIVE CANVAS
    function resizeBusAnimation() {
        const width = container.clientWidth;

        if (width > 0) {
            camera.aspect = width / 480;
            camera.updateProjectionMatrix();
            renderer.setSize(width, 480);
        }
    }

    window.addEventListener("resize", resizeBusAnimation);

    if (typeof ResizeObserver !== "undefined") {
        new ResizeObserver(resizeBusAnimation).observe(container);
    }
}
