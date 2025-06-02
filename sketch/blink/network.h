void initFS(){
	if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) PRINTF("Montado de LittleFS fallido\n"); // Formatea los archivos del sistema en caso de no estarlo
	else {
		const std::vector<const char*> path = {
			"/assets/icons",
			"/config",
			"/css",
			"/js",
			"/pages"
		};
		PRINTF("Listando directorios\n");
		for (int i = 0; i < path.size(); i++) listDir(path[i], 0); // Muestra los archivos en la memoria flash
	}
}

JsonDocument Jdoc; // create a JSON container

std::vector<std::vector<String>> JdocWiFiOrg, JdocAPOrg, JdocEthernetOrg;

void initNetwork() {
	// static int attemp = 0;
	// Network.onEvent([] (arduino_event_id_t event, arduino_event_info_t info) {
	//   PRINTF(" ");
	//   switch (event) {
	//     case ARDUINO_EVENT_WIFI_STA_START:          PRINTF("Wi-Fi iniciado"); break;
	//     case ARDUINO_EVENT_WIFI_STA_CONNECTED:      PRINTF("Wi-Fi conectado"); break;
	//     case ARDUINO_EVENT_WIFI_STA_GOT_IP:         // PRINTF("Wi-Fi IP asignada: %s", WiFi.localIP().toString().c_str());
	//       // WiFi.AP.enableNAPT(true);
	//       break;
	//     case ARDUINO_EVENT_WIFI_STA_LOST_IP:        PRINTF("Wi-Fi perdió la dirección IP");
	//       // WiFi.AP.enableNAPT(false);
	//       break;
	//     case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:   if (attemp++ == 0) { PRINTF("Wi-Fi desconectado");
	//       // WiFi.AP.enableNAPT(false);
	//       }
	//       break;
	//     case ARDUINO_EVENT_WIFI_STA_STOP:           PRINTF("Wi-Fi detenido"); break;

	//     case ARDUINO_EVENT_WIFI_AP_START:           PRINTF("AP iniciado"); break;
	//     case ARDUINO_EVENT_WIFI_AP_STACONNECTED:    PRINTF("AP conectado"); break;
	//     case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED: PRINTF("AP desconectado"); break;
	//     case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:   PRINTF("AP IP asignada: %s", IPAddress(info.wifi_ap_staipassigned.ip.addr).toString().c_str()); break;
	//     case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:  PRINTF("AP recibiendo respuesta"); break;
	//     case ARDUINO_EVENT_WIFI_AP_STOP:            PRINTF("AP detenido"); break;

	//     default: break;
	//   }
	//   PRINTF("\n");
	// });
	
	DeserializationError error = deserializeJson(Jdoc, readFile("/config/network.json"));
	if (error) {
		PRINTF(F("deserializeJson() fallido: %s"), error.f_str());
		return;
	}
	else {
		JsonObject	WiFi_addresses = Jdoc["WiFi"]["addresses"], WiFi_credentials = Jdoc["WiFi"]["credentials"],
								AP_addresses = Jdoc["AP"]["addresses"], AP_credentials = Jdoc["AP"]["credentials"],
								Ethernet_addresses = Jdoc["Ethernet"]["addresses"];

		JdocWiFiOrg = {   // Original
			{ // enable
				Jdoc["WiFi"]["enable"]
			},
			{ // dhcp
				Jdoc["WiFi"]["dhcp"]
			},
			{ // credentials
				WiFi_credentials["ssid"],
				WiFi_credentials["pass"]
			},
			{ // addresses
				WiFi_addresses["address"],
				WiFi_addresses["gateway"],
				WiFi_addresses["subnet"],
				WiFi_addresses["dns1"],
				WiFi_addresses["dns2"]
			}
		};
		JdocAPOrg = {
			{ // enable
				Jdoc["AP"]["enable"]
			},
			{ // dhcp
				Jdoc["AP"]["dhcp"]
			},
			{ // credentials
				AP_credentials["ssid"],
				AP_credentials["pass"]
			},
			{ // addresses
				AP_addresses["address"],
				AP_addresses["gateway"],
				AP_addresses["subnet"],
				AP_addresses["since"],
				AP_addresses["dns"]
			}
		};
		JdocEthernetOrg = {
			{ // enable
				Jdoc["Ethernet"]["enable"]
			},
			{ // dhcp
				Jdoc["Ethernet"]["dhcp"]
			},
			{ // addresses
				Ethernet_addresses["address"],
				Ethernet_addresses["gateway"],
				Ethernet_addresses["subnet"],
				Ethernet_addresses["dns"]
			}
		};

		// PRINTF("%s\n", readFile(dirFile[Jconfig]));
		
		const char* HOSTNAME = Jdoc["Hostname"]; // name of the server.
		
		if ((Jdoc["WiFi"]["enable"] == true) || (Jdoc["AP"]["enable"] == true)) {
			PRINTF("\nIniciando %s\n\n", HOSTNAME);
			
			if ((Jdoc["WiFi"]["enable"] == true) && (Jdoc["AP"]["enable"] == false)) WiFi.mode(WIFI_STA);
			else if ((Jdoc["WiFi"]["enable"] == false) && (Jdoc["AP"]["enable"] == true)) WiFi.mode(WIFI_AP);
			else if ((Jdoc["WiFi"]["enable"] == true) && (Jdoc["AP"]["enable"] == true)) WiFi.mode(WIFI_AP_STA);

			if (Jdoc["WiFi"]["enable"] == true) {
				if (Jdoc["WiFi"]["dhcp"] == false) {
					WiFi.config(toIPAddress(WiFi_addresses["address"]), toIPAddress(WiFi_addresses["gateway"]), toIPAddress(WiFi_addresses["subnet"]));
					
					if ((strcmp(WiFi_addresses["dns1"], "") != 0) && (strcmp(WiFi_addresses["dns2"], "") != 0)) WiFi.setDNS(toIPAddress(WiFi_addresses["dns1"]), toIPAddress(WiFi_addresses["dns2"]));
				}
				
				WiFi.setHostname(HOSTNAME);
				WiFi.begin((const char*)WiFi_credentials["ssid"], (const char*)WiFi_credentials["pass"]); // Iniciar la conexión del WiFi

				int attemp = 0;
				while ((WiFi.status() != WL_CONNECTED) && (attemp++ <= 5)) delay(1000); // Espera a que se conecte el Wi-Fi
				if (WiFi.status() != WL_CONNECTED) WiFi.disconnect(); // Desconecta el Wi-Fi en caso de no haberse establecido la conexión
				else {
					WiFi.setAutoReconnect(true);
					WiFi.AP.enableNAPT(true); // Habilita el NAPT, necesario para que comparta el internet del Wi-Fi
					
					PRINTF("Abre en el navegador <http://%s> para Wi-Fi\n", WiFi.localIP().toString().c_str());
				}
			}
			else WiFi.disconnect();
			
			if (Jdoc["AP"]["enable"] == true) {
				PRINTF("%s", (WiFi.status() == WL_CONNECTED) ? "o\n" : "");
				
				if (Jdoc["AP"]["dhcp"] == false) WiFi.softAPConfig(toIPAddress(AP_addresses["gateway"]), toIPAddress(AP_addresses["gateway"]), toIPAddress(AP_addresses["subnet"]), toIPAddress(AP_addresses["address"]), toIPAddress(AP_addresses["dns"]));
				
				WiFi.softAPsetHostname(HOSTNAME);
				WiFi.softAP((const char*)AP_credentials["ssid"], (const char*)AP_credentials["pass"]); // Iniciar la conexión del AP
				WiFi.AP.enableNAPT(true);

				PRINTF("Abre en el navegador <http://%s> para AP\n", WiFi.softAPIP().toString().c_str());
			}
			else WiFi.softAPdisconnect(true);
		}
		
		if ((Jdoc["Ethernet"]["enable"] == true)) {
			
			Ethernet.setHostname(HOSTNAME);
			
			#define ETH_CS       5
			#define ETH_SPI_SCK  18
			#define ETH_SPI_MISO 19
			#define ETH_SPI_MOSI 23
			
			SPI.begin(ETH_SPI_SCK, ETH_SPI_MISO, ETH_SPI_MOSI);
			// You can use Ethernet.init(pin) to configure the CS pin
			Ethernet.init(ETH_CS);   // ESP32
			
			// Check for Ethernet hardware present
			if (Ethernet.hardwareStatus() == EthernetNoHardware) {
				PRINTF("Ethernet shield was not found.  Sorry, can't run without hardware. :(\n");
				while (true) {
					delay(1); // do nothing, no point running without Ethernet hardware
				}
			}
			if (Ethernet.linkStatus() == LinkOFF) Serial.println("Ethernet cable is not connected.");
			else PRINTF("o\n");
			
			if (Jdoc["Ethernet"]["dhcp"] == false) {
				Ethernet.begin(MAC_Ethernet, toIPAddress(Ethernet_addresses["address"]), toIPAddress(Ethernet_addresses["dns"]), toIPAddress(Ethernet_addresses["gateway"]), toIPAddress(Ethernet_addresses["subnet"]));
			}
			else {
				
				if (Ethernet.begin(MAC_Ethernet)) Serial.println("Configure Ethernet using DHCP");
				else {
					Ethernet.begin(MAC_Ethernet, toIPAddress(Ethernet_addresses["address"])); // try to configure using IP address instead of DHCP:
					PRINTF("Configure Ethernet using fake DHCP\n");
				}
			}
			
			PRINTF("Abre en el navegador <http://%s> para Ethernet\n", Ethernet.localIP().toString().c_str());
		}
		
	}
}

