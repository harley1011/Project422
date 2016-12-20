int motionSenorPin = 10;

void setup() {
  Serial.begin(4800);
  int randomNumber = random(8);
  pinMode(motionSenorPin, INPUT);
}

void loop() {
  int val = digitalRead(motionSenorPin);
  Serial.println(val);
  delay(2000);
}


