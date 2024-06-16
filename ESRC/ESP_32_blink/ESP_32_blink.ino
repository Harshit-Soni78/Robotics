#define LED_BUILTIN 4
// pin no 4 for flash light
// pin no 33 and 22 for normal red led
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(500);                      
  digitalWrite(LED_BUILTIN, LOW);   
  delay(1000);                      
}
