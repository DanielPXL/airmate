const dataUpdateElements = document.querySelectorAll(".dataUpdate");
const settingsOpenButton = document.getElementById("settingsOpenButton");
const openButton = document.getElementById("openButton");
const autoEnableInput = document.getElementById("autoEnableInput");
const securityDialog = document.getElementById("securityDialog");
const warningDialog = document.getElementById("warningDialog");
const dataDialog = document.getElementById("dataDialog");
const dataContainer = document.getElementById("dataContainer");
const closeTimeSelect = document.getElementById("closeTimeSelect");

let g_state = "closed";

async function updateData() {
    const response = await fetch("/data");
    if (response.status !== 200) {
        console.log("Could not reach /data");
        return;
    }

    let data;
    try {
        data = await response.json();
    } catch (ex) {
        return;
    }

    g_state = data.state;
    if (data.state === "alarm") {
        securityDialog.showModal();
    }

    updateWarningDialog(data);
    openButton.innerText = stateToButtonText(data.state);
    autoEnableInput.checked = data.autoEnabled;
    closeTimeSelect.value = `${data.closeTime}min`;

    formatData(data);
    setDataElements(data);
}

function formatData(data) {
    data.sensors.temperature = data.sensors.temperature.toFixed(1) + "°C";
    data.sensors.humidity = data.sensors.humidity.toFixed(1) + "%";
    data.sensors.co2ppm = data.sensors.co2ppm.toFixed(0) + "ppm";
    data.sensors.dewpoint = data.sensors.dewpoint.toFixed(1) + "°C";

    data.weather.temperature = data.weather.temperature.toFixed(1) + "°C";
    data.weather.humidity = data.weather.humidity.toFixed(1) + "%";
    data.weather.apparentTemperature =
        data.weather.apparentTemperature.toFixed(1) + "°C";
    data.weather.dewpoint = data.weather.dewpoint.toFixed(1) + "°C";
    data.weather.windSpeed = data.weather.windSpeed.toFixed(1) + "kmh";
    data.weather.precipitation = data.weather.precipitation.toFixed(1) + "ml";
}

function setDataElements(data) {
    for (const elem of dataUpdateElements) {
        const path = elem.getAttribute("data");
        const value = path
            .split(".")
            .reduce((acc, current) => acc[current], data);
        elem.innerText = value;
    }
}

let warningLastShown = 0;
function updateWarningDialog(data) {
    if (Date.now() - warningLastShown < 30 * 60 * 1000 || g_state != "closed") {
        return;
    }

    warningLastShown = Date.now();

    if (data.sensors.temperature > 25) {
        warningDialog.showModal();
    }

    if (data.sensors.dewPoint >= 5.0) {
        warningDialog.showModal8();
    }

    if (data.sensors.CO2 >= 1200) {
        warningDialog.showModal();
    }
}

async function onButtonPress() {
    const response = await fetch("/buttonPushed", { method: "POST" });
    if (response.status !== 200) {
        console.error("Could not reach /buttonPushed");
    }

    let data;
    try {
        data = await response.json();
    } catch (ex) {
        return;
    }

    setValues(data);
}

function stateToButtonText(state) {
    switch (state) {
        case "closed":
            return "Öffnen";
        case "closing":
            return "Stop";
        case "open":
            return "Schließen";
        case "opening":
            return "Stop";
        case "locking":
            return "Schließt ab...";
        case "unlocking":
            return "Schließt auf...";
        case "pausing":
            return "Pausiert...";
        case "alarm":
            return "Schließen";
    }
}

async function onAutoEnableChange() {
    const url = autoEnableInput.checked ? "/setAuto" : "/resetAuto";
    const response = await fetch(url, { method: "POST" });
    if (response.status !== 200) {
        console.error("Could not reach " + url);
    }

    let data;
    try {
        data = await response.json();
    } catch (ex) {
        return;
    }

    setValues(data);
}

updateData();
setInterval(updateData, 1000);
openButton.addEventListener("click", onButtonPress);
autoEnableInput.addEventListener("click", onAutoEnableChange);

const settingsDialog = document.getElementById("siteSettings");
const darkModeInput = document.getElementById("darkModeInput");

settingsOpenButton.addEventListener("click", () => {
    settingsDialog.showModal();
});

darkModeInput.addEventListener("change", () => {
    updateDarkMode(darkModeInput.checked);
});

function updateDarkMode() {
    if (darkModeInput.checked) {
        document.body.classList.add("dark");
        localStorage.setItem("darkMode", "true");
    } else {
        document.body.classList.remove("dark");
        localStorage.setItem("darkMode", false);
    }
}

const darkStorage = localStorage.getItem("darkMode");
darkModeInput.checked = darkStorage === null ? false : darkStorage === "true";
updateDarkMode();

dataContainer.addEventListener("click", () => dataDialog.showModal());

closeTimeSelect.addEventListener("change", async () => {
    // Remove "min"
    const value = closeTimeSelect.value.slice(0, -3);
    await fetch(`/setCloseTime${value}`, { method: "POST" });
});
