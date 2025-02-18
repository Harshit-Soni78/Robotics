#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define IN1 D1 // Set your IN1 pin here
#define IN2 D2 // Set your IN2 pin here
#define IN3 D3 // Set your IN3 pin here
#define IN4 D4 // Set your IN4 pin here

ESP8266WebServer server(80);

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  WiFi.begin("Soniji", "i am soni"); // Set your network name and password here

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  server.on("/update", handleUpdate);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleUpdate() {
  int16_t AcX = server.arg("AcX").toInt();
  int16_t AcY = server.arg("AcY").toInt();
  int16_t AcZ = server.arg("AcZ").toInt();
  Serial.print(AcX);
  if (AcX > 10000) {
    // Move forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (AcX < -10000) {
    // Move backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else if (AcY > 10000) {
    // Turn right
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (AcY < -10000) {
    // Turn left
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    // Stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  server.send(200);
}