// std::map<byte, IPAddress> clientMap; // Almacena las direcciones ip de las conexiones

void initServer() { // Inicia el servidor

	// server.begin(); // start WebServer
  serverWiFi.begin();
  serverETH.begin();
	
  // webSockets.begin();
	
	// webSocket.begin(); // start WebSocket
			
	// webSocket.onEvent([] (byte num, WStype_t type, uint8_t* payload, size_t length) {      // the parameters of this callback function are always the same -> num: id of the client who send the event, type: type of message, payload: actual data sent and length: length of payload
	// webSockets.onEvent([] (const uint8_t& num, const WStype_t& type, uint8_t * payload, const size_t& length) {      // the parameters of this callback function are always the same -> num: id of the client who send the event, type: type of message, payload: actual data sent and length: length of payload
	//  switch (type) {                                     // switch on the type of information sent
	// 		case WStype_DISCONNECTED:                         // if a client is disconnected, then type == WStype_DISCONNECTED
	// 			if (clientMap.find(num) != clientMap.end()) {
	// 				PRINTF("Cliente %s desconectado", clientMap[num].toString().c_str());
	// 				clientMap.erase(num);
	// 			}
	// 			break;
	// 		case WStype_CONNECTED:                            // if a client is connected, then type == WStype_CONNECTED
	// 			clientMap[num] = webSockets.remoteIP(num);
	// 			PRINTF("Cliente %s conectado", clientMap[num].toString().c_str());
	// 			break;
	// 		case WStype_TEXT:                                 // if a client has sent data, then type == WStype_TEXT
	// 			// try to decipher the JSON string received
	// 			DeserializationError error = deserializeJson(Jdoc, payload);
	// 			if (error) {
	// 				PRINTF(F("deserializeJson() fallido: %s"), error.f_str());
	// 				return;
	// 			}
	// 			else {
	// 				// serializeJsonPretty(Jdoc, Serial); // Print the result
					
	// 				JnetworkChange();
	// 			}
	// 			break;
	// 	}
	// 	PRINTF("\n");

	// }); // define a callback function -> what does the ESP32 need to do when an event from the websocket is received? -> run function "webSocketEvent()"
}

