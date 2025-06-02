#include <Arduino.h>

#include <WiFi.h>
#include <EthernetENC.h>

#include <ArduinoJson.h>       // needed for JSON encapsulation (send multiple variables with one string)

#include <SPI.h>
#include <FS.h>
#include <LittleFS.h>

#include <vector>
#include <cstring>
#include <map>

#include "utils.h"
#include "functions.h"
#include "network.h"

// Setup everything to make the webserver work.
void setup(void) {

	delay(3000);  // wait for serial monitor to start completely.

	// Use Serial port for some trace information
	Serial.begin(115200);
	Serial.setDebugOutput(false);

	PRINTF("\n\n");

	initFS();
	
	initNetwork();
	
	initServer();

}  // setup

// run the server...
void loop(void) {
	handleClient(serverWiFi.available());
	handleClient(serverETH.available());
}  // loop()
