#include <ESP8266WiFi.h>
#include <espnow.h>


//These are needed for MPU
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


// MPU control/status vars
MPU6050 mpu;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


// REPLACE WITH YOUR RECEIVER MAC Address
// uint8_t receiverMacAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t receiverMacAddress[] = { 0x40,0x91,0x51,0x56,0x10,0xD0 };


struct PacketData 
{
  byte xAxisValue;
  byte yAxisValue;
  byte zAxisValue;  
};
PacketData data;

// String myData = "Test";


// Callback function called when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t status) {
  Serial.println(status == status ? "Message sent" : "Message failed");
}

void setupMPU()
{
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) 
  {
      // Calibration Time: generate offsets and calibrate our MPU6050
      mpu.CalibrateAccel(6);
      mpu.CalibrateGyro(6);
      mpu.setDMPEnabled(true);
      dmpReady = true;
  } 
}

void setup() {
  // Init Serial Monitor
  Serial.begin(74880);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("Initialized ESP-NOW");
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);


  // Add peer
  if (esp_now_add_peer(receiverMacAddress, ESP_NOW_ROLE_SLAVE, 0, NULL, 0) != 0) {
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("Added peer");
  }

  if (esp_now_is_peer_exist(receiverMacAddress)) {
    Serial.println("Peer exists");
  } else
    Serial.println("No exists");
    
  //This is to set up MPU6050 sensor
  setupMPU();  
}

void loop() {
  // if (Serial.available() > 0) {
  //   myData = Serial.readString();  //read until timeout
  //   myData.trim();                 // remove any \r \n whitespace at the end of the String
  //   Serial.println(myData);

  //   // Send message via ESP-NOW
  //   int result = esp_now_send(receiverMacAddress, (uint8_t *)&myData, sizeof(myData));

  //   if (result == 0) {
  //     Serial.println("Sent with success");
  //   } else {
  //     Serial.println("Error sending the data");
  //   }
  // }

  // delay(1000);

  // if programming failed, don't try to do anything
  if (!dmpReady) return;
  // read a packet from FIFO. Get the Latest packet
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) 
  {  
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    int xAxisValue = constrain(ypr[2] * 180/M_PI, -90, 90);
    int yAxisValue = constrain(ypr[1] * 180/M_PI, -90, 90);
    int zAxisValue = constrain(ypr[0] * 180/M_PI, -90, 90);    
    data.xAxisValue = map(xAxisValue, -90, 90, 0, 254); 
    data.yAxisValue = map(yAxisValue, -90, 90, 0, 254);
    data.zAxisValue = map(zAxisValue, -90, 90, 0, 254); 

    // esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *) &data, sizeof(data));
    uint8_t result = esp_now_send(receiverMacAddress, (uint8_t *) &data, sizeof(data));

    String inputData  = inputData + "values " + xAxisValue + "  " + yAxisValue + "  " + zAxisValue;
    // Serial.println(inputData);
    delay(50);            
  }
}
