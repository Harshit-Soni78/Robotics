#define CAMERA_MODEL_AI_THINKER // Has PSRAM

#include <stdint.h>
//#include <stdlib.h>

#include "esp_camera.h"
#include "camera_pins.h"
#include <TFT_eSPI.h>       // Hardware-specific library

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "soft_serial_tx_util.c"

#define TEXT_SIZE   2 // Avoid changing. Try, NOT to change

// #define SERVOMIN   150 // This is the 'minimum' pulse length count (out of 4096)
// #define SERVOMAX   600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN      600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX     2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ  50 // Analog servos run at ~50 Hz updates

uint8_t servonum = 0;

// Include the header files that contain the icons
//#include "Alert.h"
//#include "Close.h"
//#include "Info.h"


#define SDA_PIN                    13  // Fixed for TL_RAN PCB
#define SCL_PIN                    14  // Fixed for TL_RAN PCB
#define PCA_ENABLE_PIN_NO           3  //  Fixed for TL_RAN PCB
#define LED_RED_INBUILD_PIN_NO     33  //  Fixed for ESP32-CAM 

#define PWM_CHANNEL_NO_EM     7  // On PCA PCB
#define PWM_CHANNEL_NO_LASER  6  // On PCA PCB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Buzzer 15 // Buzzer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // Default I2C address 0x40
int channel = 0;
int value = 0;
int position_uS;
long count = 0; // Loop count
int lcd_row;
const int lcd_row_incrementer = 20;

//#define EM_ON  0
//#define EM_OFF 1

#define LED_INBUILD_ON  0
#define LED_INBUILD_OFF 1

void hang(void);

