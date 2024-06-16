const int RED = 3;
const int YELLOW = 4;
const int GREEN = 5;

void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
}
char c;
void loop() {
  digitalWrite(RED, 0);
  digitalWrite(YELLOW, 0);
  digitalWrite(GREEN, 0);

  Serial.println("Enter which traffic light you want to glow: \nR for Red\nY for Yellow\nG for Green");
  while (Serial.available() == 0) { ; }
  c = Serial.read();
  switch (c) {
    case 'R':
      digitalWrite(RED, 1);
      digitalWrite(GREEN, 0);
      digitalWrite(YELLOW, 0);
      delay(5000);
      break;
    case 'Y':
      digitalWrite(RED, 0);
      digitalWrite(YELLOW, 1);
      digitalWrite(GREEN, 0);
      delay(5000);
      break;
    case 'G':
      digitalWrite(RED, 0);
      digitalWrite(GREEN, 1);
      digitalWrite(YELLOW, 0);
      delay(5000);
      break;
  }
}