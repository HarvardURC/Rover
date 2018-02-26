#include <RH_RF95.h>
#include <Wire.h>

// Blinky on receipt
#define LED 13

// For I2C
#define SLAVE_ADDRESS 0x04

char stringBuffer[32] = "Default Buffer";
int byteIndex = 0;

void setup() 
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop() {
  int bufIndex = 0;
  bool startingRead = true;

  if (Serial.available()) {
    stringBuffer[0] = '\0';
    
    while(Serial.available()) {
      stringBuffer[bufIndex] = Serial.read();
      bufIndex++;
    }

    Serial.print("Pi now has: ");
    Serial.println(stringBuffer);
  }
}

void receiveData(int byteCount){
  
//  char buf[byteCount];
//  
//  int byteCounter = 0;
//  while(Wire.available()) {
//    buf[byteCounter] = Wire.read();
//    byteCounter++;
//  }

//  Serial.println(buf);
}

void sendData() {  
  Wire.write(stringBuffer[byteIndex]);
  
  byteIndex = byteIndex + 1;
  if (byteIndex >= 32) {
    byteIndex = 0;
  }
}

void requestEvent() {
  
}
