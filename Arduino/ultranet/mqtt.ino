void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char cstr_cmd[length + 1];
  memset(cstr_cmd, 0, length);
  strncpy(cstr_cmd, (char*)payload, length);
  cstr_cmd[length] = '\0';
  String command(cstr_cmd);

  if (String(topic) != mqtt_req_channel) {
    Serial.println("ERR: Unknown message received. Discarded.");
    return;
  }
  
  Serial.print("Command: ");
  Serial.println(command);
  if (command == "arq") {
    Serial.println("Processing Air Quality Request...");
    publishAirQuality();
  } else if (command.startsWith("ledstrip")){
    char pattern[32];
    int pattern_enum = 0;
    int count = 0;
    char buf[command.length() +1];
    command.toCharArray(buf, sizeof(buf));
    char *pch = strtok(buf, " ");
    while ((pch = strtok (NULL, " ")) != NULL) {
      count++;
      if (count == 1) {
        strcpy(pattern, pch);
        Serial.print("pch: ");
        Serial.println(pattern);
        pattern_enum = atoi(pattern);
        Serial.print("pattern_enum: ");
        Serial.println(pattern_enum);
        displayLEDPattern(pattern_enum);
      }
    }
  } else {
    Serial.println("Not Supported!");
  }
}

void reconnect() {
 // Loop until we're reconnected
 while (!mqtt.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (mqtt.connect("ESP8266 Client", mqtt_user, mqtt_password)) {
  Serial.println("connected");
  // ... and subscribe to topic
  mqtt.subscribe(mqtt_req_channel);
 } else {
  Serial.print("failed, rc=");
  Serial.print(mqtt.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}
