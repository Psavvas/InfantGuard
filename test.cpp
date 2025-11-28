#include "DHT.h"
#include "BotleticsSIM7000.h"
#include <SoftwareSerial.h>
#define SIMCOM_7000
#define BOTLETICS_PWRKEY 6

//SMS Variables
char URL[256];
char cord1_buf[16];
char cord2_buf[16];
char temp_buf[16];
char data[80];
uint16_t statuscode;
uint16_t length;

//Sheild Variables
#define MODEM_SERIAL Serial1   // TX1=18, RX1=19
Botletics_modem_LTE modem = Botletics_modem_LTE(); // Instantiate modem LTE class

//HM-10 Bluetooth Variables
#define BT Serial2   // TX2=16, RX2=17
const char* MAC_Address = "DD88000011EE";
bool parentPresent = false;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Arduino On");
  bluetoothSetup();
  Serial.println("Bluetooth Set Up.");
  sheildSetUp();
  Serial.println("Sheild Set Up."); 

}

void loop() {
  delay(1000);
  Serial.println("Bluetooth Active");
  scanDevices();
  delay(1000);
  Serial.println("SIM7000A Active");
  delay(1000);
  for(int i = 0; i < 10; i++){
    float cord1, cord2, speed_kph, heading, altitude;
    modem.getGPS(&cord1, &cord2, &speed_kph, &heading, &altitude);
    Serial.println("GPS Cordinates recorded.");
    if(parentPresent){
        textCloudflare(100, cord1, cord2, "gen");
        Serial.println("General Text attempted to send Parent.");
    } else{
        textCloudflare(100, cord1, cord2, "emg");
        Serial.println("Emergency Text attempted to send 911.");    
    }
  }
  Serial.println("Done with texts");
  delay(10000);
}

void textCloudflare(float temp, float cord1, float cord2, char* type){
  dtostrf(cord1, 1, 6, cord1_buf);
  dtostrf(cord2, 1, 6, cord2_buf);
  dtostrf(temp,  1, 1, temp_buf);
  snprintf(URL, sizeof(URL), "http://webhook.site/infantguard/?cord1=%s&cord2=%s&temp=%s&type=%s", cord1_buf, cord2_buf, temp_buf, type);
  modem.HTTP_GET_start(URL, &statuscode, &length);
  modem.HTTP_GET_end();
}

void sheildSetUp(){
  //Setup code for the SIM7000A Sheild
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

void bluetoothSetup(){
  //HM-10 Bluetooth Setup
  BT.begin(9600);
  delay(1500);
  delay(1000);
  sendCommand("AT");          // establishes connection
  sendCommand("ATE0");      // disable echo
  sendCommand("AT+IMME1");    // manual connection
  sendCommand("AT+ROLE1");    // sets device as a scanner
  sendCommand("AT+NOTI1");    // enables notifications
  delay(1000);
}

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
  delay(1000);
  unsigned long start = millis(); //gets start time
  String response = ""; //makes blank response string

  Serial.print("CMD: AT+DISI?");
  BT.print("AT+DISI?"); //sends command to module

  //parses all the data recived for 5 seconds
  while (millis() - start < 5000) {
    while (BT.available()) {
      response += (char)BT.read();
    }
  }
  start = millis(); //resets start time

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
