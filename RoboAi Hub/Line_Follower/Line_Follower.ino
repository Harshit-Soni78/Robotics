#define IR_SENSOR_RIGHT 11
#define IR_SENSOR_LEFT 12
#define MOTOR_SPEED 180

//Right motor
// int enableRightMotor=6;
int rightMotorPin1=7;
int rightMotorPin2=8;

//Left motor
// int enableLeftMotor=5;
int leftMotorPin1=9;
int leftMotorPin2=10;

void setup()
{
  // put your setup code here, to run once:
  // pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  
  // pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  rotateMotor(0,0);   
}


void loop()
{

  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

  //If none of the sensors detects black line, then go straight
  if (rightIRSensorValue == HIGH && leftIRSensorValue == HIGH)
  {
    rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
  }
  //If right sensor detects black line, then turn right
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
  {
      rotateMotor(-MOTOR_SPEED, MOTOR_SPEED); 
  }
  //If left sensor detects black line, then turn left  
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
  {
      rotateMotor(MOTOR_SPEED, -MOTOR_SPEED);
  } 
  //If both the sensors detect black line, then stop 
  else 
  {
    rotateMotor(0, 0);
  }
}


void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  /////////////////////////////////////////////////
  if (rightMotorSpeed < 0)  // back - right motor
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0) // forward - right motor
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else // stop - right motor
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }
  /////////////////////////////////////////////////

  if (leftMotorSpeed < 0) // back left motor
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0) // forward left motor
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else // stop left motor
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }
  /////////////////////////////////////////////////
  // analogWrite(enableRightMotor, abs(rightMotorSpeed));
  // analogWrite(enableLeftMotor, abs(leftMotorSpeed));    
}





















