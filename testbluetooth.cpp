#include <SoftwareSerial.h>

SoftwareSerial BT(8, 9);
const char* TARGET_MAC = "DD88000011EE";

bool parentPresent = false;

String sendCommand(String cmd, unsigned long timeout = 500) {
  BT.print(cmd);
  unsigned long start = millis();
  String resp = "";

  while (millis() - start < timeout) {
    while (BT.available()) {
      resp += (char)BT.read();
    }
  }
  return resp;
}

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  delay(1500);

  Serial.println("Configuring HM-10...");

  sendCommand("AT");
  sendCommand("AT+RESET");
  sendCommand("AT+ROLE1");
  sendCommand("AT+IMME1");
  sendCommand("AT+NOTI1");

  // Start scan
  sendCommand("AT+DISI?");

  unsigned long scanStart = millis();
  String scanData = "";

  while (millis() - scanStart < 2500) {  // collect ~2.5s of data
    while (BT.available()) {
      char c = BT.read();
      scanData += c;
      Serial.write(c);
    }
  }

  Serial.println("\n--- End of Scan ---");

  // Convert to uppercase (HM-10 sometimes mixes case, depending on firmware)
  scanData.toUpperCase();

  // Check for MAC anywhere in the scan dump
  if (scanData.indexOf(TARGET_MAC) != -1) {
    parentPresent = true;
  } else {
    parentPresent = false;
  }

  Serial.print("Parent present: ");
  Serial.println(parentPresent ? "YES" : "NO");
}

void loop() {}

