#include <Servo.h>
Servo myServo;

void setup() {
  // put your setup code here, to run once:
          myServo.attach(9);

}
int pos;
void loop() {
  // put your main code here, to run repeatedly:
      for(pos = 0 ;pos<=180; pos++ ){
        myServo.write(pos);
        delay(10);
      }
      for(pos=180; pos>=0 ; pos--){
        myServo.write(pos);
        delay(10);
      }
}
