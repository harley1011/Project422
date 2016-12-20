int motionSenorPin = 10;
int lightPin = 0;

void setup() {
  Serial.begin(4800);
  int randomNumber = random(8);
  pinMode(motionSenorPin, INPUT);
}

void loop() {
  int val = readPhoto();
  //Serial.println(val);
  delay(2000);
}

int readPhoto() {
  Serial.println(analogRead(lightPin));
  return 0;
}

