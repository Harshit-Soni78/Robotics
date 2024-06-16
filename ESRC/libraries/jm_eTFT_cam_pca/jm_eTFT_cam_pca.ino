        // TFT_CS GPIO12
        // TFT_DIN GPIO15
        // TFT_CLK GPIO2
        // TFT_DC GPIO01 TXD0
        // PCA_OE GPIO03 RXD0

        // FLASH   GPIO4  [Solder Pad]

        // I2C_SDA GPIO13
        // I2C-SCL GPIO14

        // // You can connect a push button here to make OTA easy
        // PSRAM_CS GPIO16 [Solder Pad]
        // CSI_MCLK_XCLK GPIO0 [Solder Pad]


#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#include <Adafruit_PWMServoDriver.h>
#include <ESP32WebServer.h>    // https://github.com/Pedroalbuquerque/ESP32WebServer download and place in your Libraries folder
#include <ESPmDNS.h>
#include <FS.h>
//#include <SD.h>
#include <SPI.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>       // Hardware-specific library
#include <Update.h>
//#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiMulti.h> 
#include <Wire.h>

//#include "file_services.c"
//#include <stdlib.h>
#include <stdint.h>

#include "camera_pins.h"
#include "CSS.h"
#include "esp_camera.h"
#include "soft_serial_tx_util.c"
#include "web_page.h"



#define TEXT_SIZE   2 // Avoid changing. Try, NOT to change

// #define SERVOMIN   150 // This is the 'minimum' pulse length count (out of 4096)
// #define SERVOMAX   600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN      600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX     2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ  50 // Analog servos run at ~50 Hz updates

uint8_t servonum = 0;

#define SDA_PIN                    13  // Fixed for TL_RAN PCB
#define SCL_PIN                    14  // Fixed for TL_RAN PCB
#define PCA_ENABLE_PIN_NO           3  //  Fixed for TL_RAN PCB
#define LED_RED_INBUILD_PIN_NO     33  //  Fixed for ESP32-CAM 

#define PWM_CHANNEL_NO_EM     7  // On PCA PCB
#define PWM_CHANNEL_NO_LASER  6  // On PCA PCB

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // Default I2C address 0x40
//WebServer server(80);
ESP32WebServer  server(80);

const char* host = "esp32";
const char* ssid = "myPhone";
const char* password = "test1234";

bool    SPIFFS_present = false;
bool    SD_present = false; // DO not change, as no SD card in robot


int channel = 0;
int value = 0;
int position_uS;
long count = 0; // Loop count
int lcd_row;
const int lcd_row_incrementer = 20;

String  webpage = "";


#define   servername "fileserver"  // Set your server's logical name here e.g. if 'myserver' then address is http://myserver.local/
IPAddress local_IP(192, 168, 0, 150); // Set your server's fixed IP address here
IPAddress gateway(192, 168, 0, 1);    // Set your network Gateway usually your Router base address
IPAddress subnet(255, 255, 255, 0);   // Set your network sub-network mask here
IPAddress dns(192,168,0,1);           // Set your network DNS usually your Router base address


//#define EM_ON  0
//#define EM_OFF 1

#define LED_INBUILD_ON  0
#define LED_INBUILD_OFF 1

void hang(void);
char buffer_temp[30];
   
