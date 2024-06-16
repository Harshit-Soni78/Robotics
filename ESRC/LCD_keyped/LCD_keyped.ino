#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4); 
const byte ROWS = 4; 
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
void setup(){
    Serial.begin(9600);
    lcd.init();    
    lcd.init();
    lcd.backlight();
}
void loop(){
    char customKey = customKeypad.getKey();
    lcd.setCursor(0,0);
    lcd.print(" Team no. 5  !!");
    if (customKey){
        lcd.setCursor(15,0);
        lcd.println(customKey);
  }
    lcd.setCursor(0,1);
    lcd.print("The Robo Wizard");
}
