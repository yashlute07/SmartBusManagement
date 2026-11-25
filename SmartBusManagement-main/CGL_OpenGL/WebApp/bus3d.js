import * as THREE from "https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js";

const container = document.getElementById("bus3d");
const status = document.getElementById("seatStatus");

if (!container) throw new Error("3D bus container not found.");

const scene = new THREE.Scene();
scene.background = new THREE.Color(0x101827);

const camera = new THREE.PerspectiveCamera(
    48, container.clientWidth / container.clientHeight, 0.1, 100
);
camera.position.set(0, 19, 13);
camera.up.set(0, 0, -1);
camera.lookAt(0, 0, 0);

const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(container.clientWidth, container.clientHeight);
renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
renderer.shadowMap.enabled = true;
container.appendChild(renderer.domElement);

// Lighting
scene.add(new THREE.AmbientLight(0xffffff, 1.1));
const light = new THREE.DirectionalLight(0xffffff, 2);
light.position.set(-5, 12, -5);
light.castShadow = true;
scene.add(light);

// Checker-style floor texture, matching the C++ project
const textureCanvas = document.createElement("canvas");
textureCanvas.width = 256;
textureCanvas.height = 256;
const ctx = textureCanvas.getContext("2d");

for (let y = 0; y < 8; y++) {
    for (let x = 0; x < 8; x++) {
        ctx.fillStyle = (x + y) % 2 === 0 ? "#9ba5aa" : "#5f6e78";
        ctx.fillRect(x * 32, y * 32, 32, 32);
    }
}

const floorTexture = new THREE.CanvasTexture(textureCanvas);
floorTexture.wrapS = THREE.RepeatWrapping;
floorTexture.wrapT = THREE.RepeatWrapping;
floorTexture.repeat.set(2, 5);

const floor = new THREE.Mesh(
    new THREE.PlaneGeometry(6, 18),
    new THREE.MeshStandardMaterial({ map: floorTexture, roughness: 0.9 })
);
floor.rotation.x = -Math.PI / 2;
floor.position.y = 0;
floor.receiveShadow = true;
scene.add(floor);

function addBox(x, y, z, sx, sy, sz, color) {
    const mesh = new THREE.Mesh(
        new THREE.BoxGeometry(sx, sy, sz),
        new THREE.MeshStandardMaterial({ color, roughness: 0.65 })
    );
    mesh.position.set(x, y, z);
    mesh.castShadow = true;
    mesh.receiveShadow = true;
    scene.add(mesh);
    return mesh;
}

// Bus side walls and end walls
addBox(-3.1, 0.7, 0, 0.25, 1.4, 18, 0x265980);
addBox(3.1, 0.7, 0, 0.25, 1.4, 18, 0x265980);
addBox(0, 0.7, -8.8, 6.2, 1.4, 0.3, 0x326699);
addBox(0, 0.7, 8.8, 6.2, 1.4, 0.3, 0x326699);

// Central aisle and driver area
addBox(0, 0.05, 0, 0.65, 0.08, 17, 0xd9c9a0);
addBox(0, 0.4, -8.1, 1.5, 0.7, 0.9, 0x444b5a);

// Create 20 seats: synchronized with the booking form
const seats = [];
let selectedSeat = 0;

function getActiveBookedSeats() {
    const bus = document.getElementById("bus")?.value;
    const date = document.getElementById("travelDate")?.value;
    const departure = document.getElementById("departureTime")?.value;
    const source = document.getElementById("source")?.value;
    const destination = document.getElementById("destination")?.value;

    if (!bus || !date || !departure || !source || !destination || source === destination) {
        return [];
    }

    try {
        const records = JSON.parse(localStorage.getItem("smartBusBookings") || "[]");
        if (!Array.isArray(records)) return [];

        return records
            .filter(record =>
                record.bus === bus &&
                record.date === date &&
                record.source === source &&
                record.destination === destination &&
                (!record.departureTime || record.departureTime === departure) &&
                record.status !== "Cancelled"
            )
            .map(record => record.seat);
    } catch (error) {
        console.error("Unable to read booking seats:", error);
        return [];
    }
}