void setup()
{
  //////software_serial_configure();
  pinMode(LED_RED_INBUILD_PIN_NO, OUTPUT);  digitalWrite(LED_RED_INBUILD_PIN_NO, LED_INBUILD_OFF); 
  pinMode(4, OUTPUT);  digitalWrite(4, LOW); 
  pinMode(PCA_ENABLE_PIN_NO, OUTPUT);  digitalWrite(PCA_ENABLE_PIN_NO, HIGH); // Enable PCA module
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    pinMode(Buzzer , OUTPUT);  // Buzzer
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ss_puts("\r\n =================  RESET ESP32-CAM-CH340  =================\r\n");
  
  tft.begin(); // Resolution in UserSteup file 320 x 240
  tft.setRotation(0);	// 0 & 2 Portrait. 1 & 3 landscape
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(TEXT_SIZE);
  
  //tft.setSwapBytes(true);  // Swap the colour byte order when rendering
 
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //  text, background
  tft.drawString("ESP32-CAM with ST7789V", 0, lcd_row); lcd_row += lcd_row_incrementer;

  tft.setTextColor(TFT_WHITE, TFT_RED); //  text, background
  tft.drawString(" R E S E T", 0, lcd_row); lcd_row += lcd_row_incrementer;
  
   
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
  // config.pixel_format = PIXFORMAT_RGB565; 
  config.pixel_format = PIXFORMAT_GRAYSCALE;

    // FRAMESIZE_96X96,    // 96x96
    // FRAMESIZE_QQVGA,    // 160x120
    // FRAMESIZE_QCIF,     // 176x144
    // FRAMESIZE_HQVGA,    // 240x176
    // FRAMESIZE_240X240,  // 240x240
    // FRAMESIZE_QVGA,     // 320x240 // ASG: There are many more otions available.

  // config.frame_size = FRAMESIZE_QQVGA;
  config.frame_size = FRAMESIZE_240X240; // ASG: Good for our TFT (use this for initial coding purposes)
  //config.frame_size = FRAMESIZE_QVGA;
  // config.frame_size = FRAMESIZE_96X96; // ASG: Goof for memory saving (Advanced users only)

  // ASG: Double buffering: Good for videos; NOT good for robot still imgaes
  config.fb_count = 2;   ////  2; //// <------ Frame buffer count 
  
  esp_err_t err = esp_camera_init(&config); // Init Camera
  if (err != ESP_OK) {
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
    ss_puts("\r\n Servo# "); ss_put_int(servonum); ss_puts(" : ");  ss_put_int(position_uS);
    tft.drawString("Servo# ",   0, lcd_row); tft.drawNumber(servonum,  100, lcd_row); tft.print(" "); tft.drawNumber(position_uS,  100, lcd_row);
    lcd_row += lcd_row_incrementer; // Draw integer using current font       
    delay(500); // Wait for few seconds

    servonum    = 1; // Valid Values 0, 1, 2
    position_uS = 1500;  // (2400 + 600) /2 = 1500
    pwm.writeMicroseconds(servonum, position_uS);    
    ss_puts("\r\n Servo# "); ss_put_int(servonum); ss_puts(" : ");  ss_put_int(position_uS);
    tft.drawString("Servo# ",   0, lcd_row); tft.drawNumber(servonum,  100, lcd_row); tft.print(" "); tft.drawNumber(position_uS,  100, lcd_row);
    lcd_row += lcd_row_incrementer; // Draw integer using current font       
    delay(500); // Wait for few seconds

    servonum    = 2; // Valid Values 0, 1, 2
    position_uS = 600; // Extream
    pwm.writeMicroseconds(servonum, position_uS);    
    ss_puts("\r\n Servo# "); ss_put_int(servonum); ss_puts(" : ");  ss_put_int(position_uS);
    tft.drawString("Servo# ",   0, lcd_row); tft.drawNumber(servonum,  100, lcd_row); tft.print(" "); tft.drawNumber(position_uS,  100, lcd_row);
    lcd_row += lcd_row_incrementer; // Draw integer using current font       
    delay(500); // Wait for few seconds

    ss_puts("\r\n Laser is ON (EM is OFF)...");
    pwm.setPWM(PWM_CHANNEL_NO_EM, 0, 4096); // EM OFF
    pwm.setPWM(PWM_CHANNEL_NO_LASER, 0, 4096); // LASER ON
    delay(500);

  



  // while(1);

// TESTs LASER and Electro Magnet (EM)   ONLY  (Note: Infinite loop):-  
        digitalWrite(PCA_ENABLE_PIN_NO, LOW); // When this pin is low all pins are enabled.
          // while(1)
           {
          ss_puts("\r\n Laser and EM are ON ...");
          pwm.setPWM(PWM_CHANNEL_NO_EM, 4096, 0); // EM ON
          pwm.setPWM(PWM_CHANNEL_NO_LASER, 0, 4096); // LASER ON
          delay(500);
          
          ss_puts("\r\n Laser OFF ...");
          pwm.setPWM(PWM_CHANNEL_NO_EM, 0, 4096);  // EM OFF
          pwm.setPWM(PWM_CHANNEL_NO_LASER, 4096, 0);  // LASER OFF
          delay(300);
        }

  tft.setTextColor(TFT_WHITE, TFT_GREEN); //  text, background
  tft.drawString(" ALL - OK ", 0, lcd_row); lcd_row += lcd_row_incrementer;
  delay(10);
  

  tft.fillScreen(TFT_BLACK);
}
//////////////////////////////////////////////////////////////////////////////
camera_fb_t * fb;
void loop()
{ 
lcd_row = 240;
ss_puts("\r\n  -------------------------");
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
                  // Serial.println("Camera capture failed");
                  tft.setTextColor(TFT_WHITE, TFT_RED); //  text, background
                  tft.setTextSize(2);
                  tft.drawString("ERROR: fb_get()", 0, lcd_row); lcd_row += lcd_row_incrementer;
                  hang();
                  return;
                }
  }
  delay(1);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // int i =0 ;
  // int sum=0;
  // int threshold = 150 ;
  // int count;


  
  // for( ; i<fb->len ; i++){
       
  //      sum+= fb->buf[i];
       

  //   if(fb->buf[i] < threshold/2){
  //       fb->buf[i]=0;
  //   }
  //   // else if(fb->buf[i] > threshold/2 && fb->buf[i] < threshold){
  //       // fb->buf[i]=125;
  //   }
  //   else{
  //       fb->buf[i]=255;
  //   }
  // }

    // sum = sum/fb->len ;


    // int a=sum;
    // int b=sum;



    // if( a != b ){
    //     tft.drawString("Error# ", 100 , lcd_row);
    // }

  tft.pushImage(0, 0, fb->width, fb->height, (uint8_t*)fb->buf); // Display on LCD

  esp_camera_fb_return(fb);   // Return the frame buffer: Memory reclaim

  tft.setTextColor(TFT_GREEN, TFT_BLACK); //  text, background
  tft.drawString("Frame# ",   0, lcd_row);
  tft.drawNumber(count,     100, lcd_row); lcd_row += lcd_row_incrementer; // Draw integer using current font
      


      
   

      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  ss_puts("\r\n Loop Ends \r\n ------------------------------");
  count++;
} // loop()
//////////////////////////////////////////////////////////////////////////////////////////////////
void hang(void)
{
  // Disable PCA Output (PCA is a Active Low device) :
  digitalWrite(PCA_ENABLE_PIN_NO, HIGH); // When this pin is low all pins are enabled.
  ss_puts("\r\n HANG");
  
  tft.setTextColor(TFT_WHITE, TFT_RED); //  text, background
  tft.setTextSize(2);
  tft.drawString(" H A N G ", 0, 80);
  
  // while(1)
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

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////