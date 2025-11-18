#include <SoftwareSerial.h>

// HM-10 wiring
SoftwareSerial BT(8, 9);  // RX = 8, TX = 9

// MAC address you want to detect
const char* TARGET_MAC = "DD88000011EE";

bool parentPresent = false;

// Sliding window for MAC detection (12 chars)
char macWindow[13];
uint8_t macPos = 0;

// Send AT command helper
String sendCommand(const char* cmd, unsigned long timeout = 500) {
  BT.print(cmd);

  unsigned long start = millis();
  String resp = "";

  while (millis() - start < timeout) {
    if (BT.available()) {
      resp += (char)BT.read();
    }
  }

  resp.trim();
  Serial.print("CMD: ");
  Serial.print(cmd);
  Serial.print(" | RESP: ");
  Serial.println(resp);

  return resp;
}

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  delay(2000);

  Serial.println("Configuring HM-10...");

  sendCommand("AT");
  sendCommand("AT+RESET");
  sendCommand("AT+ROLE1");
  sendCommand("AT+IMME1");
  sendCommand("AT+NOTI1");

  // Start 1-time scan
  sendCommand("AT+DISI?");

  unsigned long lastByteTime = millis();

  Serial.println("Scanning...");

  while (true) {
    while (BT.available()) {
      char c = BT.read();
      Serial.write(c);  // Debug output to Serial Monitor

      // Normalize to uppercase
      if (c >= 'a' && c <= 'z') {
        c -= 32;
      }

      // Fill sliding MAC window
      macWindow[macPos++] = c;
      if (macPos >= 12) macPos = 0;

      // Build potentialBeacon string in correct order
      char potentialBeacon[13];
      for (uint8_t i = 0; i < 12; i++) {
        potentialBeacon[i] = macWindow[(macPos + i) % 12];
      }
      potentialBeacon[12] = '\0';

      // Compare
      if (strcmp(potentialBeacon, TARGET_MAC) == 0) {
        parentPresent = true;
      } else {
        parentPresent = false;
      }

      lastByteTime = millis();
    }

    // No data for 2 seconds → end scan
    if (millis() - lastByteTime > 2000) {
      Serial.println("\n--- End of Scan ---");
      break;
    }
  }

  Serial.print("Parent present: ");
  Serial.println(parentPresent ? "YES" : "NO");
}

void loop() {
  // Nothing — one-time scan only
}
