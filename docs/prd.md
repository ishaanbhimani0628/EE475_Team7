---
layout: page
title: Project Requirements
---

## Project Summary
Our proposed project is a method for tracking the hydration of animals. Dehydration in animals can lead to many health problems. This can be especially costly for livestock. The proposed solution would use sensors for tracking proximity to water sources and orientation of the head, to confirm the animal is drinking.  
You can find the evolving document with component links [here](https://docs.google.com/document/d/1S5zivW_AB6peCOJfSw9JGnZi5F4OPk7b4tQhdf4p1Rw/edit?usp=sharing).

## Deliverables
- Proximity tracking
- Orientation sensor tracking
- Neck size sensor tracking
- Communication to app
- Tested and demoed without animals
- Tested and demoed with real animals 

## Critical Features
- Low power collar
- Solar charged base station
- Tracks each collar: time spent drinking (with head bowed) at water station
- Bluetooth communication with mobile app

## Performance Metrics
- Measuring success and speed of bluetooth connection of collar and base station when animal collar is within range/near the water source
- Time animal has actually spent drinking water vs time calculated 
- Head tilt- success rate of ball tilt sensor being triggered when the animal is drinking water

## Milestones
1. Establish bluetooth communication between MSP EZ430 with BLE module and Adafruit Feather nRF52840
2. RSSI implementation/access on Adafruit Feather nRF52840
3. Working accelerometer readings on MSP EZ430
4. Power calculations for collar (MSP EZ430, BLE Module, Accelerometer)
5. Select a power scheme for collar
6. Form factor/pcb development for collar if necessary
7. Establish bluetooth communication between Adafruit Feather and mobile device/app
8. Writability to base station memory
9. Select power scheme for base station
10. Data representation on mobile app
11. Implement features on app (reach)


## Responsibilities of Each Team Member
Website:
Weekly updates by each team member regarding what they worked on that week. Assignments that are posted to the website will be completed together.

App:
App development will be split amongst team members. 

Collar:
Bluetooth integration with the collar will be completed together. Each team member will complete one of accelerometer, ball tilt switch, and flex sensor integration with the collar. 

Base Station:
Bluetooth communication with the app and collar will be worked on together. 

## Materials & Outside Help Needed
- Microcontrollers
- Adafruit Feather nRF52840 (Base station)
- TI MSP EZ430 (Collar)
- Sensors
- Accelerometer
- Ball tilt switch ($2)
- Flex Sensor/Force Sensitive Resistor
- BTLE for Collar
- On-board memory for base station
- Mobile/Web App
- Power
- Solar panel
- Batteries 
- Coin cell or AA for collar
- Rechargable (lithium-ion batteries) for base station


## Budget
- Adafruit Feather nRF52840 - $24.95
- TI MSP EZ430 - $31.92 
- Ball tilt switch x2 - $4.00
- Force Sensitive Resistor x 2 - $39.90
- HM-10/AT-09 BLE Module x 2 - $10.50
- Low-power accelerometer x 1 - $16.95
- Battery connector for collar (AA or coin cell) - TBD based on power scheme
- Solar Panel - TBD based on power scheme
- Additional storage for Adafruit Feather - TBD based on memory scheme

## Risks
- Unable to determine distance over BLE
- Confusing grazing/lowered head with drinking
- Bluetooth takes too long to connect
- Power consumption too high on base station and collar
- Data communication rate may be too low
- 
## How Risks Will Be Addressed
- Assume BLE connection close enough or implement RFID
- Observe animals drinking water and collect data on their movements to determine the difference between grazing and drinking.
- Experiment with connection times to determine how risky this is. Assume animals drink for a sufficient amount of time.
- If power consumption is too high at the base station we can add more batteries/increase solar panels since size is less of an issue in this case. For decreasing power consumption of the collar, we can tune the interrupts from the ball tilt sensor to better recognize lowering head for longer periods to drink vs moving head around. 
- Storing data on collar and sending as it is able to, and use simple data (true or false over BLE) instead of sending other info. Do all processing on the base station or app.
