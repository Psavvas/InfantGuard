#include "DHT.h"
#include "BotleticsSIM7000.h"
#include <SoftwareSerial.h>
#define SIMCOM_7000
#define BOTLETICS_PWRKEY 6

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
char data[80];
uint16_t statuscode;
uint16_t length;

//Sheild Variables
#define MODEM_SERIAL Serial1   // TX1=18, RX1=19
Botletics_modem_LTE modem = Botletics_modem_LTE(); // Instantiate modem LTE class

void setup() {
  Serial.begin(9600);
  sheildSetUp();
  dht.begin();
  delay(1000);
  pinMode(buttonPin, INPUT);
  
  float cord1, cord2, speed_kph, heading, altitude;
  modem.getGPS(&cord1, &cord2, &speed_kph, &heading, &altitude);
  Serial.println(cord1);
  Serial.println(cord2);
  textCloudflare(103.3, cord1, cord2, "emg");
  textCloudflare(103.3, cord1, cord2, "gen");

  /* Add shield setup data and potentially a notification text to the user letting them know that the sytem is active.*/
}

void loop() {
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

