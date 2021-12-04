float movingAvg(int *ptrArrNumbers, int *ptrSum, int pos, int len, int nextNum)
{
  //Subtract the oldest number from the prev sum, add the new number
  *ptrSum = *ptrSum - ptrArrNumbers[pos] + nextNum;
  //Assign the nextNum to the position in the array
  ptrArrNumbers[pos] = nextNum;
  //return the average
  return (float)*ptrSum / (float)len;
}

void controlPeripheral(BLEDevice peripheral) {
  int RSSI_buf[10] = {0};
  int orientation_buf[10] = {0};
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  } else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return;
  }

  int retries = 0;
  Serial.println("- Discovering peripheral device attributes...");
    while(retries < 6) {
    if (peripheral.discoverAttributes()) {
      Serial.println("* Peripheral device attributes discovered!");
      Serial.println(" ");
      break;
    } else {
      Serial.println("* Peripheral device attributes discovery failed!");
      Serial.println(" ");
      retries ++;
      if (retries > 5) {
        peripheral.disconnect();
        return;
      }
      delay(1000);
    }
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
    Serial.println("Found custom characteristic\n");
  } else {
    Serial.println("Custom char not found, try another UUID!\n");
    return;
  }

  BLECharacteristic customChar = peripheral.characteristic("FFE1");
  // retrieve the simple key characteristic

  // subscribe to the simple key characteristic
  Serial.println("Subscribing to simple key characteristic ...");
  if (!customChar) {
    Serial.println("no custome characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!customChar.canSubscribe()) {
    Serial.println("custom characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!customChar.subscribe()) {
    Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  }

  float RSSI_avg = 0;
  float orient_avg = 0;
  int pos = 0;
  int RSSI_sum = 0;
  int orient_sum = 0;
  int len = 10;
  unsigned long drink_start = 0;
  unsigned long drink_curr = 0;
  while (peripheral.connected()) {
    if (customChar.valueUpdated()) {
      byte buff;
      customChar.readValue(buff);
      Serial.print("Notified read request: ");
      Serial.println((char)buff);
      int p_rssi = peripheral.rssi() * -1;
      Serial.print("RSSI = ");
      Serial.println(p_rssi);

      //implement averaging
      RSSI_avg = movingAvg(RSSI_buf, &RSSI_sum, pos, len, p_rssi);
      orient_avg = movingAvg(orientation_buf, &orient_sum, pos, len, ((int) buff - 48));
      pos++;
      if (pos >= len) {
        pos = 0;
      }
      Serial.print("RSSI Avg:");
      Serial.println(RSSI_avg);
      Serial.print("Orientation Avg:");
      Serial.println(orient_avg);

      if (orient_avg > 0.5 && RSSI_avg < 50) {
          Serial.println("Drinking!");
          if (drink_start == 0) {
            drink_start = millis();
          } else {
            drink_curr = millis();
          }
      } else {
        unsigned long drink_time = drink_curr - drink_start;
        total_time += drink_time;
        drink_curr = 0;
        drink_start = 0;
        Serial.println("Not Drinking");
      }
      
      if (RSSI_avg > 80) {
        peripheral.disconnect();
      }
    }

  }
}

void connectToPeripheral() {
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");
  int waitTime = millis();
  do
  {
   // BLE.scanForUuid(deviceServiceUuid);
    BLE.scanForName(device_Name);
    peripheral = BLE.available();
  } while (!peripheral && (millis() - waitTime < 5000)); //only wait max 5s
  
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
  } else {
    BLE.stopScan();
  }
  
}
