#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

extern const char* ssid;
extern const char* password;
extern const char* mqtt_server;
extern const char* mqtt_req_channel;
extern const char* mqtt_res_channel;
extern const char* mqtt_user;
extern const char* mqtt_password;

// Create an instance of the server
// specify the port to listen on as an argument

WiFiServer server(80);
WiFiClient mqtt_client;
PubSubClient mqtt(mqtt_client);


void setup() {
  WiFiClient client;
  WiFi.persistent(false);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  IPAddress apIP(192, 168, 4, 1);
  
  Serial.println();
  Serial.println("Configuring Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server Started");

  String req, resp;
  char ssid[32], password[32];

  while (true) {
    // Check if a client has connected
    while (!(client = server.available())) {
      delay(1);
    }

    Serial.print("Remote IP: ");
    Serial.println(client.remoteIP());
    Serial.print("Remote Port: ");
    Serial.println(client.remotePort());

    // Wait until the client sends some data
    Serial.println("New Client");
    while(!client.available() && client.status()) {
      delay(500);
    }

    if (!client.status()) {
      continue;
    }
  
    // Read the first line of the request
    req = client.readStringUntil('\r');
    Serial.println(req);
    client.flush();

    if (req == "GET / HTTP/1.1") {
      resp = response_302();
    } else if (req == "GET /auth HTTP/1.1") {
      resp = response("<form><input type='submit' name='continue' /></form>");
    } else {
      resp = response("Request Received");
    }

    client.flush();
    // Send the response to the client
    client.print(resp);
    delay(1);

    if (req.startsWith("GET /config/")) {
      char buf[req.length()+1];
      int count = 0;
      req.toCharArray(buf, sizeof(buf));
      strtok (buf, "/");
      char *pch = strtok(NULL, "/");
      while ((pch = strtok (NULL, " /")) != NULL) {
        count++;
        if (count == 1) {
          strcpy(ssid, pch);
        } else if (count == 2) {
          strcpy(password, pch);
          break;
        }
      }
      break;
    }
  }

  delay(1000);
  server.close();
  WiFi.softAPdisconnect(true);
  Serial.println("Client Disconnected");

  WiFi.mode(WIFI_OFF);
  delay(1000);

  /* Connect to local WiFi Network for Internet */
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print(" : ");
  Serial.print(password);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  mqtt.setServer(mqtt_server, 16448);
  mqtt.setCallback(callback);
  displayLEDPattern(15);
}

String response(String r) {
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  s += r;
  s += "</html>\n";
  return s;
}

String response_302() {
  String s = "HTTP/1.1 302 Found\r\nLocation: http://192.168.1.4/auth\r\n\r\n";
  return s;
}

void loop() {
 if (!mqtt.connected()) {
  reconnect();
 }
 mqtt.loop();
}
