"use strict";

(function () {
    const SESSION_KEY = "smartBusSession";
    const page = location.pathname.split("/").pop().toLowerCase();

    if (
        page === "login.html" ||
        page === "admin-login.html" ||
        page === "passenger-login.html" ||
        page === ""
    ) return;

    let session = null;

    try {
        session = JSON.parse(localStorage.getItem(SESSION_KEY) || "null");
    } catch (_) {}

    if (!session || !session.email || !session.role) {
        location.replace("login.html");
        return;
    }

    if (page === "passengers.html" && session.role !== "admin") {
        alert("Admin access only.");
        location.replace("index.html");
        return;
    }

    document.addEventListener("DOMContentLoaded", function () {

        if (session.role === "passenger") {
            if (page === "buses.html") {
                const addBusButton = document.querySelector(".add-btn");
                const busModal = document.getElementById("busModal");

                if (addBusButton) addBusButton.style.display = "none";
                if (busModal) busModal.style.display = "none";
            }

            if (page === "index.html") {
                document.querySelectorAll('a[href="passengers.html"]').forEach(function (link) {
                    link.style.display = "none";
                });
            }
        }
        const passengerInput = document.getElementById("passenger");
        const phoneInput = document.getElementById("phone");

        if (passengerInput && session.role === "passenger") {
            passengerInput.value = session.name || "";
        }

        if (phoneInput && session.role === "passenger") {
            phoneInput.value = session.phone || "";
        }

        const logout = document.createElement("button");
        logout.type = "button";
        logout.textContent = "Logout (" + (session.name || session.role) + ")";
        logout.style.cssText =
            "position:fixed;right:18px;top:18px;z-index:99999;" +
            "padding:10px 16px;background:#b42318;color:white;" +
            "border:0;border-radius:8px;cursor:pointer;font-weight:bold;";

        logout.addEventListener("click", function () {
            localStorage.removeItem(SESSION_KEY);
            location.replace("login.html");
        });

        document.body.appendChild(logout);
    });
})();
