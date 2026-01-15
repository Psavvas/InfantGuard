# 🛡️InfantGuard
Infant Guard is a fully integrated, Arduino-driven safety platform engineered to detect forgotten infants in vehicles and escalate alerts before temperatures reach life-threatening levels. This solution is purpose-built to address Pediatric Vehicular Heatstroke (PVH) by combining proven hardware, streamlined logic flows, and a resilient communication stack.
## 📚 Background
Forgotten Baby Syndrome (FSD) is a primary driver of Pediatric Vehicular Heatstroke (PVH) deaths and injuries. Every year infants across the US die due to overheating in a vehicle after being left unsupervised. Since 1998 over 1000 infants have lost their lives due to PVH. That averages to about ~37 deaths per year. In most of these cases, guardians simply forgot their infants were still in the overheating vehicle. Since vehicles can heat up to 120F in as little as 30 minutes, being able to take action before it's too late becomes crucial. Infant deaths can occur as early as 109F. InfantGuard attempts to resolve this issue by creating a reliable and long-lasting monitoring system that detects a forgotten baby and notifies the parent. If temperatures reach deadly levels InfantGuard can automatically notify emergency services with the vehicles details, temperature and location.
## ✨ Features
- ***Use of Bluetooth Fob:*** InfantGuard uses a Bluetooth device, in this case a BLE iBeacon attached to the car keys, to detect the parent's presence in the vehicle. This allows for Arduino to better know the current scenario and take more appropriate actions.
- ***Text Notifications about forgotten infant:*** InfantGuard uses text notifications to alert the user if their infant is left unsupervised in the vehicle. If temperature begin nearing dangerous levels, InfantGuard will text the parent in a dedicated alert, making sure to give the vehicles current temperature. If temperatures reach deadly lives the user will be sent an emergency text, notifying them of the situation and that emergency services have been contacted. In all these texts the vehicles location is always attached.
- ***Emergency Services Alert:*** If vehicle temperatures reach deadly levels, InfantGuard will automatically text emergency services, including the vehicle make and model, temperature and location. This is to allow for a quick response that could help save the infant's life.
- ***Manual Reset:*** If for some reason InfantGuard needs to be manually reset, a button present in the vehicle is available. Once pressed the system will stop monitoring for 15 minutes, before beginning scanning again.
- ***Status Notifications:*** InfantGuard provides the parent with constant status notifications. When the system is manually deactivated, the parent will receive a confirmation text, before receiving another one when the system reactivates. Furthermore the parent can also be notified with other texts, like when the system first boots up and is connected.
- ***Long Lasting Battery Life:*** InfantGuard is designed to have a long lasting battery, so that it can continue monitoring even if the vehicle is turned off. The current 10000mAh Belkin Battery Pack allows for roughly 80 hours of straight use. However the battery pack can recharge in roughly 4 hrs., so InfantGuard has been designed to recharge when the vehicle is turned on, and use battery when the vehicle is turned off.

## 🪛 Hardware
InfantGuard's hardware and software provides reliable and long-lasting vehicle monitoring, ready to take action if necessary.
### 🌡️ Sensors
- 2 Force Sensing Resistors are attached to key location's were the child's back will be located. This insures accurate detection of the infant at all times.
- A DHT-22 Temperature sensor allows for accurate temperature detection which can trigger InfantGuard to take necessary actions to protect the infant.
- An HM-10 Bluetooth Receiver scans for the desired device/devices that are used to consider the parent present. The HM-10 uses RSSI signal strengthen to determine whether the parent is present with infant or too far away, which could require attention.
- An Arduino Push Button also allows for the system to be manually deactivated for 15 minutes at a time. During this period the Arduino will stop scanning and monitoring the vehicle.
### 📶 Connectivity
InfantGuard is powered by a Botletics SIM7000A shield. This allows for the Arduino to communicate with the internet, allowing for texts to be remotely send via Twilio's API. The SIM7000A is powered by Holograms IoT SIM Card, allowing for cheap and reliable connection no-matter the vehicles location. Furthermore the SIM7000A also acts as the GPS of the system, allowing InfantGuard to easily be able to record the vehicles location.
### 💽 Microcontroller
InfantGuard is powered by an Arduino Mega 2560, to allow for multiple active Hardware Serial ports. This allows for both the HM-10, SIM7000A to actively communicate and scan. Furthermore if the Arduino is plugged into a computer for debugging, the Serial Monitor can also be used simultaneously.
### 📦 Exterior and FSR Docking
InfantGuard is conveniently placed into a 3D printed case, that allows for all wires, microcontrollers and sensors to conveniently be hidden. Furthermore the box allows for a mounting location for the push button, temperature sensor and HM-10, so that they can get more accurate readings.

To allow for the car seat to easily be removed two docking components, 1 for each FSR sensor were created. A universal male and female component were created. This allows for two wire connectors to be slide in and for them to automatically connect, thanks to a funnel system when the car seat is reattached to its base. The universal male and female components can be modified with specific mounting brackets to fit the car's requirements. In this prototype, a more specific variant was created so that the two components could be mounted at the rear of the seat.

