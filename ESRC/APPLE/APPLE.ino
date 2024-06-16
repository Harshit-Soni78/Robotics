/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  const int a=2;
  const int b=3;
  const int c=4;
  const int d=5;
  const int e=6;
  const int f=7;
  const int g=8;
  const int h=9;
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(2, 1);  
  digitalWrite(3, 1);  
  digitalWrite(4, 1);  
  digitalWrite(5, 1);
  digitalWrite(6, 1);
  digitalWrite(7, 0);
  digitalWrite(8, 1);
  digitalWrite(9, 0); 
  delay(1000);
  digitalWrite(2, 0);  
  digitalWrite(3, 0); 
  digitalWrite(4, 0);  
  digitalWrite(5, 0);  
  digitalWrite(6, 0);  
  digitalWrite(7, 0);  
  digitalWrite(8, 0);  
  digitalWrite(9, 1);  
  delay(1000);
  digitalWrite(2, 1);  
  digitalWrite(3, 1);  
  digitalWrite(4, 1);  
  digitalWrite(5, 1);  
  digitalWrite(6, 1);  
  digitalWrite(7, 0);  
  digitalWrite(8, 0); 
  digitalWrite(9, 0);
  delay(1000);
  digitalWrite(2, 0);  
  digitalWrite(3, 0);  
  digitalWrite(4, 0);  
  digitalWrite(5, 0);  
  digitalWrite(6, 0);  
  digitalWrite(7, 0);  
  digitalWrite(8, 0); 
  digitalWrite(9, 1);
  delay(1000);
  digitalWrite(2, 1);  
  digitalWrite(3, 1);  
  digitalWrite(4, 1);  
  digitalWrite(5, 1);  
  digitalWrite(6, 1);  
  digitalWrite(7, 0);  
  digitalWrite(8, 0); 
  digitalWrite(9, 0);
  delay(1000);
  digitalWrite(2, 0);  
  digitalWrite(3, 0);  
  digitalWrite(4, 0);  
  digitalWrite(5, 0);  
  digitalWrite(6, 0);  
  digitalWrite(7, 0);  
  digitalWrite(8, 0); 
  digitalWrite(9, 1);
  delay(1000);
  digitalWrite(2, 0);  
  digitalWrite(3, 1);  
  digitalWrite(4, 0);  
  digitalWrite(5, 0);  
  digitalWrite(6, 1);  
  digitalWrite(7, 1);  
  digitalWrite(8, 0); 
  digitalWrite(9, 0);
  delay(1000);
  digitalWrite(2, 0);  
  digitalWrite(3, 0);  
  digitalWrite(4, 0);  
  digitalWrite(5, 0);  
  digitalWrite(6, 0);  
  digitalWrite(7, 0);  
  digitalWrite(8, 0); 
  digitalWrite(9, 1);
  delay(1000);
  digitalWrite(2, 1);  
  digitalWrite(3, 1);  
  digitalWrite(4, 1);  
  digitalWrite(5, 0);  
  digitalWrite(6, 1);  
  digitalWrite(7, 1);  
  digitalWrite(8, 0); 
  digitalWrite(9, 0);
  delay(1000);
  digitalWrite(2, 0);  
  digitalWrite(3, 0);  
  digitalWrite(4, 0);  
  digitalWrite(5, 0);  
  digitalWrite(6, 0);  
  digitalWrite(7, 0);  
  digitalWrite(8, 0); 
  digitalWrite(9, 1);
  delay(1000);
}
