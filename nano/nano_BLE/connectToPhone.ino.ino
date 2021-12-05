void updateDataLevel() {
  /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the charge level of a battery.
  */
  Serial.print("Sending Data: ");
  unsigned long dummy = 42069 + connectNum;
  Serial.println(dummy);
  String dummy_string = String(dummy);
  int str_len = dummy_string.length();
  byte dummy_arr[str_len];
  const char* dummy_c_str = dummy_string.c_str();
  for(int i = 0; i < str_len; i++){
    dummy_arr[i] = dummy_c_str[i];
  }
  dataChar.writeValue(dummy_arr, str_len);  // and update the battery level characteristic
  connectNum++;
}

void waitForConnection() {
  int updated = 0;
  int waitTime = millis();
  Serial.println("Bluetooth device active, waiting for connections...");// wait for a BLE central
  while (millis() - waitTime < 30000) { //wait a maximum of 30 seconds.
    BLEDevice central = BLE.central();
    // if a central is connected to the peripheral:
    if (central) {
      Serial.print("Connected to central: ");
      //connectNum++;
      // print the central's BT address:
      Serial.println(central.address());
      // turn on the LED to indicate the connection:
      //digitalWrite(LED_BUILTIN, HIGH);
  
      // check the battery level every 200ms
      // while the central is connected:
      while (central.connected()) {
        long currentMillis = millis();
        // if 200ms have passed, check the battery level:
        if (currentMillis - previousMillis >= 200) {
            updateDataLevel();
        }
      }
      // when the central disconnects, turn off the LED:
      //digitalWrite(LED_BUILTIN, LOW);
      Serial.print("Disconnected from central: ");
      Serial.println(central.address());
    }
  }
}
