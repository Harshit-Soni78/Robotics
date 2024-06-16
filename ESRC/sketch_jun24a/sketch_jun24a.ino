/*
Q6 Write an Interactive program for switching the LEDs, ON and OFF. Connect four LEDs to the Arduino
with four different resistances of 470-ohm each. The sample input-output of the program is shown below:
Do you want to glow RED LED : y/n
User enters: y
Do you want to glow GREEN LED : y/n
User enters: y
Do you want to glow YELLOW LED : y/n
User enters: n
Do you want to glow WHITE LED : y/n
User enters: y
The status of the LEDs is updated as per user requirement.
HINT: Use Serial.print() to display messages on Serial Port. Use some kind of Serial input function to 
input user response. This program is just like a printf() and scanf() type of exercise!
*/
char p=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   const int LED_red =2;
   const int LED_Green = 3;
   const int LED_yellow =4;
   const int LED_white = 5;
   const int LED_Blue =6;
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
}
int led (char p){
  if(p=='y'){
    return 1;
  }
  else if(p){
    return 0;
  }
}
void loop() {
      digitalWrite(2, 0);
      digitalWrite(3, 0);
      digitalWrite(4, 0);
      digitalWrite(5, 0);
      digitalWrite(6, 0);
  Serial.print("Enter Which colour do you want to blink \n");
  Serial.print("Do you want to glow red led \n");
  while(Serial.available()==0);
    p = Serial.read();
    Serial.print(p);
    Serial.println("");
  digitalWrite(2, led(p) );
  delay(1000);
  p=0;
  Serial.print("Do you want to glow Green led \n");
  while(Serial.available()==0);
    p = Serial.read();
    Serial.print(p);
    Serial.println("");
  digitalWrite(3, led(p) );
  delay(1000);
    p=0;
  Serial.print("Do you want to glow yellow led \n");
  while(Serial.available()==0);
    p = Serial.read();
    Serial.print(p);
    Serial.println("");
  digitalWrite(4, led(p) );
  delay(1000);
    p=0;
  Serial.print("Do you want to glow white led \n");
  while(Serial.available()==0);
    p = Serial.read();
    Serial.print(p);
    Serial.println("");
  digitalWrite(5, led(p) );
  delay(1000);
    p=0;
  Serial.print("Do you want to glow blue led \n");
  while(Serial.available()==0);
    p = Serial.read();
    Serial.print(p);
    Serial.println("");
  digitalWrite(6, led(p) );
  delay(10000);
  }
