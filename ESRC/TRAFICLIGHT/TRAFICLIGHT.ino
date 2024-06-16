void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);

}



void loop() {
  digitalWrite(13, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(2000);                      // wait for a second
  digitalWrite(12, HIGH);   // turn the LED off by making the voltage LOW
  delay(2000);    
  digitalWrite(11, HIGH);
  delay(2000);
  digitalWrite(10, LOW);   // turn the LED off by making the voltage LOW
  delay(2000); 
  digitalWrite(9, HIGH);   // turn the LED off by making the voltage LOW
  delay(2000);     
  digitalWrite(8, LOW);   // turn the LED off by making the voltage LOW
  delay(2000);      
 