void setup()
{
  ss_printf("\r\n\r\n =================  RESET ESP32-CAM-CH340  =================\r\n");

  pinMode(PCA_ENABLE_PIN_NO, OUTPUT);       digitalWrite(PCA_ENABLE_PIN_NO, HIGH); // Disable PCA module
  pinMode(LED_RED_INBUILD_PIN_NO, OUTPUT);  digitalWrite(LED_RED_INBUILD_PIN_NO, LED_INBUILD_OFF); 
  pinMode(4, OUTPUT);                       digitalWrite(4, LOW); 

  #define PRODUCER 13
  #define CONSUMER 0

  pinMode(PRODUCER, OUTPUT);   digitalWrite(PRODUCER, LOW); 
  pinMode(CONSUMER, INPUT);                        
    
  ss_puts("\r\n Resolution in UserSteup file 320 x 240");
  tft.begin(); // Resolution in UserSteup file 320 x 240
  tft.setRotation(0);	// 0 & 2 Portrait. 1 & 3 landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(TEXT_SIZE);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //  text, background
  tft.drawString("ESP32-CAM ST7789V", 0, lcd_row); lcd_row += lcd_row_incrementer;
  
  tft.setTextColor(TFT_WHITE, TFT_RED); //  text, background
  tft.drawString(" R E S E T", 0, lcd_row); lcd_row += lcd_row_incrementer;
  
  
  
  if (!SPIFFS.begin(true)) {
    ss_puts("\r\n SPIFFS initialisation failed...");
    tft.setTextColor(TFT_RED, TFT_BLACK); //  text, background
    tft.drawString("No SPIFFS", 0, lcd_row);  lcd_row += lcd_row_incrementer;
    SPIFFS_present = false;
  } else {
    ss_puts("\r\n SPIFFS initialised... file access enabled...");
    tft.setTextColor(TFT_GREEN, TFT_BLACK); //  text, background
    tft.drawString("SPIFFS: Okay", 0, lcd_row);  lcd_row += lcd_row_incrementer;
    SPIFFS_present = true;
  }
                
  for(count=100; count >=1; count--)
  {
      tft.setTextColor(TFT_YELLOW, TFT_BLACK); //  text, background
      tft.drawString("Press WiFi Key: ", 0, lcd_row);

      tft.setTextColor(TFT_ORANGE, TFT_BLACK); //  text, background
      tft.drawNumber(count/100,     200, lcd_row);

    if(digitalRead(CONSUMER) == LOW) // Switch Pressed for WiFi / Web-Browser mode
    {
      tft.fillScreen(TFT_BLACK);
      lcd_row = 0;
      tft.setTextColor(TFT_WHITE, TFT_BLUE); //  text, background
      tft.drawString("WiFi Browser Mode", 0, lcd_row);  lcd_row += lcd_row_incrementer;
      ss_puts("\r\n Key pressed: Now in WiFi / Browser Mode");
      digitalWrite(4, HIGH); delay(2); digitalWrite(4, LOW);
        // Connect to WiFi network
  WiFi.begin(ssid, password);
  ss_puts("\r\n");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    ss_putc('.');
  }

  IPAddress ip = WiFi.localIP();
  String ipString = ip.toString();
  
  ss_printf("\r\n Connected to: %s", ssid);
  ss_printf("\r\n IP address: %s",  ipString);
  ss_printf("\r\nhttp://%s\r\n",  ipString);
  
  // tft.setTextSize(TEXT_SIZE);
  tft.setTextColor(TFT_BLUE, TFT_BLACK); //  text, background
  tft.drawString("SSD: ", 0, lcd_row);  tft.drawString(ssid, 80, lcd_row);           lcd_row += lcd_row_incrementer;
  tft.drawString("IP: ", 0, lcd_row);   lcd_row += lcd_row_incrementer;
  
  tft.setTextColor(TFT_SKYBLUE, TFT_BLACK); //  text, background
  tft.setTextSize(TEXT_SIZE+1);
  tft.drawString(ipString, 0, lcd_row); lcd_row = lcd_row + ( lcd_row_incrementer + lcd_row_incrementer/2);
  tft.setTextSize(TEXT_SIZE);
  
  tft.setTextColor(TFT_BLUE, TFT_BLACK); //  text, background
  tft.drawString("Open Browser", 0, lcd_row);   lcd_row += lcd_row_incrementer;
  sprintf(buffer_temp, "http://%s", ipString);

  tft.drawString(buffer_temp, 0, lcd_row);  lcd_row += lcd_row_incrementer;

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
   ss_puts("\r\n Error setting up MDNS responder!");
   tft.setTextColor(TFT_RED, TFT_BLACK); //  text, background
   tft.drawString("ERROR: MDNS", 0, lcd_row);   lcd_row += lcd_row_incrementer;
   //hang();
  }
  else
    {
      ss_puts("\r\n mDNS responder started");
      tft.drawString("MDNS: Okay", 0, lcd_row);   lcd_row += lcd_row_incrementer;
    }
    
  /*return index page which is stored in serverIndex */
  // server.on("/", HTTP_GET, []() {
  //   server.sendHeader("Connection", "close");
  //   server.send(200, "text/html", loginIndex);
  // });
  server.on("/",         HomePage);
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      ss_printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        ss_printf("\r\nUpdate Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });

  server.on("/download", File_Download);
  server.on("/upload",   File_Upload);
  server.on("/fupload",  HTTP_POST,[](){ server.send(200);}, handleFileUpload);
  server.on("/stream",   File_Stream);
  server.on("/delete",   File_Delete);
  server.on("/dir",      SPIFFS_dir);

  server.begin();
  ss_printf("\r\n Waiting for request from web-browser");
  while(1)
  {
    server.handleClient();
    delay(1);
  }

    }


    if(count%10==0)
    { ss_printf("\r\n SNo=%d  %d", count, digitalRead(CONSUMER)); }
    
    delay(100);    
  }
  lcd_row += lcd_row_incrementer;

  while(digitalRead(CONSUMER) == LOW)  // Let the user release the WiFi key
    ;
  delay(1000);
  pinMode(CONSUMER, OUTPUT);   

  tft.setTextColor(TFT_GREEN, TFT_BLACK); //  text, background
  tft.drawString("Robot Run Mode", 0, lcd_row); lcd_row += lcd_row_incrementer;
  
  ss_puts("\r\n Robot Run Mode");

  
  ss_puts("\r\n Starting Camera Configuration ....");
  
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
  config.pixel_format = PIXFORMAT_RGB565; 

    // FRAMESIZE_96X96,    // 96x96
    // FRAMESIZE_QQVGA,    // 160x120
    // FRAMESIZE_QCIF,     // 176x144
    // FRAMESIZE_HQVGA,    // 240x176
    // FRAMESIZE_240X240,  // 240x240
    // FRAMESIZE_QVGA,     // 320x240 // ASG: There are many more otions available.

  //config.frame_size = FRAMESIZE_QQVGA;
  config.frame_size = FRAMESIZE_240X240; // ASG: Good for our TFT (use this for initial coding purposes)
  //config.frame_size = FRAMESIZE_QVGA;
  //config.frame_size = FRAMESIZE_96X96; // ASG: Goof for memory saving (Advanced users only)

  // ASG: Double buffering: Good for videos; NOT good for robot still imgaes
  config.fb_count = 1;   ////  2; //// <------ Frame buffer count 
  
  ss_puts("\r\n About to init ...");
  esp_err_t err = esp_camera_init(&config); // Init Camera
  if (err != ESP_OK) {
      ss_puts("\r\n ERROR: err != ESP_OK");
      tft.setTextColor(TFT_WHITE, TFT_RED); //  text, background
      tft.setTextSize(TEXT_SIZE);
      tft.drawString("CAMERA: ERROR", 0, lcd_row); lcd_row += lcd_row_incrementer;
      tft.drawString("Press Reset  ", 0, lcd_row); lcd_row += lcd_row_incrementer;
      tft.drawString("to try again ", 0, lcd_row); lcd_row += lcd_row_incrementer;
      hang();
      return;
  }

  tft.setTextColor(TFT_GREEN, TFT_BLACK); //  text, background
  tft.drawString("CAMERA: Init Okay", 0, lcd_row); lcd_row += lcd_row_incrementer;
  
  delay(500);
 
  Wire.begin(SDA_PIN, SCL_PIN);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

     // NOTE: To command any device connected to Blue PCB (PCA9685), the Blue PCBs output should be anabled.
   //       A LOW on OE pin on Blue PCB (Blue Wire) is output enable
   //       Devices such as Servo Motors, Electro Magnet, Laser and Buzzer are connected to Blue PCB  (PCA9685)
            digitalWrite(PCA_ENABLE_PIN_NO, LOW); // When this pin is low all 16- output pins are enabled
  
  // Servo Motor positions while assembling the robot:
  // Bring motors to these positions first and then "join" the link joints together.
  // Keep the circuit / motors powered while assembling
  // Please note that plastic gears are delicate so handel them carefully.
  //
  // Positions during robot assembly:
  // Servo 1 : 180'   Link-1 in Extreme Left (RIght handed human seated on human side of checker board)
  // Servo 2 : Middle Link-2A in middle position
  // Servo 3 : 0' Link-2B in "pick the checker" position (extream downward)
  //
  //
  // First connect only M1 to Blue PCB 
  // once it is assembled
  // then addtionally connect Link2A's motor M2 to Blue PCB
  // once it is assembled
  // then addtionally connect Link2B's motor M3 to Blue PCB
  // once it is assembled
  // then addtionally connect Laser (and ELectromagnet) driver PCB to Blue Board.
  //
  // the Laser diode should project the Red spot on the calibration point.
  //
    servonum    = 0; // Valid Values 0, 1, 2
    position_uS = 2400;
    pwm.writeMicroseconds(servonum, position_uS);   
    ss_printf("\r\n  Servo# %d : %d us", servonum, position_uS);
    tft.drawString("Servo# ",   0, lcd_row); tft.drawNumber(servonum,  100, lcd_row); tft.drawNumber(position_uS,  140, lcd_row);
    lcd_row += lcd_row_incrementer; // Draw integer using current font       
    delay(5000); // Wait for few seconds

    servonum    = 1; // Valid Values 0, 1, 2
    position_uS = 1500;  // (2400 + 600) /2 = 1500
    pwm.writeMicroseconds(servonum, position_uS);    
    ss_printf("\r\n  Servo# %d : %d us", servonum, position_uS);
    tft.drawString("Servo# ",   0, lcd_row); tft.drawNumber(servonum,  100, lcd_row); tft.drawNumber(position_uS,  140, lcd_row);
    lcd_row += lcd_row_incrementer; // Draw integer using current font       
    delay(5000); // Wait for few seconds

    servonum    = 2; // Valid Values 0, 1, 2
    position_uS = 600; // Extream
    pwm.writeMicroseconds(servonum, position_uS);    
    ss_printf("\r\n  Servo# %d : %d us", servonum, position_uS);
    tft.drawString("Servo# ",   0, lcd_row); tft.drawNumber(servonum,  100, lcd_row); tft.drawNumber(position_uS,  140, lcd_row);
    lcd_row += lcd_row_incrementer; // Draw integer using current font       
    delay(5000); // Wait for few seconds

    ss_printf("\r\n Laser is ON (EM is OFF)...");
    tft.drawString("Laser: ON",   0, lcd_row);  lcd_row += lcd_row_incrementer; 
    pwm.setPWM(PWM_CHANNEL_NO_LASER, 0, 4096); // LASER ON
    
    tft.drawString("EM: OFF",   0, lcd_row);  lcd_row += lcd_row_incrementer; 
    pwm.setPWM(PWM_CHANNEL_NO_EM, 0, 4096); // EM OFF
    
    
  //while(1);