Both of these files can be found as STL files on Thingiverse at the following link: [https://www.thingiverse.com/psavvas/collections/43677561/things](https://www.thingiverse.com/psavvas/collections/43677561/things)
<p align="center">
  <img src="https://github.com/user-attachments/assets/edfd0030-a746-4da9-93bb-535f2fd2935c" alt="InfantGuard Mountable FSR Docking Components 1" style="width:320px; margin-right:12px;">
  <img src="https://github.com/user-attachments/assets/33637767-4be1-48d4-a11b-8721f7af63bf" alt="InfantGuard Mountable FSR Docking Components 2" style="width:320px;">
</p>
<p align="center"><strong>Mountable InfantGuard FSR Docking Components</strong></p>
<p align="center">
  <img src="https://github.com/user-attachments/assets/3b0f6c85-e666-4a14-8905-3b7371bfaaf1" alt="InfantGuard Generic FSR Docking Components 1" style="width:320px; margin-right:12px;">
  <img src="https://github.com/user-attachments/assets/80aefcca-c5c2-435d-acca-245b43f3f05a" alt="InfantGuard Generic FSR Docking Components 2" style="width:320px;">
</p>
<p align="center"><strong>Generic InfantGuard FSR Docking Components</strong></p>

### 🧵Wiring
InfantGuard's wiring using generic Dupont Wires that can be either used on a breadboard and the Arduino or soldered onto a circuit if preferred.

<img width="1280" height="720" alt="InfantGuard Circuit Schematic" src="https://github.com/user-attachments/assets/470edf90-954b-477a-a258-aee44d707836" />
<p align="center"><strong>InfantGuard Circuit Schematic</strong></p>

## 🧑‍💻 Software
InfantGuard is an open source project on GitHub. All coding files are publicly available on GitHub to learn more about the contributing visit the *Learn More and Contribute* section of this README. This project is made up of two major sections the on-device code and the cloud code. These were coded in Arduino Sketch and JavaScript respectively.
### 🔍 General Overview
The following flow chart exhibits the general flow of InfantGuard across both on-device programs and cloud programs:
<img width="1280" height="720" alt="InfantGuard General Code Flow" src="https://github.com/user-attachments/assets/9f1b2c82-039d-49e1-8833-9332df4062f9" />
<p align="center"><strong>InfantGuard General Code Flow</strong></p>

### 📲 Text Notification Workflow
1. SIM7000A establishes connection with local cellular towers via a Hologram SIM.
2. The SIM7000A then establishes a connection with a specific webhook.site URL that include all the required information including vehicle location and temperature
3. webhook.site then redirects the data received to a secure Cloudflare Worker.
4. The Cloudflare Worker parses the data received and determines which message to send.
5. Cloudflare fills the chosen message template with received variables before sending it to Twilio's API
6. Twilio then take charge of the SMS delivery to the user's phone.
### ⌨️ On-Device Code
On-device code of InfantGuard, being coded in majority of Arduino Sketch has the majority of its code repeating itself in `void loop()`. The `void setup()` function is responsible for booting up the system, activating the sensors, establishing cellular and GPS connection as well as enabling the Bluetooth scanning. Inside `void loop()`, the program will always begin by checking the FSR sensor for an infant. If an infant is present then by precaution InfantGuard will record GPS location. InfantGuard will then scan for nearby Bluetooth devices using the MAC Address of the parent device and the RSSI signal strength value. InfantGuard uses this information to check whether the parent is present or not and also checks the temperature. Depending on these results, InfantGuard will determine whether it needs to send a text, and whether the text cooldown period has elapsed. If so it uses it cellular capabilities to communicate with the webhook.site

### ☁️ Off-Device Code
***Webhook.site:***
Since the SIM7000A can only establish an HTTP connection, websites that don't enforce as a secure connection were required. A custom webhook.site was then created which allowed for the Arduino to communicate with the webhook, have the webhook parse the data, and create a custom action that redirects the information to a secure Cloudflare Worker. The webhook uses an HTTP redirect action that forwards to the Cloudflare Worker. The action uses a 'GET' method with the following programmable URL: 
```
https://workerURLgoeshere.workers.dev/?cord1=$request.query.cord1$&cord2=$request.query.cord2$&temp=$request.query.temp$&type=$request.query.type$
```
***Cloudflare Worker:***
Once webhook.site relays the information to the Cloudflare Worker, it parses the URL data and determines which text message template to send. It then fills out the variables in the message template like temperature or vehicle location and securely communicates with the Twilio API. Twilio is the text message provider that was chosen to securely provide text notifications to the user's phone. The full Cloudflare worker code can be found in the file: `Cloudflare.js` This workflow also helps protect the API keys which can be stored as secret variables inside the Cloudflare Worker dashboard.
## 🪛 Full Parts List
- Arduino Mega 2560
- FSR Force Resistance Sensor
- Temperature Sensor
- HM-10 Bluetooth Module
- iBeacon Transmitter
- Breadboard
- Dupont Wires
- 3.7V LiPo Battery
- 10000 mAh Belkin Battery Pack
- PLA Filament
- 10K Ω & 2K Ω Resistor
- Type C to USB D Adapter
- Push Button
- Circuit Board
- Circuit Board Mounts
- Male & Female 12v DC Power Jack Adapter Connector
- Screws & Nuts
- PLA Filament

## 🌐 Learn More & Contribute
To learn more about InfantGuard and this life-saving opensource project visit: www.paulsavvas.me/infantguard

If you're interested in contributing feel free to fork the repository and submit a Pull Request. InfantGuard is designed to be an open-source, life-saving project.


*InfantGuard was created as a 2025 Engineering Fair Project for the Great Mills High School STEM Academy and the St. Mary's County Science & Engineering Fair. It was created by Paul Savvas.*
