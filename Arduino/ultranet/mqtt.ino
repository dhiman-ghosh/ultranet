#include <WiFiClient.h>
#include <PubSubClient.h>

extern const char* mqtt_server;
extern const char* mqtt_req_channel;
extern const char* mqtt_res_channel;
extern const char* mqtt_user;
extern const char* mqtt_password;

WiFiClient mqtt_client;
extern PubSubClient mqtt(mqtt_client);

void setup_mqtt() {
  mqtt.setServer(mqtt_server, 16448);
  mqtt.setCallback(mqtt_callback);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
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
    air_quality_publish();
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

void mqtt_reconnect() {
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

void loop_mqtt() {
  if (!mqtt.connected()) {
    mqtt_reconnect();
  }
  mqtt.loop();
}