function seatColor(index, bookedSeats) {
    const seatName = "S" + String(index + 1).padStart(2, "0");
    if (bookedSeats.includes(seatName)) return 0xdc2626;
    if (index === selectedSeat) return 0x2563eb;
    return 0x16a34a;
}

function updateSeats() {
    const bookedSeats = getActiveBookedSeats();

    seats.forEach((seat, index) => {
        seat.material.color.setHex(seatColor(index, bookedSeats));
    });

    status.textContent =
        `Selected Seat: S${String(selectedSeat + 1).padStart(2, "0")} | Booked: ${bookedSeats.length} | Vacant: ${20 - bookedSeats.length}`;
}

for (let i = 0; i < 20; i++) {
    const x = i % 2 === 0 ? -1.25 : 1.25;
    const z = -7.5 + Math.floor(i / 2) * 1.65;

    const seat = addBox(x, 0.2, z, 1, 0.35, 1, 0x16a34a);
    seat.userData.index = i;
    seats.push(seat);

    addBox(x, 0.65, z + 0.35, 1, 0.8, 0.2, 0x64748b);
    addBox(x - 0.48, 0.42, z, 0.08, 0.3, 0.8, 0x475569);
    addBox(x + 0.48, 0.42, z, 0.08, 0.3, 0.8, 0x475569);
}

function selectedSeatName() {
    return "S" + String(selectedSeat + 1).padStart(2, "0");
}

function selectCurrentSeat() {
    const bookedSeats = getActiveBookedSeats();
    if (bookedSeats.includes(selectedSeatName())) {
        updateSeats();
        return;
    }
    window.tripxSelectSeat?.(selectedSeatName());
    updateSeats();
}

function vacateCurrentSeat() {
    window.tripxDeselectSeat?.(selectedSeatName());
    updateSeats();
}

function resetSeats() {
    window.tripxClearSelectedSeats?.();
    selectedSeat = 0;
    updateSeats();
}

const raycaster = new THREE.Raycaster();
const pointer = new THREE.Vector2();

renderer.domElement.addEventListener("click", event => {
    const rect = renderer.domElement.getBoundingClientRect();
    pointer.x = ((event.clientX - rect.left) / rect.width) * 2 - 1;
    pointer.y = -((event.clientY - rect.top) / rect.height) * 2 + 1;

    raycaster.setFromCamera(pointer, camera);
    const hits = raycaster.intersectObjects(seats);

    if (hits.length) {
        selectedSeat = hits[0].object.userData.index;
        selectCurrentSeat();
        updateSeats();
    }
});

document.getElementById("book3dSeat").addEventListener("click", selectCurrentSeat);
document.getElementById("vacate3dSeat").addEventListener("click", vacateCurrentSeat);
document.getElementById("reset3dSeats").addEventListener("click", resetSeats);

window.addEventListener("keydown", event => {
    if (event.key === "ArrowRight" || event.key === "ArrowDown") {
        selectedSeat = (selectedSeat + 1) % 20;
    } else if (event.key === "ArrowLeft" || event.key === "ArrowUp") {
        selectedSeat = (selectedSeat + 19) % 20;
    } else if (event.key.toLowerCase() === "b") {
        selectCurrentSeat();
        return;
    } else if (event.key.toLowerCase() === "v") {
        vacateCurrentSeat();
        return;
    } else if (event.key.toLowerCase() === "r") {
        resetSeats();
        return;
    } else {
        return;
    }
    updateSeats();
});

// Refresh the 3D bus whenever the booking grid or saved records change.
window.addEventListener("smartBusBookingStateChanged", updateSeats);
window.addEventListener("storage", event => {
    if (event.key === "smartBusBookings") updateSeats();
});
function animate() {
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}

new ResizeObserver(() => {
    const w = container.clientWidth;
    const h = container.clientHeight;
    if (!w || !h) return;
    camera.aspect = w / h;
    camera.updateProjectionMatrix();
    renderer.setSize(w, h);
}).observe(container);

updateSeats();
animate();

