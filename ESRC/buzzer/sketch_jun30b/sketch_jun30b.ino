const int buzzer = 9;
void setup() {
  // put your setup code here, to run once:
      pinMode(buzzer , OUTPUT );
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(buzzer , 1);
    delay(500);
    digitalWrite(buzzer , 0);
    delay(500);
    
}
