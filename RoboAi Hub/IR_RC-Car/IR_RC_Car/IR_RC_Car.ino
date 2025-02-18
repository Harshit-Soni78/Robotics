#include <IRremote.h>
#include <IRremoteInt.h>
#include <Wire.h>

int RECV_PIN = 13;          
IRrecv receiver(RECV_PIN); 
decode_results results;     

void setup() {
  Serial.begin(9600);       
  receiver.enableIRIn();
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void forward() {
  analogWrite(10, 255);
  digitalWrite(4, HIGH);
  digitalWrite (5, LOW);
  analogWrite(11, 255);
  digitalWrite (6, LOW);
  digitalWrite(7, HIGH);
  Serial.println("forward");
}
void backwards() {
  analogWrite(10, 255);
  digitalWrite(4, LOW);
  digitalWrite (5, HIGH);
  analogWrite(11, 255);
  digitalWrite (6, HIGH);
  digitalWrite(7, LOW);
  Serial.println("backwards");
}
void left() {
  analogWrite(10, 255);
  digitalWrite(4, HIGH);
  digitalWrite (5, LOW);
  analogWrite(11, 255);
  digitalWrite (6, HIGH);
  digitalWrite(7, LOW);
  delay(1000);
  stopp();
  Serial.println("left");
}
void right() {
  analogWrite(10, 255);
  digitalWrite(4, LOW);
  digitalWrite (5, HIGH);
  analogWrite(11, 255);
  digitalWrite (6, LOW);
  digitalWrite(7, HIGH);
  delay(1000);
  stopp();
  Serial.println("right");
}
void stopp() {
  analogWrite(10, 0);
  digitalWrite(4, LOW);
  digitalWrite (5, LOW);
  analogWrite(11, 0);
  digitalWrite (6, LOW);
  digitalWrite(7, LOW);
  Serial.println("stop");
}

void loop() {

  if (receiver.decode(&results)) {            
    if (results.value == 0xFF6897) {
      stopp();
    }
    if (results.value == 0xFFA857) {
      forward();
      delay (100);
    }
  if (results.value == 0xFFE01F) {
    backwards();
    delay (2000);
  }
  if (results.value == 0xFF02FD) {
    right();
  }
  if (results.value == 0xFF22DD) {
    left();
  }
  receiver.resume();
 }
}