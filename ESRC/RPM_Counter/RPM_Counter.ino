#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4); 
float value=0;
float rev=0;
int rpm;
int oldtime=0;
int time;
int sec;
void isr() 
{
rev++;
}

void setup()
{
lcd.begin(16,2);         
attachInterrupt(0,isr,RISING);  
}

void loop()
{
delay(1000);
detachInterrupt(0);       
time=millis()-oldtime;   
rpm=(rev/time)*60000;         
oldtime=millis();            
rev=0;
lcd.clear();
lcd.setCursor(0,0);
lcd.print("   RPM COUNTER  ");
lcd.setCursor(0,1);
lcd.print(     rpm);
lcd.print(" RPM");
lcd.print(" ");
sec=rpm/60;
//lcd.print(   sec);
//lcd.print("/s");
attachInterrupt(0,isr,RISING);
}

    
