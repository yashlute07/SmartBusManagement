
(function () {
    "use strict";

    const STORAGE_KEY = "tripXBuses";

    const routes = [
        // Pune Routes
        { code: "TX-PBH-001", source: "Pune", destination: "Bhusawal", fare: 450, operator: "TripX Demo Operator", time: "07:00" },
        { code: "TX-PJL-002", source: "Pune", destination: "Jalgaon", fare: 500, operator: "Khandesh Express", time: "08:00" },
        { code: "TX-PDH-003", source: "Pune", destination: "Dhule", fare: 480, operator: "TripX Demo Operator", time: "09:00" },
        { code: "TX-PMU-004", source: "Pune", destination: "Mumbai", fare: 350, operator: "TripX Demo Operator", time: "10:00" },
        { code: "TX-PCS-005", source: "Pune", destination: "Chhatrapati Sambhajinagar", fare: 420, operator: "TripX Demo Operator", time: "11:00" },
        { code: "TX-PSL-006", source: "Pune", destination: "Solapur", fare: 380, operator: "TripX Demo Operator", time: "12:00" },
        { code: "TX-PNS-007", source: "Pune", destination: "Nashik", fare: 400, operator: "TripX Demo Operator", time: "13:00" },
        { code: "TX-PW-008", source: "Pune", destination: "Wai", fare: 250, operator: "TripX Demo Operator", time: "08:00" },
        { code: "TX-PM-009", source: "Pune", destination: "Mahabaleshwar", fare: 350, operator: "TripX Demo Operator", time: "09:00" },
        { code: "TX-PS-010", source: "Pune", destination: "Satara", fare: 220, operator: "TripX Demo Operator", time: "10:00" },
        { code: "TX-PK-011", source: "Pune", destination: "Karad", fare: 280, operator: "TripX Demo Operator", time: "11:00" },
        { code: "TX-PKOL-012", source: "Pune", destination: "Kolhapur", fare: 400, operator: "TripX Demo Operator", time: "12:00" },
        { code: "TX-PSA-013", source: "Pune", destination: "Sangli", fare: 380, operator: "TripX Demo Operator", time: "13:00" },
        { code: "TX-PL-014", source: "Pune", destination: "Lonavala", fare: 150, operator: "TripX Demo Operator", time: "14:00" },
        { code: "TX-PT-015", source: "Pune", destination: "Thane", fare: 330, operator: "TripX Demo Operator", time: "15:00" },
        { code: "TX-PNM-016", source: "Pune", destination: "Navi Mumbai", fare: 320, operator: "TripX Demo Operator", time: "16:00" },

        // Wai Routes
        { code: "TX-WM-017", source: "Wai", destination: "Mahabaleshwar", fare: 120, operator: "TripX Demo Operator", time: "07:30" },
        { code: "TX-WS-018", source: "Wai", destination: "Satara", fare: 100, operator: "TripX Demo Operator", time: "08:30" },
        { code: "TX-WK-019", source: "Wai", destination: "Karad", fare: 180, operator: "TripX Demo Operator", time: "09:30" },
        { code: "TX-WMUM-020", source: "Wai", destination: "Mumbai", fare: 450, operator: "TripX Demo Operator", time: "10:30" },

        // Satara Routes
        { code: "TX-SM-021", source: "Satara", destination: "Mahabaleshwar", fare: 150, operator: "TripX Demo Operator", time: "08:00" },
        { code: "TX-SK-022", source: "Satara", destination: "Karad", fare: 100, operator: "TripX Demo Operator", time: "09:00" },
        { code: "TX-SKOL-023", source: "Satara", destination: "Kolhapur", fare: 250, operator: "TripX Demo Operator", time: "10:00" },
        { code: "TX-SS-024", source: "Satara", destination: "Sangli", fare: 220, operator: "TripX Demo Operator", time: "11:00" },

        // Other Routes
        { code: "TX-MUM-025", source: "Mumbai", destination: "Nashik", fare: 300, operator: "TripX Demo Operator", time: "07:00" },
        { code: "TX-MUK-026", source: "Mumbai", destination: "Kolhapur", fare: 550, operator: "TripX Demo Operator", time: "08:00" },
        { code: "TX-NK-027", source: "Nashik", destination: "Dhule", fare: 180, operator: "TripX Demo Operator", time: "09:00" },
        { code: "TX-JD-028", source: "Jalgaon", destination: "Dhule", fare: 150, operator: "TripX Demo Operator", time: "10:00" },
        { code: "TX-KS-029", source: "Kolhapur", destination: "Sangli", fare: 120, operator: "TripX Demo Operator", time: "11:00" },
        { code: "TX-TNM-030", source: "Thane", destination: "Navi Mumbai", fare: 100, operator: "TripX Demo Operator", time: "12:00" },
        { code: "TX-LM-031", source: "Lonavala", destination: "Mumbai", fare: 180, operator: "TripX Demo Operator", time: "13:00" },
        { code: "TX-SOL-032", source: "Solapur", destination: "Kolhapur", fare: 350, operator: "TripX Demo Operator", time: "14:00" },
        { code: "TX-CSN-033", source: "Chhatrapati Sambhajinagar", destination: "Nashik", fare: 250, operator: "TripX Demo Operator", time: "15:00" }
    ];

    const defaultBuses = [
        { id: "SB101", busNumber: "BUS-101", driver: "Rajesh Patil", routeCode: "TX-PNS-007", source: "Pune", destination: "Nashik", capacity: 40, status: "Active", operator: "TripX Demo Operator" },
        { id: "SB102", busNumber: "BUS-102", driver: "Suresh More", routeCode: "TX-PMU-004", source: "Pune", destination: "Mumbai", capacity: 40, status: "Active", operator: "TripX Demo Operator" },
        { id: "SB103", busNumber: "BUS-103", driver: "Amit Jadhav", routeCode: "", source: "Pune", destination: "Kolhapur", capacity: 40, status: "Maintenance", operator: "Legacy Bus" },
        { id: "SB104", busNumber: "BUS-104", driver: "Demo Driver 4", routeCode: "TX-PBH-001", source: "Pune", destination: "Bhusawal", capacity: 40, status: "Active", operator: "TripX Demo Operator" },
        { id: "SB105", busNumber: "BUS-105", driver: "Demo Driver 5", routeCode: "TX-PJL-002", source: "Pune", destination: "Jalgaon", capacity: 40, status: "Active", operator: "Khandesh Express" },
        { id: "SB106", busNumber: "BUS-106", driver: "Demo Driver 6", routeCode: "TX-PDH-003", source: "Pune", destination: "Dhule", capacity: 40, status: "Active", operator: "TripX Demo Operator" },
        { id: "SB107", busNumber: "BUS-107", driver: "Demo Driver 7", routeCode: "TX-PCS-005", source: "Pune", destination: "Chhatrapati Sambhajinagar", capacity: 40, status: "Active", operator: "TripX Demo Operator" },
        { id: "SB108", busNumber: "BUS-108", driver: "Demo Driver 8", routeCode: "TX-PSL-006", source: "Pune", destination: "Solapur", capacity: 40, status: "Active", operator: "TripX Demo Operator" }
    ];

    function readBuses() {
        try {
            const saved = localStorage.getItem(STORAGE_KEY);
            if (saved === null) {
                localStorage.setItem(STORAGE_KEY, JSON.stringify(defaultBuses));
                return defaultBuses.map(bus => ({ ...bus }));
            }

            const parsed = JSON.parse(saved);
            return Array.isArray(parsed) ? parsed : [];
        } catch (error) {
            console.error("TripX bus storage error:", error);
            return [];
        }
    }

    function saveBuses(buses) {
        localStorage.setItem(STORAGE_KEY, JSON.stringify(buses));
    }

    function getActiveBuses() {
        return readBuses().filter(bus => bus.status === "Active");
    }

    function getBus(id) {
        return readBuses().find(bus => bus.id === id);
    }

    function matchRoute(value) {
        const clean = text => String(text || "")
            .toLowerCase()
            .replace(/[^a-z]/g, "");

        const entered = clean(value);

        return routes.find(route => {
            const forward = clean(route.source + route.destination);
            const reverse = clean(route.destination + route.source);
            return entered === forward || entered === reverse;
        }) || null;
    }

    function addBus(details) {
        const busNumber = String(details.busNumber || "").trim().toUpperCase();
        const driver = String(details.driver || "").trim();
        const capacity = Number(details.capacity);
        const route = matchRoute(details.route);

        if (!busNumber || !driver || !route || !Number.isInteger(capacity) || capacity < 1 || capacity > 100) {
            throw new Error("Enter a valid bus number, driver, one of the 33 TripX routes, and capacity from 1 to 100.");
        }

        const buses = readBuses();

        if (buses.some(bus =>
            String(bus.busNumber).toUpperCase() === busNumber ||
            String(bus.id).toUpperCase() === busNumber
        )) {
            throw new Error("This Bus Number already exists.");
        }

        const bus = {
            id: busNumber,
            busNumber,
            driver,
            routeCode: route.code,
            source: route.source,
            destination: route.destination,
            capacity,
            status: "Active",
            operator: route.operator
        };

        buses.push(bus);
        saveBuses(buses);
        return bus;
    }

    function renderBusCards(container) {
        const buses = readBuses();
        container.innerHTML = "";

        buses.forEach(bus => {
            const card = document.createElement("div");
            card.className = "bus-card";

            const header = document.createElement("div");
            header.className = "bus-header";

            const heading = document.createElement("h3");
            heading.textContent = bus.busNumber;

            const status = document.createElement("span");
            status.className = "status " + (bus.status === "Active" ? "active" : "maintenance");
            status.textContent = bus.status.toUpperCase();

            header.append(heading, status);

            const info = document.createElement("div");
            info.className = "bus-info";

            const fields = [
                ["Driver:", bus.driver],
                ["Route:", bus.source + " → " + bus.destination],
                ["Route Code:", bus.routeCode || "Legacy"],
                ["Operator:", bus.operator || "TripX"],
                ["Capacity:", bus.capacity + " Passengers"]
            ];

            fields.forEach(([label, value]) => {
                const p = document.createElement("p");
                const strong = document.createElement("strong");
                strong.textContent = label + " ";
                p.append(strong, document.createTextNode(String(value)));
                info.appendChild(p);
            });

            const progress = document.createElement("div");
            progress.className = "progress";

            const occupancy = document.createElement("p");
            occupancy.style.cssText = "font-size:13px;margin-bottom:6px;";
            occupancy.textContent = "Availability is calculated from bookings.";

            progress.appendChild(occupancy);
            card.append(header, info, progress);
            container.appendChild(card);
        });
    }

    window.TripXData = {
        routes,
        readBuses,
        saveBuses,
        getActiveBuses,
        getBus,
        matchRoute,
        addBus,
        renderBusCards
    };
})();
