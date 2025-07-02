const sensorTemperatureDiv = document.getElementById("sensorTemperature");
const sensorHumidityDiv = document.getElementById("sensorHumidity");
const sensorCO2ppmDiv = document.getElementById("sensorCO2ppm");
const weatherTemperatureDiv = document.getElementById("weatherTemperature");
const weatherHumidityDiv = document.getElementById("weatherHumidity");
const weatherApparentTemperatureDiv = document.getElementById(
    "weatherApparentTemperature",
);
const settingsOpenButton = document.getElementById("settingsOpenButton");
const openButton = document.getElementById("openButton");
const autoEnableInput = document.getElementById("autoEnableInput");
const securityDialog = document.getElementById("securityDialog");

async function updateData() {
    const response = await fetch("/data");
    if (response.status !== 200) {
        console.log("Could not reach /data");
    }

    let data;
    try {
        data = await response.json();
    } catch (ex) {
        return;
    }

    setValues(data);
}

function setValues(data) {
    sensorTemperatureDiv.innerText = data.sensors.temperature.toFixed(1) + "°C";
    sensorHumidityDiv.innerText = data.sensors.humidity.toFixed(1) + "%";
    sensorCO2ppmDiv.innerText = data.sensors.co2ppm.toFixed(1) + "ppm";

    weatherTemperatureDiv.innerText =
        data.weather.temperature.toFixed(1) + "°C";
    weatherHumidityDiv.innerText = data.weather.humidity.toFixed(1) + "%";
    weatherApparentTemperatureDiv.innerText =
        data.weather.apparentTemperature.toFixed(1) + "°C";

    autoEnableInput.checked = data.autoEnabled;

    if (data.state === "alarm") {
        securityDialog.showModal();
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
settingsOpenButton.addEventListener("click", onButtonPress);
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