template <typename Client>
void handleClient(Client client) {
  if (client) {
    String req = "";
    // Leer cabeceras
    while (client.connected()) {
			if (client.available()) {
        req += (char)client.read();
				if (req.endsWith("\r\n\r\n")) break; // Fin de cabeceras
      }
    }

    // Extraer la ruta solicitada
    String path = "/";
    int idx1 = req.indexOf(' ');
    int idx2 = req.indexOf(' ', idx1 + 1);
    if (idx1 != -1 && idx2 != -1) {
      path = req.substring(idx1 + 1, idx2);
    }
		
		int contentLength = 0;
    // Buscar Content-Length
    int clIdx = req.indexOf("Content-Length:");
    if (clIdx != -1) {
      int clEnd = req.indexOf("\r\n", clIdx);
      String clStr = req.substring(clIdx + 15, clEnd);
      contentLength = clStr.toInt();
    }

    // Leer el JSON
    String body = "";
    while (body.length() < contentLength && client.connected()) {
      if (client.available()) {
        body += (char)client.read();
      }
    }

		if (contentLength > 0) {
			if (req.startsWith("POST /api/data")) {
				// Procesa el JSON aquí
				DeserializationError error = deserializeJson(Jdoc, body);
				if (error) {
					client.print("HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\n\r\n");
					client.print("{\"status\":\"error\",\"msg\":\"JSON inválido\"}");
				}
				else {
					// serializeJsonPretty(Jdoc, Serial); // Print the result
					client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
					client.print("{\"status\":\"ok\"}");
					// Aquí puedes guardar el JSON si lo deseas
					JnetworkChange();
				}
				
    		client.stop();
				return; // Salir después de procesar el JSON
			}
		}
		
    // Buscar el archivo solicitado en dirFile
    bool found = false;
    for (int i = 0; i < dirFile.size(); i++) {
      if (path == dirFile[i]) {
        client.print("HTTP/1.1 200 OK\r\n");
        client.print("Content-Type: ");
        client.print(typeFile(dirFile[i]));
        client.print("\r\n");
        client.print("Connection: close\r\n");
        client.print("Access-Control-Allow-Origin: *\r\n\r\n");
        client.print(readFile(dirFile[i]));
        found = true;
        break;
      }
      if (path == "/" && String(dirFile[i]).endsWith("index.html")) {
        client.print("HTTP/1.1 302 Found\r\n");
        client.print("Location: ");
        client.print(dirFile[i]);
        client.print("\r\n\r\n");
        found = true;
        break;
      }
    }

    if (!found) {
      for (int i = 0; i < dirFile.size(); i++) {
        if (String(dirFile[i]).endsWith("404_error.html")) {
          client.print("HTTP/1.1 404 Not Found\r\n");
          client.print("Content-Type: ");
          client.print(typeFile(dirFile[i]));
          client.print("\r\n");
          client.print("Connection: close\r\n\r\n");
          client.print(readFile(dirFile[i]));
          found = true;
          break;
        }
      }
    }

    delay(10);
    client.stop();
  }
	
	delay(1);  // give the webserver a little time to process the request
}

