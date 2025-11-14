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

//Sheild Variables:
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
  int fsrValue = analogRead(FSR_PIN); //Reads anbd stores FSR value
  float temp = dht.readTemperature(true); // Reads anbd stores FSR value in farenhite
  buttonState = digitalRead(buttonPin);
  //Record location via SIM7000A shield
  //Verify if parent is present via bluetooth fob and returns true if present and false if not

  /* Add section that checks for battery level.
    - Potetnially use battery voltage
    - Will be checked everytime void loop runs
    - Notify Users if the temperature is below necessary threshold.
  */

  // Activates sytem if child is detected
  if (fsrValue >= childThreshold) {
    if (temp >= deadlyTemp){
      //Take Action to notify parent and call 911
      Serial.println("Detected infant and deadly temperature");
    } else if (temp >= nearDeadlyTemp && !parentPresent){
      textParent();
      Serial.println("Detected infant alone and near-deadly temperature");
    } else if (!parentPresent){
      textParentNormal()
      Serial.println("Detected infant alone and non-deadly temperature");
    }
  }

  if (buttonState == HIGH) {
    Serial.println("System disactivated for 15 minutes.");
    delay(900000); //15 minute delay
  }
  
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Force: ");
  Serial.println(fsrValue);
  delay(2000);
  
}
// Functions to notify parent/911
void textParentNormal(){
  if (millis() - lastParentTextTime >= 90000){
    //Code to text parents that ifant is forgotten but temperature is normal
    lastParentTextTime = millis();
  }
}

void textParentNearDeadly(){
  if (millis() - lastParentTextTime >= 90000){
    //Code to text parents that ifant is forgotten and temperature is near deadly
    lastParentTextTime = millis();
  }
}

void textParentsAnd911(){
  if (millis() - last911TextTime >= 1800000){
    //Code to text 911 that infant is forgotten and temperature deadly
    last911TextTime = millis();
  }
  if (millis() - lastParentTextTime >= 900000){
    //Code to text 911 that infant is forgotten and temperature deadly
    lastParentTextTime = millis();
  }
}
void sheildSetUp(){
  //Setup code for the SIM7000A Sheild
}