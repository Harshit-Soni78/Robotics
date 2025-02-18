#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define MPU6050_ADDR 0x68
#define SDA_PIN D2 // Set your SDA pin here
#define SCL_PIN D1 // Set your SCL pin here

WiFiClient client;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  WiFi.begin("Soniji", "i am soni"); // Set your network name and password here

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void loop() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU6050_ADDR, (size_t)6, true);


  int16_t AcX = Wire.read() << 8 | Wire.read();
  int16_t AcY = Wire.read() << 8 | Wire.read();
  int16_t AcZ = Wire.read() << 8 | Wire.read();

  String payload = "AcX=" + String(AcX) + "&AcY=" + String(AcY) + "&AcZ=" + String(AcZ);

  if (client.connect("yourServerIP", 80)) { // Set your server IP and port here
    client.println("POST /update HTTP/1.1");
    client.println("Host: yourServerIP"); // Set your server IP here
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println("Content-Length: " + String(payload.length()));
    client.println();
    client.println(payload);
  }

  delay(200);
}
