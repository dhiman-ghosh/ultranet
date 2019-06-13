
void publishAirQuality() {
  int arq = analogRead(A0);
  char cstr[6];
  itoa(arq, cstr, 10);
  Serial.print("Air Quality: ");
  Serial.print(arq);
  Serial.print( "PPM");
  bool ret = mqtt.publish(mqtt_res_channel, cstr, false);
  if (ret) {
    Serial.println(" ... Published!");
  } else {
    Serial.println(" ... NOT Published!");
  }
}
