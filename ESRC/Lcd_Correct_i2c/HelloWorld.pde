#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
void setup()
{
  lcd.init(); 
  lcd.init();
  lcd.backlight();
  
}
void loop()
{
    lcd.setCursor(0,0);
    lcd.print(millis() / 1000);
  lcd.print("  Team no:- 5 ");
  lcd.setCursor(0,1);
  lcd.print("The Robo Wizards");
}
