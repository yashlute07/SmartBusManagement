
        const STORAGE_KEY = "smartBusTravelHistory";

        const dashboardContent = document.getElementById("dashboardContent");
        const historySection = document.getElementById("historySection");
const dsSection = document.getElementById("dsSection");
const coa4Section = document.getElementById("coa4Section");
const coa4Link = document.getElementById("coa4Link");
const dsLink = document.getElementById("dsLink");
        const pageTitle = document.getElementById("pageTitle");
        const dashboardLink = document.getElementById("dashboardLink");
        const historyLink = document.getElementById("historyLink");
        const historyForm = document.getElementById("historyForm");
        const historyMessage = document.getElementById("historyMessage");
        const historyTableBody = document.getElementById("historyTableBody");
        const emptyHistory = document.getElementById("emptyHistory");
        const journeyCount = document.getElementById("journeyCount");
        const travelDateTime = document.getElementById("travelDateTime");

        // Set the current date and time as the default.
        function setCurrentDateTime() {
            const now = new Date();
            now.setMinutes(now.getMinutes() - now.getTimezoneOffset());
            travelDateTime.value = now.toISOString().slice(0, 16);
        }

        // Read saved records safely.
        function getHistory() {
            try {
                const records = JSON.parse(
                    localStorage.getItem(STORAGE_KEY) || "[]"
                );

                return Array.isArray(records) ? records : [];
            } catch (error) {
                return [];
            }
        }

        // Save records to browser storage.
        function saveHistory(records) {
            localStorage.setItem(STORAGE_KEY, JSON.stringify(records));
        }

        // Switch between Dashboard and History.
        function showPage(page) {
            const showHistory = page === "history";
            const showDS = page === "ds";
            const showCOA4 = page === "coa4";

            dashboardContent.classList.toggle("hidden", showHistory || showDS || showCOA4);
            historySection.classList.toggle("visible", showHistory);
            dsSection.style.display = showDS ? "block" : "none";
            coa4Section.style.display = showCOA4 ? "block" : "none";

            dashboardLink.classList.toggle("active", page === "dashboard");
            historyLink.classList.toggle("active", showHistory);
            if (dsLink) dsLink.classList.toggle("active", showDS);
            coa4Link.classList.toggle("active", showCOA4);

            pageTitle.textContent = showHistory ? "Travel History" :
                                    showDS ? "Data Structures" : showCOA4 ? "COA CO4 - Control Unit & Pipelining" : "Dashboard";

            if (showHistory) {
                renderHistory();
            }
        }

        dashboardLink.addEventListener("click", function(event) {
            event.preventDefault();
            history.replaceState(null, "", "#dashboard");
            showPage("dashboard");
        });
        coa4Link.addEventListener("click", function(event) {
            event.preventDefault();
            history.replaceState(null, "", "#coa4");
            showPage("coa4");
        });
        historyLink.addEventListener("click", function(event) {
            event.preventDefault();
            history.replaceState(null, "", "#history");
            showPage("history");
        });

        document.getElementById("quickHistory").addEventListener("click", function(event) {
            event.preventDefault();
            history.replaceState(null, "", "#history");
            showPage("history");
        });

        // Render the saved journeys in the table.
        function renderHistory() {
            const records = getHistory().sort(
                (a, b) => new Date(b.travelDateTime) - new Date(a.travelDateTime)
            );

            historyTableBody.replaceChildren();

            journeyCount.textContent =
                records.length + (records.length === 1 ? " journey" : " journeys");

            emptyHistory.style.display = records.length ? "none" : "block";

            records.forEach(function(record) {
                const row = document.createElement("tr");

                function addCell(value) {
                    const cell = document.createElement("td");
                    cell.textContent = value;
                    row.appendChild(cell);
                }

                const date = new Date(record.travelDateTime);

                addCell(record.passenger);
                addCell(record.busNumber);
                addCell(record.source + " → " + record.destination);

                addCell(Number.isNaN(date.getTime())
                    ? "Invalid date"
                    : date.toLocaleDateString("en-IN"));

                addCell(Number.isNaN(date.getTime())
                    ? "Invalid time"
                    : date.toLocaleTimeString("en-IN", {
                        hour: "2-digit",
                        minute: "2-digit"
                    }));

                addCell("₹" + Number(record.fare).toFixed(2));

                const actionCell = document.createElement("td");
                const deleteButton = document.createElement("button");

                deleteButton.type = "button";
                deleteButton.textContent = "Delete";
                deleteButton.className = "delete-btn";
                deleteButton.setAttribute("aria-label", "Delete journey");

                deleteButton.addEventListener("click", function() {
                    const updatedRecords = getHistory().filter(
                        item => item.id !== record.id
                    );

                    saveHistory(updatedRecords);
                    renderHistory();
                    historyMessage.textContent = "Journey deleted.";
                });

                actionCell.appendChild(deleteButton);
                row.appendChild(actionCell);
                historyTableBody.appendChild(row);
            });
        }

        // Record a new journey.
        historyForm.addEventListener("submit", function(event) {
            event.preventDefault();

            const passenger = document.getElementById("passenger").value.trim();
            const busNumber = document.getElementById("busNumber").value.trim();
            const source = document.getElementById("source").value.trim();
            const destination = document.getElementById("destination").value.trim();
            const fareValue = document.getElementById("fare").value;
            const dateValue = travelDateTime.value;
            const fare = Number(fareValue);

            if (!passenger || !busNumber || !source || !destination ||
                !dateValue || fareValue === "" || !Number.isFinite(fare) ||
                fare < 0 || !Number.isFinite(new Date(dateValue).getTime())) {
                historyMessage.textContent = "Please enter valid journey details.";
                return;
            }

            const records = getHistory();

            records.push({
                id: (crypto.randomUUID
                    ? crypto.randomUUID()
                    : Date.now().toString() + Math.random().toString(16).slice(2)),
                passenger: passenger,
                busNumber: busNumber,
                source: source,
                destination: destination,
                fare: fare,
                travelDateTime: new Date(dateValue).toISOString()
            });

            try {
                saveHistory(records);
                historyForm.reset();
                setCurrentDateTime();
                renderHistory();
                historyMessage.textContent = "Journey saved successfully!";
            } catch (error) {
                historyMessage.textContent =
                    "Could not save the journey. Browser storage may be full or disabled.";
            }
        });

        // Clear all journey records after confirmation.
        document.getElementById("clearHistory").addEventListener("click", function() {
            if (confirm("Are you sure you want to delete all travel history?")) {
                try {
                    localStorage.removeItem(STORAGE_KEY);
                    renderHistory();
                    historyMessage.textContent = "All travel history cleared.";
                } catch (error) {
                    historyMessage.textContent = "Could not clear travel history.";
                }
            }
        });

        // Initialize the page.
        setCurrentDateTime();
        showPage(location.hash === "#history" ? "history" : location.hash === "#datastructures" ? "ds" : location.hash === "#coa4" ? "coa4" : "dashboard");
    




