const sensorTemperatureDiv = document.getElementById("sensorTemperature");
const sensorHumidityDiv = document.getElementById("sensorHumidity");
const sensorCO2ppmDiv = document.getElementById("sensorCO2ppm");

async function updateData() {
    const response = await fetch("/data");
    const data = await response.json();
    sensorTemperatureDiv.innerText = data.sensors.temperature + "°C";
    sensorHumidityDiv.innerText = data.sensors.humidity + "%";
    sensorCO2ppmDiv.innerText = data.sensors.co2ppm + "ppm";
}

updateData();
setInterval(updateData, 10000);
