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

To allow for the car seat to easily be removed two docking components, 1 for each FSR sensor were created. A universal male and female component were created. This allows for two wire connectors to be slide in and for them to automatically connect, thanks to a funnel system when the car seat is reattached to its base. The universal male and female components can be modified with specific mounting brackets to fit the car's requirements. In this prototype, a more specifc varraint was created so that the two componetns could be moutned at the rear of the seat.
### Add image

## 🧑‍💻 Software
InfantGuard is an open source project on GitHub. All coding files are publicly available on GitHub to learn more about the contributing visit the *Learn More and Contribute* section of this README. This project is made up of two major sections the on-device code and the cloud code. These were coded in Arduino Sketch and JavaScript respectively.
### 🔍 General Overview

### 📲 Text Notification Setup

## 🪛 Full Parts List
- List all the parts
- List all the parts
- List all the parts
- List all the parts
- List all the parts
- List all the parts
- List all the parts
- List all the parts
- List all the parts
- List all the parts
- List all the parts
- List all the parts
## 🌐 Learn More & Contribute
To learn more about InfantGuard and this life-saving opensource project visit: www.paulsavvas.me/infantguard
If you're interested in contributing feel free to fork the repository and submit a Pull Request. InfantGuard is designed to be an open-source, life-saving project.


*InfantGuard was created as a 2025 Engineering Fair Project for the Great Mills High School STEM Academy and the St. Mary's County Science & Engineering Fair. It was created by Paul Savvas.*
