#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_Sensor.h>

//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 915.0
#define LED 13

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  delay(100);
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  
  rf95.setTxPower(23, false);
}

void loop()
{
  if (rf95.available()) {
    
    sensors_event_t event;
    uint8_t len = sizeof(event);
    uint8_t buf[len];
    
    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);

      memcpy(&event, buf, len);

      Serial.print("X: ");
      Serial.print(event.orientation.x, 4);
      Serial.print("\tY: ");
      Serial.print(event.orientation.y, 4);
      Serial.print("\tZ: ");
      Serial.print(event.orientation.z, 4);
      Serial.println("");
      
      delay(10);
      
      digitalWrite(LED, LOW);
    }
    
    else {
      Serial.println("Receive failed");
    }
  }
}
