#include "DHT.h"
#include "BotleticsSIM7000.h"
#include <SoftwareSerial.h>

// Define Sensor Pins
#define FSR_PIN A1
#define DHT_PIN 7
#define DHT_TYPE DHT22
const int buttonPin = 2;  // the number of the pushbutton pin
DHT dht(DHT_PIN, DHT_TYPE);

// Global Varriables
const int childThreshold = 50; // Update FSR value that determines infant is present
const int nearDeadlyTemp = 90; // Update temperature value that determines near deadly temperature
const int deadlyTemp = 95; // Update temperature value that determines  deadly temperature
bool parentPresent = false;
int buttonState = 0;  // variable for reading the pushbutton status
unsigned long lastParentTextTime = 0;
unsigned long last911TextTime = 0;

//SMS Variables
char URL[256];
char cord1_buf[16];
char cord2_buf[16];
char temp_buf[16];
uint16_t statuscode;
uint16_t length;

//Sheild Variables
#define TX 10 //SIM7000A Shield
#define RX 11 //SIM7000A Sheild
SoftwareSerial modemSS = SoftwareSerial(TX, RX);
SoftwareSerial *modemSerial = &modemSS;
Botletics_modem_LTE modem = Botletics_modem_LTE(); // Instantiate modem LTE class

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("System Active");
  delay(1000);
  pinMode(buttonPin, INPUT);
  sheildSetUp();

  /* Add shield setup data and potentially a notification text to the user letting them know that the sytem is active.*/
}

void loop() {
  int fsrValue = analogRead(FSR_PIN); //Reads and stores FSR value
  float temp = dht.readTemperature(true); // Reads and stores FSR value in farenhite
  buttonState = digitalRead(buttonPin);
  //Verify if parent is present via bluetooth fob and returns true if present and false if not

  /* Add section that checks for battery level.
    - Potetnially use battery voltage
    - Will be checked everytime void loop runs
    - Notify Users if the temperature is below necessary threshold.
  */

  // Activates sytem if child is detected
  if (fsrValue >= childThreshold) {
    //Get GPS Coordinates
    float cord1, cord2, speed_kph, heading, altitude;
    modem.getGPS(&cord1, &cord2, &speed_kph, &heading, &altitude);

    if (temp >= deadlyTemp){
      //Take Action to notify parent and call 911
      textCloudflare(temp, cord1, cord2, "emg");
      textCloudflare(temp, cord1, cord2, "gen");
      Serial.println("Detected infant and deadly temperature");
    } else if (!parentPresent){
      textCloudflare(temp, cord1, cord2, "gen");
      Serial.println("Detected infant alone and non-deadly temperature");
    }
  }

  if (buttonState == HIGH) {
    Serial.println("System disactivated for 15 minutes.");
    delay(900000); //15 minute delay
  }
  
  //For debugging purposes only
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Force: ");
  Serial.println(fsrValue);
  delay(2000);
  
}

void textCloudflare(float temp, float cord1, float cord2, char* type){
  dtostrf(cord1, 1, 6, cord1_buf);
  dtostrf(cord2, 1, 6, cord2_buf);
  dtostrf(temp,  1, 1, temp_buf);
  nprintf(URL, sizeof(URL), "http://webhook.site/infantguard/?cord1=%s&cord2=%s&temp=%s&type=%s", cord1_buf, cord2_buf, temp_buf, type);
  modem.HTTP_GET_start(URL, &statuscode, &length);
  modem.HTTP_GET_end();
}

void sheildSetUp(){
  //Setup code for the SIM7000A Sheild
  modemSS.println("AT+IPR=9600"); // Manually set baud rate regardless of whether or not modem is actually on 115200
  delay(100);
  modemSS.begin(9600);
  modem.begin(modemSS);
  modem.setNetworkSettings(F("hologram")); // For Hologram SIM card
  modem.openWirelessConnection(true); // Enable connection
  modem.enableGPS(true); // Enable GPS
  delay(10000);
  Serial.println("Setup Completed"); // For debugging purposes only
}
