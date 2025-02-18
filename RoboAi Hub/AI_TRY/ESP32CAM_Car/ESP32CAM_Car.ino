#include "esp_camera.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>  // Include the ESPAsyncWebServer library

AsyncWebServer server(80);  // Create an instance of the server


#define CAMERA_MODEL_AI_THINKER

const char* ssid = "Soniji";   //Enter SSID WIFI Name
const char* password = "i am soni";   //Enter WIFI Password

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22


// GPIO Setting
extern int gpLb =  2; // Left 1
extern int gpLf = 14; // Left 2
extern int gpRb = 15; // Right 1
extern int gpRf = 13; // Right 2
extern int gpLed =  4; // Light
extern String WiFiAddr ="";

// Motor control function
void controlMotor(String direction) {
  if (direction == "forward") {
    digitalWrite(gpLf, HIGH);
    digitalWrite(gpLb, LOW);
    digitalWrite(gpRf, HIGH);
    digitalWrite(gpRb, LOW);
  } else if (direction == "backward") {
    digitalWrite(gpLf, LOW);
    digitalWrite(gpLb, HIGH);
    digitalWrite(gpRf, LOW);
    digitalWrite(gpRb, HIGH);
  } else if (direction == "left") {
    digitalWrite(gpLf, LOW);
    digitalWrite(gpLb, HIGH);
    digitalWrite(gpRf, HIGH);
    digitalWrite(gpRb, LOW);
  } else if (direction == "right") {
    digitalWrite(gpLf, HIGH);
    digitalWrite(gpLb, LOW);
    digitalWrite(gpRf, LOW);
    digitalWrite(gpRb, HIGH);
  } else {
    // Stop
    digitalWrite(gpLf, LOW);
    digitalWrite(gpLb, LOW);
    digitalWrite(gpRf, LOW);
    digitalWrite(gpRb, LOW);
  }
}

void startCameraServer() {
  // ... existing camera server setup code ...

  server.on("/forward", HTTP_GET, ={
    controlMotor("forward");
    request->send(200, "text/plain", "Car is moving forward");
  });

  server.on("/backward", HTTP_GET, ={
    controlMotor("backward");
    request->send(200, "text/plain", "Car is moving backward");
  });

  server.on("/left", HTTP_GET, ={
    controlMotor("left");
    request->send(200, "text/plain", "Car is turning left");
  });

  server.on("/right", HTTP_GET, ={
    controlMotor("right");
    request->send(200, "text/plain", "Car is turning right");
  });

  server.on("/stop", HTTP_GET, ={
    controlMotor("stop");
    request->send(200, "text/plain", "Car has stopped");
  });

  // ... existing camera server setup code ...
}


void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();


  pinMode(gpLb, OUTPUT); //Left Backward
  pinMode(gpLf, OUTPUT); //Left Forward
  pinMode(gpRb, OUTPUT); //Right Forward
  pinMode(gpRf, OUTPUT); //Right Backward
  pinMode(gpLed, OUTPUT); //Light

  //initialize
  digitalWrite(gpLb, LOW);
  digitalWrite(gpLf, LOW);
  digitalWrite(gpRb, LOW);
  digitalWrite(gpRf, LOW);
  digitalWrite(gpLed, LOW);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  //drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();
  server.begin();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  WiFiAddr = WiFi.localIP().toString();
  Serial.println("' to connect");
}

void loop() {

}
