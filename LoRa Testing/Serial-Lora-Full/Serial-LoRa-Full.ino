#include <SPI.h>
#include <RH_RF95.h>

//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 915.0
#define LED 13

bool haveDataToSend = false;

RH_RF95 rf95(RFM95_CS, RFM95_INT);

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
}

uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

void loop()
{
  int index = 0;
  char serialBuf[32];
  
  if (Serial.available()) {
    
    while(Serial.available()) {
      serialBuf[index] = Serial.read();
      index++;
    }

    while (index < 30) {
      serialBuf[index] = '_';
      index++;
    }

    serialBuf[index] = '\0';

    Serial.print("Sending "); Serial.println(serialBuf);
    rf95.send((uint8_t *)serialBuf, 32);

    rf95.waitPacketSent();

    if (rf95.waitAvailableTimeout(1000)) { 
      if (rf95.recv(buf, &len)) {
        Serial.print("Got reply: ");
        Serial.println((char*)buf);
        Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);    
      }

      else {
        Serial.println("Receive failed");
      }
    }
  }
}
    
