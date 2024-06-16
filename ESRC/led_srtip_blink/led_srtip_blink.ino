void setup() {
  // put your setup code here, to run once:

   const int LED_Green = 2;
   const int LED_yellow =3;
   const int LED_red =4;
   const int LED_white = 5;
   const int LED_Blue =6 ;

    
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  delay(1000);    
      digitalWrite(2, 1);
      digitalWrite(3, 0);
      digitalWrite(4, 0);
      digitalWrite(5, 0);
      digitalWrite(6, 0);  
  delay(1000);   
      digitalWrite(2, 0);
      digitalWrite(3, 1);
      digitalWrite(4, 0);
      digitalWrite(5, 0);
      digitalWrite(6, 0);
  delay(1000);          
      digitalWrite(2, 0);
      digitalWrite(3, 0);
      digitalWrite(4, 1);
      digitalWrite(5, 0);
      digitalWrite(6, 0);
  delay(1000);          
      digitalWrite(2, 0);
      digitalWrite(3, 0);
      digitalWrite(4, 0);
      digitalWrite(5, 1);
      digitalWrite(6, 0);
  delay(1000);      
      digitalWrite(2, 0);
      digitalWrite(3, 0);
      digitalWrite(4, 0);
      digitalWrite(5, 0);
      digitalWrite(6, 1);  
      digitalWrite(2, 0);
      digitalWrite(3, 0);
      digitalWrite(4, 0);
      digitalWrite(5, 0);
  delay(1000);     
}
