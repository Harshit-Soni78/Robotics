/*
Q7 Ask the user to enter a ONE digit number,
Ask the user to enter another ONE digit number
Print the sum of two numbers on serial terminal
ALSO display the sum on 7-segment display (SSD).
Use the function void displayNumber(int n)
[If you have not developed this function, do it now, it will be used in other assignments as well]
NOTE:
If the number is <= 9 then display on SSD
Else blank the display (lit the dot 'dp') to indicate an over flow.
HINT 1:
// Here num1, num2 and sum are integers
// a and b are characters.
read a from Arduino Serial Monitor
num1 = a -'0'; // Serial comm. only read/write chars, so convert ASCII char to number
read b from Arduino Serial Monitor
num2 = b -'0'; // Serial comm. only read/write chars, so convert ASCII char to number
sum = num1 + num2;
if(sum <= 9)
Simply display
else
 Special case
*/
char a=0;
char b=0;
char c=0;
void setup()  
{  Serial.begin(9600); 
  pinMode(2,OUTPUT); 
  pinMode(3,OUTPUT); 
  pinMode(4,OUTPUT); 
  pinMode(5,OUTPUT); 
  pinMode(6,OUTPUT); 
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);  
  pinMode(9,OUTPUT);
}  
void loop() { 
  Serial.print("Enter First numbers :-  \n");
  a=GetChar();
  Serial.println(a);
  Serial.print("Enter Second number. \n");
  b=GetChar();
  Serial.println(b);
  c = a + b - 96;
  displayNumber(c);  
  delay(1000);
}  
//////////////////////////////////////////////////////////////////////////////////////////
void displayNumber(char c)
{
 if(c<0||c>9)
   {
        digitalWrite(9,0);
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
            digitalWrite(9,0);
            digitalWrite(2,1);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,1);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 1: 
            digitalWrite(9,0);
            digitalWrite(2,0);
            digitalWrite(3,0);
            digitalWrite(4,1);
            digitalWrite(5,0);
            digitalWrite(6,0);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 2: 
            digitalWrite(9,1);
            digitalWrite(2,0);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,1);
            digitalWrite(6,1);
            digitalWrite(7,0);
            digitalWrite(8,0);
             break;
             case 3: 
            digitalWrite(9,1);
            digitalWrite(2,0);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,0);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 4: 
            digitalWrite(9,1);
            digitalWrite(2,1);
            digitalWrite(3,0);
            digitalWrite(4,1);
            digitalWrite(5,0);
            digitalWrite(6,0);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 5: 
            digitalWrite(9,1);
            digitalWrite(2,1);
            digitalWrite(3,1);
            digitalWrite(4,0);
            digitalWrite(5,0);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 6: 
            digitalWrite(9,1);
            digitalWrite(2,1);
            digitalWrite(3,1);
            digitalWrite(4,0);
            digitalWrite(5,1);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 7: 
            digitalWrite(9,0);
            digitalWrite(2,0);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,0);
            digitalWrite(6,0);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 8: 
            digitalWrite(9,1);
            digitalWrite(2,1);
            digitalWrite(3,1);
            digitalWrite(4,1);
            digitalWrite(5,1);
            digitalWrite(6,1);
            digitalWrite(7,1);
            digitalWrite(8,0);
             break;
             case 9: 
            digitalWrite(9,1);
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
char GetChar(void){
  while(Serial.available()==0);
  return Serial.read();
}