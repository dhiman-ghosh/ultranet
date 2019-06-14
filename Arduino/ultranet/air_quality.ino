#include <PubSubClient.h>

extern PubSubClient mqtt;
extern const char* mqtt_res_channel;
extern const uint8_t airq_analog_pin;

void air_quality_publish() {
  int arq = analogRead(airq_analog_pin);
  char cstr[6];
  itoa(arq, cstr, 10);
  Serial.print("Air Quality: ");
  Serial.print(arq);
  Serial.print( "PPM");
  bool ret = mqtt.publish(mqtt_res_channel, cstr, false);
  if (ret) {
    Serial.println(" ... ARQ Published!");
  } else {
    Serial.println(" ... ARQ NOT Published!");
  }
}
