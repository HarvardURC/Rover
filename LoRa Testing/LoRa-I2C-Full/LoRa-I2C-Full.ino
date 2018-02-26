#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>

//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

#define LED 13
#define SLAVE_ADDRESS 0x04

char messageBuffer[32];
int byteIndex = 0;

void setup() 
{
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  delay(100);
  
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  rf95.init();
  rf95.setFrequency(RF95_FREQ);
  rf95.setTxPower(23, false);

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop()
{
  uint8_t loraBuffer[32];
  uint8_t len = 32;
  
  if (rf95.available()) {
    
    if (rf95.recv(loraBuffer, &len)) {
      memcpy(messageBuffer, loraBuffer, 32);
      delay(200);
      uint8_t data[] = "Confirm Packet Reciept!";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
    }
    
    else {
      Serial.println("Receive failed");
    }
  }
}

void receiveData(int byteCount){
  
  while (Wire.available()) {
    int c = Wire.read();

    // note - this can cause some problems w/ more than one number in the future!!
    if (c = 100) {
      byteIndex = 0;
    }
  }
}

void sendData() {
  Wire.write(messageBuffer[byteIndex]);
  
  byteIndex = byteIndex + 1;
  if (byteIndex >= 32) {
    byteIndex = 0;
  }
}
