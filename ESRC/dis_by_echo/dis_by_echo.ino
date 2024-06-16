int trig = 13;
int echo = 12;
float duration ;
float dis_inch , dis_cm ;
void setup() {
  // put your setup code here, to run once:
      Serial.begin(9600);
      pinMode(trig , OUTPUT);
      pinMode(echo , INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
      digitalWrite(trig , HIGH);
      delayMicroseconds(10);
      digitalWrite(trig , LOW);
      duration = pulseIn(echo , 1)/2;
      dis_inch = duration /74;
      dis_cm = duration / 29;
      Serial.print(dis_inch);
      Serial.println(" inch ");
      Serial.print(dis_cm);
      Serial.println(" cm  ");
      Serial.println(" ");
      delay(1000);
}
