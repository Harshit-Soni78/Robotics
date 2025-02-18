#include <Wire.h>
#include <MPU6050.h>


#define MPU6050_ADDR 0x68
#define SDA_PIN D2 // Set your SDA pin here
#define SCL_PIN D1 // Set your SCL pin here

void setup() {
  Serial.begin(38400);
  
  Wire.begin(SDA_PIN, SCL_PIN);
}

void loop() {
  
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU6050_ADDR, (size_t)6, true);


  int16_t AcX = Wire.read() << 8 | Wire.read();
  int16_t AcY = Wire.read() << 8 | Wire.read();
  int16_t AcZ = Wire.read() << 8 | Wire.read();

  String payload = "AcX= " + String(AcX) + " &AcY= " + String(AcY) + " &AcZ= " + String(AcZ);
  Serial.println(payload);
  delay(1000);
}
