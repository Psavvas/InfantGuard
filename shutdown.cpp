#include "BotleticsSIM7000.h"
#define SIMCOM_7000
#define BOTLETICS_PWRKEY 6

#define MODEM_SERIAL Serial1   // TX1=18, RX1=19
Botletics_modem_LTE modem = Botletics_modem_LTE(); // Instantiate modem LTE class
const uint8_t modem_PWRKEY = 6; // for botletics SIM7000 shield
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  shieldSetUp(); //SIM7000A Shield Setup
  modem.powerDown(); // Power on the module
  Serial.println("Done");
}

void loop() {
  // put your main code here, to run repeatedly:

}
void shieldSetUp(){
  //Setup code for the SIM7000A Shield
  MODEM_SERIAL.begin(115200);
  delay(1000);
  MODEM_SERIAL.println("AT+IPR=9600"); // Manually set baud rate regardless of whether or not modem is actually on 115200
  delay(100); // Short pause to let the command run
  MODEM_SERIAL.begin(9600);
  modem.begin(MODEM_SERIAL);
  modem.setFunctionality(1); // AT+CFUN=1
  modem.setNetworkSettings(F("hologram")); // For Hologram SIM card
  modem.openWirelessConnection(true); // Enable connection
  modem.enableGPRS(true); // Enable GPRS
  modem.enableGPS(true); // Enable GPS
  delay(10000);
}
