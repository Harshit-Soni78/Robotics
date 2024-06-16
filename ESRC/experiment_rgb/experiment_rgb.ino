int const red = 3;
int const green = 5;
int const blue = 6;
void setup() {
  pinMode(red , OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue , OUTPUT);  
}

void loop() {
  int r,g,b;
  for(r=0 ; r<=255 ; r++){
    analogWrite(red , r);
    
      for(g=0 ; g<=255 ; g++){
             analogWrite(green , g);
    
                     for(b=0 ; b<=255 ; b++){
                           analogWrite(blue , b);
                                              }
                                }
                            }
              }
