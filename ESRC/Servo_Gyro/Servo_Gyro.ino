#include <Wire.h>
#include <Servo.h>
#define MPU6050_ADDRESS 0x68
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_PWR_MGMT_1 0x6B
#define MPU6050_REG_GYRO_CONFIG 0x1B
#define SERVO_PIN 13
int16_t gyroX, gyroY, gyroZ;
float rollAngle;  
Servo myservo;
void setup() {
  Wire.begin();
  Serial.begin(9600);
  
}
void loop() {
  ///////////////
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(MPU6050_REG_PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission(true);
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(MPU6050_REG_GYRO_CONFIG);
  Wire.write(0x08);  
  Wire.endTransmission(true);
  myservo.attach(SERVO_PIN);
  /////////////
  Wire.beginTransmission(MPU6050_ADDRESS);
  Wire.write(MPU6050_REG_ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDRESS, 6, true);
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();
  rollAngle = atan2(gyroY, sqrt(pow(gyroX, 2) + pow(gyroZ, 2))) * 180 / PI;
  int servoPos = map(rollAngle, -90, 90, 0, 180);
  myservo.write(servoPos);
  Serial.println("Roll Angle: ");
  Serial.println(rollAngle);
  Serial.println(" degrees");
  delay(10);
}
