#include <ArduinoBLE.h>

const String fob_MAC_Address = "dd:88:00:00:11:ee";
boolean parentPresent = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  Serial.println("Bluetooth® Low Energy Central scan");
  
  scanDevices();

  if(parentPresent){
    Serial.println("PARENT PRESENT!!!!!");
  } else {
    Serial.println("PARENT NOT PRESENT.....");
  }

}

void loop() {

}

void scanDevices () {
  unsigned long start = millis(); //gets start time
  parentPresent = false;

  BLE.scan();
  delay(1000);

  //Search for Bluetooth devices for 10 seconds
  while (millis() - start < 10000) {

    // check if a peripheral has been discovered
    BLEDevice peripheral = BLE.available();

    if (peripheral) {
      // discovered a peripheral
      Serial.println("----- Device found -----");

      String scannedMAC = peripheral.address();

      // print address
      Serial.print("Address: ");
      Serial.println(scannedMAC);

      //Checks to see is fob was detected
      if(scannedMAC.equals(fob_MAC_Address)){
        //Checks RSSI value to see if parent is present
        int scannedRSSI = peripheral.rssi();

        // print the RSSI
        Serial.print("RSSI: ");
        Serial.println(peripheral.rssi());

        if(scannedRSSI <= -80){
          parentPresent = false;
          Serial.println("BLE out of range.");
          break;
        } else {
          parentPresent = true;
          Serial.println("BLE in range!");
          break;
        }
      }

      Serial.println();
    }
  }

}
