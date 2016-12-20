int motionSenorPin = 10;

void setup() {
  Serial.begin(4800);
  //int randomNumber = random(8);
  pinMode(motionSenorPin, INPUT);
  char data[6];
  generateID(data);
}

void loop() {
  int val = digitalRead(motionSenorPin);
  Serial.println(val);
  delay(2000);
}

void generateID(char* data){
  char idRequest[6];
  char idReceive[6];

  idRequest[0]='0';
  idRequest[1]='0';
  idRequest[2]='a';
  idRequest[3]='b';
  idRequest[4]='c';
  idRequest[5]='d';

  /*uint32_t temp = random(1000000000);
  idRequest[5]=(temp>>24) & 0xFF;
  idRequest[4]=(temp>>16) & 0xFF;
  idRequest[3]=(temp>>8) & 0xFF;
  idRequest[2]=temp & 0xFF;*/

  Serial.println(idRequest);

  while(1){
    for(int i=0; i<6; i++)
    {
      int temp = - 1;
      while (temp == -1)
        temp = Serial.read();
      idReceive[i] = temp;
    }
    /*if(idReceive[2]==idRequest[2]&&idReceive[3]==idRequest[3]
      &&idReceive[4]==idRequest[4]&&idReceive[5]==idRequest[5]){
      data[0]=idReceive[0];
      Serial.println(idReceive);
      Serial.println(data);
      break;*/
      for(int i=0;i<6;i++)
        Serial.print(idReceive[i]);
  }

}

