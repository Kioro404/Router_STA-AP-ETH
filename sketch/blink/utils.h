#define PRINTF(...) Serial.printf(__VA_ARGS__) // Trace output simplified, can be deactivated here

#define FORMAT_LITTLEFS_IF_FAILED true

#define PORT 80 // puerto de conexión

WiFiServer serverWiFi(PORT);
EthernetServer serverETH(PORT);

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte MAC_Ethernet[] = { 0x43,0x43,0x20,0x44,0x42,0x20,0x41,0x37,0x20,0x33,0x31,0x20,0x35,0x41,0x20,0x38,0x46 };
