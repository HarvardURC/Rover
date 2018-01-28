#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>

//for feather m0  
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on receipt
#define LED 13

// For I2C
#define SLAVE_ADDRESS 0x04

uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

void setup() 
{
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

//  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Feather LoRa RX Test!");
  
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

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
}

void loop()
{
  
}

void receiveData(int byteCount){
  int counter = 0;
  
  while(Wire.available()) {
    buf[counter] = Wire.read();
    counter++;

    if (counter >= RH_RF95_MAX_MESSAGE_LEN) {
      return;
    }
  }

  rf95.send(buf, sizeof(buf));
  rf95.waitPacketSent();
  Serial.println("Sent a reply");

  for (int i = 0; i < RH_RF95_MAX_MESSAGE_LEN; i++) {
    buf[i] = '\0';
  }
}

void sendData() {
  // Got request for some data
  Wire.write((char *)buf);
}
