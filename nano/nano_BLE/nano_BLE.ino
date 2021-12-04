#include <Arduino.h>
#include <ArduinoBLE.h>
//TODO: moving average for readings and RSSI

int movingAvg(int *ptrArrNumbers, int *ptrSum, int pos, int len, int nextNum);
void connectToPeripheral();
void controlPeripheral(BLEDevice peripheral);
void waitForConnection();
void updateDataLevel();

const char* deviceServiceUuid = "0000FFE0-0000-1000-8000-00805F9B34FB";
const char* device_Name = "DSD TECH";
const char* deviceCustomServiceChar = "0000FFE1-0000-1000-8000-00805F9B34FB";

// Service info for connecting to phone
unsigned long total_time = 0;  // test data to send to phone
unsigned long previousMillis = 0;  // last time the battery level was checked, in ms
BLEService dataService("1101");
BLEUnsignedCharCharacteristic dataChar("2101",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial monitor working\n");
  

  if(!BLE.begin()) {
    Serial.println("* Starting BLE Module Failed\n");
    while(1); // just hang if cannot start BLE
  }

  BLE.setLocalName("Nano 33 BLE"); 

  // set characteristic info for connecting to phone
  BLE.setAdvertisedService(dataService); // add the service UUID
  dataService.addCharacteristic(dataChar); // add the battery level characteristic
  BLE.addService(dataService); // Add the battery service
  dataChar.writeValue(total_time); // set initial value for this characteristic
  
  BLE.advertise(); // advertise self
  Serial.println("Arduino Nano 33 BLE Sense (Central Device)");
  Serial.println(" ");
}

void loop() {
  delay(10000); //wait 10 seconds so I can connect to COM
  connectToPeripheral();
  waitForConnection();
}
