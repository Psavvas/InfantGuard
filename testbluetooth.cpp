#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3);
boolean parentPresent = false;
const char* MAC_Address = "DD88000011EE";

void sendCommand(const char* cmd) {
  unsigned long start = millis(); //gets start time
  String response = ""; //blank string for response

  Serial.print("CMD: "); //prints command
  Serial.println(cmd);  // prints command
  BT.print(cmd); //sends command to module

  //parses all the data recived for 0.5 seconds
  while (millis() - start < 3000) {
    while (BT.available()) {
      response += (char)BT.read();
    }
  }

  //prints response
  Serial.print("RESP: ");
  Serial.println(response);
}

void scanDevices() {
  unsigned long start = millis(); //gets start time
  String response = ""; //makes blank response string

  Serial.print("CMD: AT+DISI?");
  BT.print("AT+DISI?"); //sends command to module

  //parses all the data recived for 3 seconds
  while (millis() - start < 3000) {
    while (BT.available()) {
      response += (char)BT.read();
    }
  }

  //prints response
  Serial.print("RESP: ");
  Serial.println(response);

  if (response.indexOf(MAC_Address) != -1) {
    parentPresent = true;
  } else {
    parentPresent = false;
  }

  Serial.print("----------------\nParent present: ");
  Serial.println(parentPresent ? "YES" : "NO");
}

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  delay(500);

  Serial.println("Setting up");

  sendCommand("AT");          // establishes connection
  sendCommand("ATE0");
  sendCommand("AT+IMME1");    // manual connection
  sendCommand("AT+ROLE1");    // sets device as a scanner
  sendCommand("AT+NOTI1");    // enables notifications

  // Bluetooth Scan
  scanDevices();

  Serial.println("-------------DONE-------------");
}

void loop() {
  // Nothing needed here
}