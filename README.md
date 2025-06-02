# Router-WIFI_AP_STA

Está diseñada para un **ESP32**, aunque un **ESP8266** también debería de funcionar cambiando algunas cosas.

Su funcionamiento es simple:

- ## Conexión inalámbrica

	- **Wi-Fi** y **AP**:
		- **Habilita** o **deshabilita** la conexión inalámbrica.
		- Configura el **SSID** y la **contraseña**.
		- Establece una conexión automática **DHCP** o **estática**.
	- **AP**:
		- Funciona cómo extensor de rango **Wi-Fi**, además de cumplir con la función de compartir el internet mediante **NAPT** (solo con la librería **<WiFi.h>** de **Arduino IDE/CLI** no funciona la de **PlatformIO**).

Todo gestionado desde una interfaz web desde la que se puede interactuar muy fácilmente.

## Método de instalación

- ### Requisitos para la instalación
	- Librerías necesarias:
		- **ArduinoJson:** https://github.com/bblanchon/ArduinoJson
		- **arduinoWebSockets:** https://github.com/Links2004/arduinoWebSockets
	- Extenciones:
		- **arduino-littlefs-upload** (para ArduinoIDE): https://github.com/earlephilhower/arduino-littlefs-upload
	- Subir el sketch:
		- **Automáticamente:** Tener conectado el **ESP32** al computador y especificar el puerto **USB** en el que está conectado y el tipo de **placa** para una instalación más rápida.
		- **Manualmente:** Subir el sketch con **Arduino IDE**