#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "cardinal_devices";
const char* password = "gowes!!!";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.print("IP:");
  Serial.println(WiFi.localIP());

  server.on("/water", handleWater);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/water", HTTP_GET, handleWater);
  server.on("/stop", HTTP_GET, handleStop);
  server.on("/status", HTTP_GET, handleStatus);

  server.begin();
  
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "Plant watering ESP8266 is running.");
}

void handleWater() {
  if (!server.hasArg("plant") || !server.hasArg("ml")) {
    server.send(400, "text/plain", "Missing plant or ml.");
    return;
  
  }

  String plant = server.arg("plant");
  String ml = server.arg("ml");

  String command = "WATER:" + plant + ":" + ml;
  Serial.println(command);

  server.send(200, "text/plain", "Sent: " + command);
}

void handleStop() {
  Serial.println("STOP");
  server.send(200, "text/plain", "Sent: STOP");
}

void handleStatus() {
  Serial.println("STATUS");
  server.send(200, "text/plain", "Sent: STATUS");
}

void handleTest() {
  Serial.println("TEST_MESSAGE");
  server.send(200, "text/plain", "Sent TEST_MESSAGE");
}

