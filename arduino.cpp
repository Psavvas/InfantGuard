#include "DHT.h"
#include "BotleticsSIM7000.h"
#define SIMCOM_7000
#define BOTLETICS_PWRKEY 6

// Define Sensor Pins
#define FSR_PIN A1
#define DHT_PIN 7
#define DHT_TYPE DHT22
const int buttonPin = 2;  // Pusbutton pin is in digital 2, which allows interupt capability
DHT dht(DHT_PIN, DHT_TYPE);

// Global Varriables
const int childThreshold = 500; // Update FSR value that determines infant is present
const int nearDeadlyTemp = 90; // Update temperature value that determines near deadly temperature
const int deadlyTemp = 95; // Update temperature value that determines  deadly temperature
bool parentPresent = false;
unsigned long lastParentTextTime = 90000;
unsigned long last911TextTime = 900000;
volatile bool buttonPressed = false;

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

void setup() {
  Serial.begin(9600);
  sheildSetUp(); //SIM7000A Sheild Setup
  dht.begin();
  bluetoothSetup(); //HM-10 Bluetooth Setup
  delay(1000);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonStatus, FALLING); // Sets up interrupt on button pin

  //Startup Notification
  Serial.println("System Active");
  textCloudflare(0, 0.0, 0.0, "stat");

}

void loop() {
  int fsrValue = analogRead(FSR_PIN); //Reads and stores FSR value
  float temp = dht.readTemperature(true); // Reads and stores FSR value in farenhite
  scanDevices();
  /* Add section that checks for battery level.
    - Potetnially use battery voltage
    - Will be checked everytime void loop runs
    - Notify Users if the temperature is below necessary threshold.
  */

  // Activates sytem if child is detected
  if (fsrValue >= childThreshold) {
    //Get GPS Coordinates
    Serial.println("Child Detected");
    checkButton();
    float cord1, cord2, speed_kph, heading, altitude;
    modem.getGPS(&cord1, &cord2, &speed_kph, &heading, &altitude);
    Serial.println(parentPresent);
    if (temp >= deadlyTemp){
      //Take Action to notify parent and call 911
      if (millis() - last911TextTime > 900000) { // Sends 911 text only once every 15 minutes
        textCloudflare(temp, cord1, cord2, "emg");
        last911TextTime = millis();
        Serial.println("911 Text Sent");
      }
      if (millis() - lastParentTextTime > 90000) { // Sends parent text only once every 90 seconds
        textCloudflare(temp, cord1, cord2, "gen");
        lastParentTextTime = millis();
        Serial.println("Parent Text Sent");
      }
      Serial.println("Detected infant and deadly temperature");
    } else if (!parentPresent){
      if (millis() - lastParentTextTime > 90000) { // Sends parent text only once every 90 seconds
        textCloudflare(temp, cord1, cord2, "gen");
        lastParentTextTime = millis();
        Serial.println("Parent Text Sent");
      }
      Serial.println("Detected infant alone and non-deadly temperature");
    }
  }

  //Deactivation Button Check
  if (buttonPressed) {
    buttonPressed = false; //Reset button pressed variable
    Serial.println("System disactivated for 15 minutes.");
    textCloudflare(1000, 0.0, 0.0, "stat"); //Sent deactivation text
    delay(900000); //15 minute delay
    textCloudflare(2000, 0.0, 0.0, "stat"); //Sent reactivation text
  }
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
  sendCommand("AT");          // establishes connection
  sendCommand("ATE0");      // disable echo
  sendCommand("AT+IMME1");    // manual connection
  sendCommand("AT+ROLE1");    // sets device as a scanner
  sendCommand("AT+NOTI1");    // enables notifications
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

  for(int i = 0; i < 5; i++) {
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
      break;
    } else {
      parentPresent = false;
    }
  }
  
  Serial.print("----------------\nParent present: ");
  Serial.println(parentPresent ? "YES" : "NO");
}

void buttonStatus() {
  // Interrupt Service Routine for button press
  buttonPressed = true; //If button is pressed at any time, set buttonPressed is set to true
}
