      // Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include <SPI.h>
#include <RH_RF95.h>

//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
#define LED 13

bool haveDataToSend = false;

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  pinMode(LED, OUTPUT); 
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

//  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Feather LoRa TX Test!");

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

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop()
{
  char inData[20];
  char inChar;
  byte index = 0;

  while(Serial.available() > 0) // Don't read unless
  {
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      if(index < 19) // One less than the size of the array
      {
          inChar = Serial.read();
          inData[index] = inChar;
          index++;
          inData[index] = '\0';
      }

      haveDataToSend = true;
  }

  if (haveDataToSend) {
    Serial.println("Sending to rf95_server");
  
    Serial.print("Sending "); Serial.println(inData);    
    Serial.println("Sending..."); delay(10);

    rf95.send((uint8_t *)inData, 20);
    Serial.println("Waiting for packet to complete..."); delay(10);
    rf95.waitPacketSent();
    
    // Now getting a reply
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    Serial.println("Waiting for reply..."); delay(10);
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

    else {
      Serial.println("No Reply. Cannot confirm receipt.");
    }

    haveDataToSend = false;
  }
}
    
