<img src="./assets/Mac App icon.png" width="300" />


---
Der *AirMate* ist ein sich automatisch öffnendes Fenster, welches sich anhand von Sensordaten selbst öffnet. Er hostet außerdem eine Website, welche die Sensordaten anzeigt. Über diese und einen Knopf am Fenster selbst lässt sich das Fenster auch manuell öffnen.

Als Projekt ist es im Rahmen eines Moduls des Informatikstudiums entstanden. Es hatte dabei die Zielsetzung eines der Nachhaltigkeitsziele der UN zu erfüllen. Wir haben uns hier auf das Ziel der Inklusion konzentriert und haben ein Konzept erstellt welches Menschen mit Einschränkungen das alltägliche Leben leichter machen soll. Da wir mit dem CO2- und Temperatursensor schon geplant hatten war es für uns einfach diese noch zu erweitern um auch eine Einbruchssicherung zu erweitern. 


# Die Software

Mittels einer API speisen wir die Wetterdaten aus dem Internet ein. Zusammen mit den Sensordaten, welche der ESP direkt erhält, entscheidet dieser dann ob das Fenster geöffnet werden sollte oder geschlossen und passt den jeweiligen Zustand an. Über den ESP selbst wird dann noch eine Website dargestellt welche die Sensordaten darstellt und es erlaubt das Fenster zu öffnen.

# Die Hardware

## Die Sensoren
Diese könnten auch weggelassen werden und das Fenster nur über Knopf und Website geöffnet werden.

- DHT11 
	Temperatur- & Feuchtigkeitssensor 
- MH-Z19C
	CO2-Sensor
- KY-021
	Reedschalter / Magnetsensor

## Der ESP
Achtung Schaltplan sieht andere Pins vor als vorhanden. 

- ESP32 
- D1 Mini DC Powershield

## Die Motoren
Von der Leistung her sind sie auf ein Leichtholzfenster im Eigenbau ausgelegt

- Nema 17 mit A4988
	Stepper-Motor für das Öffnen des Fensters
- 28BYJ-48 mit ULN2003
	Stepper-Motor für das betätigen das Verschlussmechanismus des Fensters

## 3D Druck
Modelle sind in den assets zu finden. 

- Kasten und Deckel für Unterbringung des ESPS, sowie einiger Sensoren
- Anbringung für Motor 
- Riegel für Verschluss
- Zahnrad für Öffnen
- Zahnschiene für Öffnen 

# Beitragende
- [DanielPXL](https://github.com/DanielPXL)
- [Kaskary7](https://github.com/Kaskary7)
- [Oppermannt](https://github.com/Oppermannt)
- [TheSacredCactus](https://github.com/TheSacredCactus)
- [Tomerius135](https://github.com/Tomerius135)
- [Quantkas](https://github.com/Quantkas)

# Lizens
Jeder Part dieses Projektes kann genutzt und angepasst werden, allerdings stellt es in sich selbst keine komplette Anleitung dar.
[GPL](https://choosealicense.com/licenses/gpl-3.0/)
