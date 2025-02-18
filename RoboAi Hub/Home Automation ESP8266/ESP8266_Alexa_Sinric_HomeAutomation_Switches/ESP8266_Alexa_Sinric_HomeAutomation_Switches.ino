// Viral Science www.viralsciencecreativity.com www.youtube.com/c/viralscience
// ESP8266 Alexa Home Automation with Switches

#include <Arduino.h>
#include <ESP8266WiFi.h>       // Download & Install ESP8266 Board
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>  // Download & Install https://github.com/Links2004/arduinoWebSockets/releases
#include <ArduinoJson.h>       // Download & Install https://github.com/bblanchon/ArduinoJson/tags
#include <StreamString.h>
#include <AceButton.h>   // Download & Install https://github.com/bxparks/AceButton

using namespace ace_button;
ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
WiFiClient client;

#define MyApiKey "xxxxxxxxxxxxxxxxxxxxxxxx"  // Enter the Sinric API Key
#define MySSID "xxxxxxxx"                    // Enter your WiFi Name
#define MyWifiPassword "xxxxxxxx"            // Enter your WiFi Password

#define HEARTBEAT_INTERVAL 300000 // 5 Minutes 

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

String device_ID_1 = "xxxxxxxxxxxxxxxxxxxxxxxx"; // Enter Sinric Device ID
String device_ID_2 = "xxxxxxxxxxxxxxxxxxxxxxxx"; // Enter Sinric Device ID
String device_ID_3 = "xxxxxxxxxxxxxxxxxxxxxxxx"; // Enter Sinric Device ID
String device_ID_4 = "xxxxxxxxxxxxxxxxxxxxxxxx"; // Enter Sinric Device ID

// define the GPIO connected with Relays and switches

// Relays
#define RelayPin1 D6  //D6
#define RelayPin2 D0  //D0
#define RelayPin3 D2  //D2
#define RelayPin4 D1  //D1

// Switches
#define SwitchPin1 D4  //D4
#define SwitchPin2 D5  //D5
#define SwitchPin3 D7  //D7
#define SwitchPin4 1   //TX

//WiFi Status LED
#define wifiLed    D3   //D3


ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);


void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);

void setPowerStateOnServer(String deviceId, String value);

// deviceId is the ID assgined to your smart-home-device in sinric.com dashboard. Copy it from dashboard and paste it here

void turnOn(String deviceId) {
  if (deviceId == device_ID_1) // Device ID of 1st device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin1, LOW);
  }
  if (deviceId == device_ID_2) // Device ID of 2nd device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin2, LOW);
  }
  if (deviceId == device_ID_3) // Device ID of 3rd device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin3, LOW);
  }
  if (deviceId == device_ID_4) // Device ID of 4th device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin4, LOW);
  }
}

void turnOff(String deviceId) {
  if (deviceId == device_ID_1) // Device ID of 1st device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin1, HIGH);
  }
  if (deviceId == device_ID_2) // Device ID of 2nd device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin2, HIGH);
  }
  if (deviceId == device_ID_3) // Device ID of 3rd device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin3, HIGH);
  }
  if (deviceId == device_ID_4) // Device ID of 4th device
  {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(RelayPin4, HIGH);
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      isConnected = false;
      WiFiMulti.addAP(MySSID, MyWifiPassword);
      Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
      break;
    case WStype_CONNECTED: {
        isConnected = true;
        Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
        Serial.printf("Waiting for commands from sinric.com ...\n");
      }
      break;
      case WStype_TEXT: {
        Serial.printf("[WSc] get text: %s\n", payload);

        #if ARDUINOJSON_VERSION_MAJOR == 5
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.parseObject((char*)payload);
        #endif
        #if ARDUINOJSON_VERSION_MAJOR == 6
                DynamicJsonDocument json(1024);
                deserializeJson(json, (char*) payload);
        #endif
        String deviceId = json ["deviceId"];
        String action = json ["action"];

        if (action == "setPowerState") { // Switch or Light
          String value = json ["value"];
          if (value == "ON") {
            turnOn(deviceId);
          } else {
            turnOff(deviceId);
          }
        }
        else if (action == "test") {
          Serial.println("[WSc] received test command from sinric.com");
        }
      }
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
}

void setup() {
  Serial.begin(9600);

  WiFiMulti.addAP(MySSID, MyWifiPassword);
  Serial.println();
  Serial.print("Connecting to Wifi: ");
  Serial.println(MySSID);

  // Waiting for Wifi connect
  if (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print("Connecting...");
  }
  if (WiFiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);
  
  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);
  
  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
  button3.init(SwitchPin3);
  button4.init(SwitchPin4);

  // server address, port and URL
  webSocket.begin("iot.sinric.com", 80, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", MyApiKey);

  // try again every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000);   // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
}

void loop() {

  if (WiFiMulti.run() != WL_CONNECTED)
  {  
    //  Serial.println("Not Connected");
    digitalWrite(wifiLed, HIGH);
  }
  else
  {
    //Serial.println(" Connected");
    digitalWrite(wifiLed, LOW);
    webSocket.loop();
  }

  button1.check();
  button2.check();
  button3.check();
  button4.check();

  if (isConnected) {
    uint64_t now = millis();

    // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
    if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
      heartbeatTimestamp = now;
      webSocket.sendTXT("H");
    }
  }
}

void setPowerStateOnServer(String deviceId, String value) {
  #if ARDUINOJSON_VERSION_MAJOR == 5
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
  #endif
  #if ARDUINOJSON_VERSION_MAJOR == 6
    DynamicJsonDocument root(1024);
  #endif
  
    root["deviceId"] = deviceId;
    root["action"] = "setPowerState";
    root["value"] = value;
    StreamString databuf;
  #if ARDUINOJSON_VERSION_MAJOR == 5
    root.printTo(databuf);
  #endif
  #if ARDUINOJSON_VERSION_MAJOR == 6
    serializeJson(root, databuf);
  #endif
  webSocket.sendTXT(databuf);
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_1, "ON");
      digitalWrite(RelayPin1, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_1, "OFF");
      digitalWrite(RelayPin1, HIGH);
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_2, "ON");
      digitalWrite(RelayPin2, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_2, "OFF");
      digitalWrite(RelayPin2, HIGH);
      break;
  }
}

void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_3, "ON");
      digitalWrite(RelayPin3, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_3, "OFF");
      digitalWrite(RelayPin3, HIGH);
      break;
  }
}

void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_4, "ON");
      digitalWrite(RelayPin4, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_4, "OFF");
      digitalWrite(RelayPin4, HIGH);
      break;
  }
}
