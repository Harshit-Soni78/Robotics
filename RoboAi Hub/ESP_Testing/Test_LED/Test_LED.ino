#define LED1 D1 //Led in NodeMCU at pin D1
#define LED2 D2 //Led in NodeMCU at pin D2 

void setup() 
{
 pinMode(LED1, OUTPUT);   //LED1 D1 pin as output 
 pinMode(LED2, OUTPUT);   //LED2 D5 pin as output 
}
 void loop() // runs/loops below code infinitely
{
 digitalWrite(LED1, HIGH); //turn on LED
 delay(300); 
 digitalWrite(LED1, LOW); //turn off LED
 delay(300);
 digitalWrite(LED2, HIGH); //turn on LED
 delay(300);            
 digitalWrite(LED2, LOW); //turn off LED
 delay(300); 
//delay defines duration in ms for which LED is on/off or in actual way, its //the duration for which program waits here on this line of code 
}