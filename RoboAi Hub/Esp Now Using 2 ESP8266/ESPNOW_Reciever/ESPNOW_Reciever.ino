#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SPI.h>
#include <Wire.h>

String myData = "Harshit Soni";

const int RED = D2;
const int YELLOW = D1;
const int GREEN = D0;
// callback function that will be executed when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  Serial.print("Bytes received: ");
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData);
  Serial.println();
}

void setup() {
  // Init Serial Monitor
  Serial.begin(74880);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
Serial.println(myData);

  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
}


// the loop function runs over and over again forever
void loop() {
  if(myData=="R"){
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);
  }
  else if(myData=="Y"){
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(GREEN, LOW);
  }
  else if(myData=="G"){
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, HIGH);
  }
  else{
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);
  }
}
