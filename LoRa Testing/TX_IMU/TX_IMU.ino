//COMMS
#include <SPI.h>
#include <RH_RF95.h>

//IMU
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  pinMode(13, OUTPUT);
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

  bno.begin();
  delay(500);
  bno.setExtCrystalUse(true);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop()
{ 
  digitalWrite(13, HIGH);
  sensors_event_t event;
  bno.getEvent(&event);
  
  size_t bufSize = sizeof(event);
  uint8_t buf[bufSize];
  memcpy(buf, &event, bufSize);

  rf95.send(buf, bufSize);
  rf95.waitPacketSent();
  
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}
    
