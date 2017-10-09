#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(12,13); //RX,TX
char incoming;
char outgoing = '4';
int count = 0;
int start = 0;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  mySerial.begin(4800);
  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  /*
  if(start==0){
    while(mySerial.available()>0){
      incoming = mySerial.read();
      if(incoming!=0) count++;
      Serial.print(count);
      if(count>200){
        mySerial.write(outgoing);
        start = 1;
        break;
      }
    }
  }
  else{
      if(mySerial.available()>0){
        incoming = mySerial.read();
        if(incoming!=0) Serial.print(incoming);//Wire.write(incoming);
      }
  }*/
}

void requestEvent(){
  if(start==0){
    while(mySerial.available()>0){
      incoming = mySerial.read();
      if(incoming!=0) count++;
      //Serial.print(count);
      if(count>200){
        mySerial.write(outgoing);
        start = 1;
        break;
      }
    }
  }
  else{
      if(mySerial.available()>0){
        incoming = mySerial.read();
        if(incoming!=0) Wire.write(incoming);
      }
  }
}

void receiveEvent(){
  while(Wire.available()>0){
    char c = Wire.read();
    mySerial.write(c);
  }
}

