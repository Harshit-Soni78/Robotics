#include <SoftwareSerial.h>
int enA = 9;
int enB = 10;
//int motorspeed1 = 50;

SoftwareSerial bluetooth(0, 1);  

int leftMotorPin1 = 8; 
int leftMotorPin2 = 7;   
int rightMotorPin1 = 6;  
int rightMotorPin2 = 5; 

void setup() {
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  bluetooth.begin(9600);
}
int motorspeed1;
      
void loop() {
  if (bluetooth.available()) {
    char command = bluetooth.read();
    bluetooth.println(command);
    bluetooth.println(motorspeed1);

        switch (command) {
          case 0:
          motorspeed1=150;
          break;
      case '1':
      motorspeed1=160;
      break;
      case '2':
      motorspeed1=170;
      break;
      case '3':
      motorspeed1=180;
      break;
      case '4':
      motorspeed1=190;
      break;
      case '5':
      motorspeed1=200;
      break;
      case '6':
      motorspeed1=210;
      break;
      case '7':
      motorspeed1=230;
      break;
      case '8':
      motorspeed1=240;
      break;
      case '9':
      motorspeed1=250;
      break;
      case 'q' :motorspeed1=254;
      break;
      }

    switch (command) {
      case 'F':  // Move forward
        moveForward(motorspeed1);
        break;
      case 'B':  // Move backward
        moveBackward(motorspeed1);
        break;
      case 'L':  // Turn left
        turnLeft(motorspeed1);
        break;
      case 'R':  // Turn right
        turnRight(motorspeed1);
        break;
      case 'S':  // Stop
        stop(motorspeed1);
        break;
      case 'G':
        moveForwardleft(motorspeed1);
        break;
      case 'I':
        moveForwardright(motorspeed1);
        break;

      case 'H':
      moveBackwardleft(motorspeed1);
      break;

      case 'J':

       moveBackwardright(motorspeed1);
       break;
        
       
    }
  }
}

void moveForward(int motorspeed ) {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(enA, motorspeed);
  analogWrite(enB, motorspeed);
}

void moveBackward(int motorspeed) {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
    analogWrite(enA, motorspeed);
  analogWrite(enB, motorspeed);
}

void turnLeft(int motorspeed) {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
    analogWrite(enA, motorspeed);
  analogWrite(enB, motorspeed);
}

void turnRight(int motorspeed) {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
    analogWrite(enA, motorspeed);
  analogWrite(enB, motorspeed);
}

void stop(int motorspeed) {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, LOW);
}

void moveForwardleft(int motorspeed) {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(enA, (motorspeed)/3);
  analogWrite(enB, (motorspeed));
}


void moveForwardright(int motorspeed) {
  digitalWrite(leftMotorPin1, HIGH);
  digitalWrite(leftMotorPin2, LOW);
  digitalWrite(rightMotorPin1, HIGH);
  digitalWrite(rightMotorPin2, LOW);
  analogWrite(enA, (motorspeed));
  analogWrite(enB, (motorspeed)/3);
}


void moveBackwardleft(int motorspeed) {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
    analogWrite(enA, (motorspeed)/3);
  analogWrite(enB, (motorspeed));

}


void moveBackwardright(int motorspeed) {
  digitalWrite(leftMotorPin1, LOW);
  digitalWrite(leftMotorPin2, HIGH);
  digitalWrite(rightMotorPin1, LOW);
  digitalWrite(rightMotorPin2, HIGH);
  analogWrite(enA, (motorspeed));
  analogWrite(enB, (motorspeed)/3);
}


