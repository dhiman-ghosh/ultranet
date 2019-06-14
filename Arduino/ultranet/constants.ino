/* Common */
extern const char* ssid = "IdIoT-RHGXHRUR";
extern const char* password = "asdfghjkl";
extern const char* mqtt_server = "m24.cloudmqtt.com";
extern const char* mqtt_req_channel = "urequest";
extern const char* mqtt_res_channel = "uresponse";
extern const char* mqtt_user = "rhgxhrur";
extern const char* mqtt_password = "y6DIhw4y0FRk";

/* Air Quality */
extern const uint8_t airq_analog_pin = A0;

/* WS2812B LED Control */
extern const uint8_t ws2812b_data_pin = D4;
extern const uint32_t ws2812b_max_power = 9000; /* mAmp */
extern const uint8_t ws2812b_brightness = 200; /* 0-255 */

/* RGB LED Control */
extern const uint8_t tcs34725_red_pin = D5;
extern const uint8_t tcs34725_green_pin = D6;
extern const uint8_t tcs34725_blue_pin = D7;
