/*Q4 
Declare two int variables A and B in the program (initialize between 0 and 4). The sum of A and B can goto 
another variable C. Display C on Seven Segment display.
Hardcoding the answer is not allowed.
Write a function and complete program (execute it in Arduino, this will give you confidence!
*/
void setup()  
{  
  pinMode(1,OUTPUT); 
  pinMode(2,OUTPUT); 
  pinMode(3,OUTPUT); 
  pinMode(4,OUTPUT); 
  pinMode(5,OUTPUT); 
  pinMode(6,OUTPUT); 
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);  
  Serial.print("Enter the numbers :-  \n");

}  
void loop() {  
  int a =3;
  int b=4;
  int c;
  c = a+b;
  displayNumber(c);  
  delay(1000); // time delay of 1 second or 1000 milliseconds  
}  

void displayNumber(int c)
{
 if(c<0||c>9)
   {
        digitalWrite(1,0);
        digitalWrite(2,0);
        digitalWrite(3,0);
        digitalWrite(4,0);
        digitalWrite(5,0);
        digitalWrite(6,0);
        digitalWrite(7,0);
        digitalWrite(8,1);
   }
   else
          switch(c)
          {
            case 0: 
            digitalWrite(1,0);
            digitalWrite(2,1);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,1);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 1: 
            digitalWrite(1,0);
            digitalWrite(2,0);
            digitalWrite(3,0);
            digitalWrite(4,1);
            digitalWrite(5,0);
            digitalWrite(6,0);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 2: 
            digitalWrite(1,1);
            digitalWrite(2,0);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,1);
            digitalWrite(6,1);
            digitalWrite(7,0);
            digitalWrite(8,0);
             break;
             case 3: 
            digitalWrite(1,1);
            digitalWrite(2,0);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,0);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 4: 
            digitalWrite(1,1);
            digitalWrite(2,1);
            digitalWrite(3,0);
            digitalWrite(4,1);
            digitalWrite(5,0);
            digitalWrite(6,0);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 5: 
            digitalWrite(1,1);
            digitalWrite(2,1);
            digitalWrite(3,1);
            digitalWrite(4,0);
            digitalWrite(5,0);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 6: 
            digitalWrite(1,1);
            digitalWrite(2,1);
            digitalWrite(3,1);
            digitalWrite(4,0);
            digitalWrite(5,1);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 7: 
            digitalWrite(1,0);
            digitalWrite(2,0);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,0);
            digitalWrite(6,0);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 8: 
            digitalWrite(1,1);
            digitalWrite(2,1);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,1);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 9: 
            digitalWrite(1,1);
            digitalWrite(2,1);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,0);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
          }
}


