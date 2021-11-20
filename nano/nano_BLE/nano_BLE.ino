#include <Arduino.h>
#include <ArduinoBLE.h>

void connectToPeripheral();
void controlPeripheral(BLEDevice peripheral);

const char* deviceServiceUuid = "0000FFE0-0000-1000-8000-00805F9B34FB";
const char* device_Name = "DSD TECH";
const char* deviceCustomServiceChar = "0000FFE1-0000-1000-8000-00805F9B34FB";

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial monitor working\n");
  

  if(!BLE.begin()) {
    Serial.println("* Starting BLE Module Failed\n");
    while(1); // just hang if cannot start BLE
  }

  BLE.setLocalName("Nano 33 BLE Central");
  BLE.advertise(); // advertise self
  Serial.println("Arduino Nano 33 BLE Sense (Central Device)");
  Serial.println(" ");
}

void loop() {
  delay(10000); //wait 10 seconds to I can connect to COM
  connectToPeripheral();
}

void connectToPeripheral() {
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");

  do
  {
   // BLE.scanForUuid(deviceServiceUuid);
    BLE.scanForName(device_Name);
    peripheral = BLE.available();
  } while (!peripheral);
  
  if (peripheral) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheral);
  }
  
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  } else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return;
  }

  Serial.println("- Discovering peripheral device attributes...");
  if (peripheral.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");
  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }

  // connect to Custom Service
  BLEService customService = peripheral.service("FFE0");
  if (customService) {
    Serial.println("Custom service discovered!\n");
  } else {
    Serial.println("try another UUID\n");
  }

  // get custom service characteristic
  if (customService.hasCharacteristic("FFE1")) {
    //fix this if statement
  } else {
    Serial.println("Custom char not found, try another UUID!\n");
    return;
  }

  Serial.println("Custom service characteristic found.\n");
  BLECharacteristic customChar = customService.characteristic("FFE1");
  char buff[21];
  buff[20] = '\0';
  while(1) {
  customChar.readValue(buff,20);
  Serial.print("customChar value = ");
  Serial.println(buff);
  Serial.println("\n");
  }

 }
