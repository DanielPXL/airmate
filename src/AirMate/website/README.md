# AirMate Webinterface

Innerhalb dieses Ordners befindet sich das Webinterface für den AirMate. Dieses ist in Vanilla JS, HTML und CSS geschrieben.

Das Webinterface wird in eine einzige HTML-Datei kompiliert, anschließend komprimiert und dann in einen C-Header umgewandelt, der in das AirMate-Projekt included wird. Dies ist **nicht** in dem Kompilierprozess des AirMate-Projekts enthalten und muss separat durchgeführt werden, wenn das Webinterface bearbeitet wurde.

Damit nicht jeder npm usw. installieren muss, um das AirMate-Projekt zu kompilieren, ist im Repository bereits eine kompilierte Version des Webinterface enthalten.

# Kompilieren

Folgendes muss installiert sein:

- Node.js und npm
- gzip
- xxd

Folgende Befehle zum Kompilieren ausführen:

```bash
npm install
npm run build
```