// TESTs LASER and Electro Magnet (EM)   ONLY  (Note: Infinite loop):-  
        digitalWrite(PCA_ENABLE_PIN_NO, LOW); // When this pin is low all pins are enabled.
          //while(1) 
          {
          ss_printf("\r\n Laser and EM are ON ...");
          pwm.setPWM(PWM_CHANNEL_NO_EM, 4096, 0); // EM ON
          pwm.setPWM(PWM_CHANNEL_NO_LASER, 0, 4096); // LASER ON
          delay(5000);
          
          ss_printf("\r\n Laser OFF ...");
          pwm.setPWM(PWM_CHANNEL_NO_EM, 0, 4096);  // EM OFF
          pwm.setPWM(PWM_CHANNEL_NO_LASER, 4096, 0);  // LASER OFF
          delay(3000);
        }

  tft.setTextColor(TFT_WHITE, TFT_GREEN); //  text, background
  tft.drawString(" ALL - OK ", 0, lcd_row); lcd_row += lcd_row_incrementer;
  delay(10);
  

  tft.fillScreen(TFT_BLACK);
  count=0;
}
//////////////////////////////////////////////////////////////////////////////
camera_fb_t * fb;
void loop()
{ 
lcd_row = 240;
ss_printf("\r\n  -------------------------");
ss_put_long(count);
//ss_puts("  ");

if(count%2)
{
  digitalWrite(LED_RED_INBUILD_PIN_NO, HIGH);  
}
else
{  
  digitalWrite(LED_RED_INBUILD_PIN_NO, LOW);  
}

/*
// ASG: DO NOT DELETE THIS COMMENT, you might need it, unless you find a better way
// Trick: Capture two images, first time it will returen the buffered old image, the second image will be fresh one!
// This is the trick to overcome doubble-buffering:-

// fb = esp_camera_fb_get();   // Capture a frame from the camera
// esp_camera_fb_return(fb); // read the image twice to clear the buffer and force the ESP32 to fill it up again with the fresh image
*/

fb = esp_camera_fb_get();   // Capture a frame from the camera
if (!fb) {
                delay(250); // If failure, try again, atleast once
                camera_fb_t * fb = esp_camera_fb_get();
                if (!fb) {                  
                  ss_puts("\r\n Camera capture failed");
                  tft.setTextColor(TFT_WHITE, TFT_RED); //  text, background
                  tft.drawString("ERROR: fb_get()", 0, lcd_row); lcd_row += lcd_row_incrementer;
                  hang();
                  return;
                }
  }
  delay(1);
  tft.pushImage(0, 0, fb->width, fb->height, (uint16_t*)fb->buf); // Display on LCD
  
  saveRGB565ToSPIFFS(SPIFFS, "/image.rgb", (uint16_t*)fb->buf, ((fb->width) * (fb->height)) );  // Save RGB565 raw image to SPIFFS
 
 
  ////  snprintf(buffer_temp, sizeof(buffer_temp), "image_%06d.rgb", (int) count); // make a unique file name
  ////  saveRGB565ToSPIFFS(SPIFFS, buffer_temp, (uint16_t*)fb->buf, ((fb->width) * (fb->height)) );  // Save RGB565 raw image to SPIFFS

  esp_camera_fb_return(fb);   // Return the frame buffer: Memory reclaim

  ss_printf("\r\n Image file name: %s", buffer_temp);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); //  text, background
  tft.drawString("Frame# ",   0, lcd_row);
  tft.drawNumber(count,     100, lcd_row); lcd_row += lcd_row_incrementer; // Draw integer using current font
      


  ss_puts("\r\n Loop Ends \r\n ------------------------------");
  count++;
  if(count==5)
   { 
     count = 0;
    //  ss_puts("\r\n STOP");
    //  tft.setTextColor(TFT_RED, TFT_BLACK); //  text, background
    //  tft.drawString(" S T O P ",   0, lcd_row);
     
    //  while(1);
   }
} // loop()
//////////////////////////////////////////////////////////////////////////////////////////////////
void hang(void)
{
  // Disable PCA Output (PCA is a Active Low device) :
  digitalWrite(PCA_ENABLE_PIN_NO, HIGH); // When this pin is low all pins are enabled.
  ss_puts("\r\n HANG");
  
  tft.setTextColor(TFT_WHITE, TFT_RED); //  text, background
  tft.setTextSize(2);
  tft.drawString("    H A N G    ", 0, 0);
  
  while(1)
  {
    ss_puts(" HANG ");
    digitalWrite(LED_RED_INBUILD_PIN_NO, LED_INBUILD_ON); 
    delay(500);

    ss_puts(" . ");
    digitalWrite(LED_RED_INBUILD_PIN_NO, LED_INBUILD_OFF); 
    delay(500);
  
  }

}
//////////////////////////////////////////////////////////////////////////////
void saveRGB565ToSPIFFS(fs::FS &fs, const char * path, uint16_t * buffer, size_t len) {

  if(SPIFFS_present==true){
    File file = fs.open(path, FILE_WRITE);
    if (!file) {
      ss_puts("\r\n Failed to open file for writing");
      return;
    }
    file.write((uint8_t *)buffer, len);
    file.close();
  }
}
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// All supporting functions from here...
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void HomePage(){
  SendHTML_Header();
  webpage += F("<a href='/download'><button>Download</button></a>");
  webpage += F("<a href='/upload'><button>Upload</button></a>");
  webpage += F("<a href='/stream'><button>Stream</button></a>");
  webpage += F("<a href='/delete'><button>Delete</button></a>");
  webpage += F("<a href='/dir'><button>Directory</button></a>");
  webpage += F("<a href='/serverIndex'><button>Sketch Upload</button></a>");


  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop(); // Stop is needed because no content length was sent
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File_Download(){ // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments
  if (server.args() > 0 ) { // Arguments were received
    if (server.hasArg("download")) DownloadFile(server.arg(0));
  }
  else SelectInput("Enter filename to download","download","download");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void DownloadFile(String filename){
  if (SPIFFS_present) { 
    File download = SPIFFS.open("/"+filename,  "r");
    if (download) {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename="+filename);
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");
      download.close();
    } else ReportFileNotPresent("download"); 
  } else ReportSPIFFSNotPresent();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File_Upload(){
  append_page_header();
  webpage += F("<h3>Select File to Upload</h3>"); 
  webpage += F("<FORM action='/fupload' method='post' enctype='multipart/form-data'>");
  webpage += F("<input class='buttons' style='width:40%' type='file' name='fupload' id = 'fupload' value=''><br>");
  webpage += F("<br><button class='buttons' style='width:10%' type='submit'>Upload File</button><br>");
  webpage += F("<a href='/'>[Back]</a><br><br>");
  append_page_footer();
  server.send(200, "text/html",webpage);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
File UploadFile; 
void handleFileUpload(){ // upload a new file to the Filing system
  HTTPUpload& uploadfile = server.upload(); // See https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer/srcv
                                            // For further information on 'status' structure, there are other reasons such as a failed transfer that could be used
  if(uploadfile.status == UPLOAD_FILE_START)
  {
    String filename = uploadfile.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    ss_printf("\r\n Upload File Name: %s", filename);
    SPIFFS.remove(filename);                  // Remove a previous version, otherwise data is appended the file again
    UploadFile = SPIFFS.open(filename, "w");  // Open the file for writing in SPIFFS (create it, if doesn't exist)
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    if(UploadFile) UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Write the received bytes to the file
  } 
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if(UploadFile)          // If the file was successfully created
    {                                    
      UploadFile.close();   // Close the file again
      ss_printf("\r\n Upload Size: %d", uploadfile.totalSize);
      webpage = "";
      append_page_header();
      webpage += F("<h3>File was successfully uploaded</h3>"); 
      webpage += F("<h2>Uploaded File Name: "); webpage += uploadfile.filename+"</h2>";
      webpage += F("<h2>File Size: "); webpage += file_size(uploadfile.totalSize) + "</h2><br>"; 
      append_page_footer();
      server.send(200,"text/html",webpage);
    } 
    else
    {
      ReportCouldNotCreateFile("upload");
    }
  }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///// #ifdef ESP32
void SPIFFS_dir(){ 
  if (SPIFFS_present) { 
    File root = SPIFFS.open("/");
    if (root) {
      root.rewindDirectory();
      SendHTML_Header();
      webpage += F("<h3 class='rcorners_m'>SD Card Contents</h3><br>");
      webpage += F("<table align='center'>");
      webpage += F("<tr><th>Name/Type</th><th style='width:20%'>Type File/Dir</th><th>File Size</th></tr>");
      printDirectory("/",0);
      webpage += F("</table>");
      SendHTML_Content();
      root.close();
    }
    else 
    {
      SendHTML_Header();
      webpage += F("<h3>No Files Found</h3>");
    }
    append_page_footer();
    SendHTML_Content();
    SendHTML_Stop();   // Stop is needed because no content length was sent
  } else ReportSPIFFSNotPresent();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void printDirectory(const char * dirname, uint8_t levels){
  File root = SPIFFS.open(dirname);
  if(!root){
    return;
  }
  if(!root.isDirectory()){
    return;
  }
  File file = root.openNextFile();
  while(file){
    if (webpage.length() > 1000) {
      SendHTML_Content();
    }
    if(file.isDirectory()){
      webpage += "<tr><td>"+String(file.isDirectory()?"Dir":"File")+"</td><td>"+String(file.name())+"</td><td></td></tr>";
      printDirectory(file.name(), levels-1);
    }
    else
    {
      webpage += "<tr><td>"+String(file.name())+"</td>";
      webpage += "<td>"+String(file.isDirectory()?"Dir":"File")+"</td>";
      int bytes = file.size();
      String fsize = "";
      if (bytes < 1024)                     fsize = String(bytes)+" B";
      else if(bytes < (1024 * 1024))        fsize = String(bytes/1024.0,3)+" KB";
      else if(bytes < (1024 * 1024 * 1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
      else                                  fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
      webpage += "<td>"+fsize+"</td></tr>";
    }
    file = root.openNextFile();
  }
  file.close();
}
////#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  #ifdef ESP8266
// void SPIFFS_dir(){
//   String str;
//   if (SPIFFS_present) { 
//     Dir dir = SPIFFS.openDir("/");
//     SendHTML_Header();
//     webpage += F("<h3 class='rcorners_m'>SPIFFS Card Contents</h3><br>");
//     webpage += F("<table align='center'>");
//     webpage += F("<tr><th>Name/Type</th><th style='width:40%'>File Size</th></tr>");
//     while (dir.next()) {
//       ss_printf("\r\n %s", dir.fileName());
//       webpage += "<tr><td>"+String(dir.fileName())+"</td>";
//       str  = dir.fileName();
//       str += " / ";
//       if(dir.fileSize()) {
//         File f = dir.openFile("r");
//         ss_printf(" Size: %d", f.size());
//         int bytes = f.size();
//         String fsize = "";
//         if (bytes < 1024)                     fsize = String(bytes)+" B";
//         else if(bytes < (1024 * 1024))        fsize = String(bytes/1024.0,3)+" KB";
//         else if(bytes < (1024 * 1024 * 1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
//         else                                  fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
//         webpage += "<td>"+fsize+"</td></tr>";
//         f.close();
//       }
//       str += String(dir.fileSize());
//       str += "\r\n";
//       ss_puts(str);
//     }
//     webpage += F("</table>");
//     SendHTML_Content();
//     append_page_footer();
//     SendHTML_Content();
//     SendHTML_Stop();   // Stop is needed because no content length was sent
//   } else ReportSPIFFSNotPresent();
// }
// #endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File_Stream(){
  if (server.args() > 0 ) { // Arguments were received
    if (server.hasArg("stream")) SPIFFS_file_stream(server.arg(0));
  }
  else SelectInput("Enter a File to Stream","stream","stream");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SPIFFS_file_stream(String filename) { 
  if (SPIFFS_present) { 
    File dataFile = SPIFFS.open("/"+filename,  "r"); // Now read data from SPIFFS Card 
    if (dataFile) { 
      if (dataFile.available()) { // If data is available and present 
        String dataType = "application/octet-stream"; 
        if (server.streamFile(dataFile, dataType) != dataFile.size()) {Serial.print(F("Sent less data than expected!")); } 
      }
      dataFile.close(); // close the file: 
    } else ReportFileNotPresent("Cstream");
  } else ReportSPIFFSNotPresent(); 
}   
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void File_Delete(){
  if (server.args() > 0 ) { // Arguments were received
    if (server.hasArg("delete")) SPIFFS_file_delete(server.arg(0));
  }
  else SelectInput("Select a File to Delete","delete","delete");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SPIFFS_file_delete(String filename) { // Delete the file 
  if (SPIFFS_present) { 
    SendHTML_Header();
    File dataFile = SPIFFS.open("/"+filename, "r"); // Now read data from SPIFFS Card 
    if (dataFile)
    {
      if (SPIFFS.remove("/"+filename)) {
        ss_puts("\r\n File deleted successfully");
        webpage += "<h3>File '"+filename+"' has been erased</h3>"; 
        webpage += F("<a href='/delete'>[Back]</a><br><br>");
      }
      else
      { 
        webpage += F("<h3>File was not deleted - error</h3>");
        webpage += F("<a href='delete'>[Back]</a><br><br>");
      }
    } else ReportFileNotPresent("delete");
    append_page_footer(); 
    SendHTML_Content();
    SendHTML_Stop();
  } else ReportSPIFFSNotPresent();
} 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Header(){
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate"); 
  server.sendHeader("Pragma", "no-cache"); 
  server.sendHeader("Expires", "-1"); 
  server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves. 
  append_page_header();
  server.sendContent(webpage);
  webpage = "";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Content(){
  server.sendContent(webpage);
  webpage = "";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SendHTML_Stop(){
  server.sendContent("");
  server.client().stop(); // Stop is needed because no content length was sent
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SelectInput(String heading1, String command, String arg_calling_name){
  SendHTML_Header();
  webpage += F("<h3>"); webpage += heading1 + "</h3>"; 
  webpage += F("<FORM action='/"); webpage += command + "' method='post'>"; // Must match the calling argument e.g. '/chart' calls '/chart' after selection but with arguments!
  webpage += F("<input type='text' name='"); webpage += arg_calling_name; webpage += F("' value=''><br>");
  webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value=''><br><br>");
  webpage += F("<a href='/'>[Back]</a><br><br>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReportSPIFFSNotPresent(){
  SendHTML_Header();
  webpage += F("<h3>No SPIFFS Card present</h3>"); 
  webpage += F("<a href='/'>[Back]</a><br><br>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReportFileNotPresent(String target){
  SendHTML_Header();
  webpage += F("<h3>File does not exist</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[Back]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ReportCouldNotCreateFile(String target){
  SendHTML_Header();
  webpage += F("<h3>Could Not Create Uploaded File (write-protected?)</h3>"); 
  webpage += F("<a href='/"); webpage += target + "'>[Back]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
String file_size(int bytes){
  String fsize = "";
  if (bytes < 1024)                 fsize = String(bytes)+" B";
  else if(bytes < (1024*1024))      fsize = String(bytes/1024.0,3)+" KB";
  else if(bytes < (1024*1024*1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
  else                              fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
  return fsize;
}
