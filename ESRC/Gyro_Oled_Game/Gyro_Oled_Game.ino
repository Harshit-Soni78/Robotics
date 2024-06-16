#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MPU6050 mpu;
#define CIRCLE_RADIUS 10
int16_t circleX = SCREEN_WIDTH / 2;
int16_t circleY = SCREEN_HEIGHT / 2;
float gyroXSmoothed = 0.0;
float gyroYSmoothed = 0.0;
float smoothingFactor = 0.9;
int16_t gyroThreshold = 100;
bool isGyroActive = false;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (true);
  }
  mpu.initialize();
  display.clearDisplay();
  display.display();
}
void loop() {
  int16_t gyroX, gyroY;
  mpu.getRotation(&gyroX, &gyroY, NULL);
  gyroXSmoothed = smoothingFactor * gyroXSmoothed + (1 - smoothingFactor) * gyroX;
  gyroYSmoothed = smoothingFactor * gyroYSmoothed + (1 - smoothingFactor) * gyroY;
  if (abs(gyroX) > gyroThreshold || abs(gyroY) > gyroThreshold) {
    isGyroActive = true;
    circleX -= gyroXSmoothed / 100; 
    circleY += gyroYSmoothed / 100;
    if (circleX < CIRCLE_RADIUS) {
      circleX = CIRCLE_RADIUS;
    } else if (circleX >= SCREEN_WIDTH - CIRCLE_RADIUS) {
      circleX = SCREEN_WIDTH - CIRCLE_RADIUS - 1;
    }
    if (circleY < CIRCLE_RADIUS) {
      circleY = CIRCLE_RADIUS;
    } else if (circleY >= SCREEN_HEIGHT - CIRCLE_RADIUS) {
      circleY = SCREEN_HEIGHT - CIRCLE_RADIUS - 1;
    }
  } else {
    if (isGyroActive) {
      int16_t lastCircleX = circleX;
      int16_t lastCircleY = circleY;
      circleX = SCREEN_WIDTH / 2;
      circleY = SCREEN_HEIGHT / 2;
      display.clearDisplay();
      display.fillCircle(lastCircleX, lastCircleY, CIRCLE_RADIUS, WHITE);
      display.display();
      isGyroActive = false;
    }
  }
  display.clearDisplay();
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.drawRect(1, 1, SCREEN_WIDTH - 2, SCREEN_HEIGHT - 2, WHITE);
  display.drawRect(2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, WHITE);
  display.fillCircle(circleX, circleY, CIRCLE_RADIUS, WHITE);
  display.display();
}