void JnetworkChange(){

	bool change = false;

	for (int WA = 0; WA < 3; WA++){
		std::vector<std::vector<String>> JdocType = {
			{ // enable
				"enable"
			},
			{ // dhcp
				"dhcp"
			},
			{ // credentials
				"ssid",
				"pass"
			},
			{ // addresses
				"address",
				"gateway",
				"subnet"
			}
		};
		const String tNet = (WA == 0) ? "WiFi" : (WA == 1) ? "AP": "Ethernet"; // tNet = type of network
		
		switch (WA) {
			case 0: /*WiFi*/ for (int dns = 1; dns <= 2; dns++) JdocType[3].push_back("dns" + String(dns)); break;
			case 1: /*AP*/ JdocType[3].push_back("dns"); break;
			case 2: /*Ethernet*/
				JdocType[3].push_back("dns");
				JdocType.erase(JdocType.begin() + 2); // Elimina las credenciales de la red Ethernet, ya que no se utilizan
				break;
		}
		
		for (int i = 0; i < JdocType.size(); i++) {
			for (int j = 0; j < JdocType[i].size(); j++) {
				String JdocMod;
				if (i <= 1) JdocMod = (String)Jdoc[tNet][JdocType[i][j]]; // Se utiliza (String) para obtener true o false respectivamente, debido a que (const char*) o (bool) no devuelven el valor esperado para ser modificado
				else if (tNet != "Ethernet") JdocMod = (String)Jdoc[tNet][(i == 2) ? "credentials" : "addresses"][JdocType[i][j]];
				else JdocMod = (String)Jdoc[tNet]["addresses"][JdocType[i][j]]; // En caso de ser Ethernet, las credenciales no existen, por lo que se utiliza el índice 2 para acceder a las direcciones
				
				// PRINTF("%s\n", JdocMod.c_str());
				
				if (JdocMod != ((tNet == "WiFi") ? JdocWiFiOrg[i][j] : (tNet == "AP") ? JdocAPOrg[i][j] : JdocEthernetOrg[i][j])) { // Si el valor es diferente al original, se modifica
					change = true;
					modJFile(tNet.c_str(), JdocType[i][j].c_str(), (i <= 1) ? JdocMod : "\"" + JdocMod + "\""); // Cambia el valor de 'message'
				}
			}
		}
	}

	// Serial.println(readFile(dirFile[Jconfig]));

	if (change) {
		PRINTF("Reiniciando\n");
		esp_restart(); // reboot the ESP32
	}
}