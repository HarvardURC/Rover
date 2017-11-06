/****************************************************************************
CAN Write Demo for the SparkFun CAN Bus Shield. 

Written by Stephen McCoy. 
Original tutorial available here: http://www.instructables.com/id/CAN-Bus-Sniffing-and-Broadcasting-with-Arduino
Used with permission 2016. License CC By SA. 

Distributed as-is; no warranty is given.
*************************************************************************/

#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>

//********************************Setup Loop********xw*************************//

void setup() {
  Serial.begin(9600);
  Serial.println("CAN Write - Testing transmission of CAN Bus messages");
  delay(1000);
  
  if(Canbus.init(CANSPEED_125))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");
    
  delay(1000);
}

//********************************Main Loop*********************************//

void loop() 
{

  uint8_t dataArray[] = {34,255,96,0,232,3,0,0};
  
  //Motor 1
  tCAN message1;
  message1.id = 0x601; //formatted in HEX
  message1.header.rtr = 0;
  message1.header.length = 8; //formatted in DEC

  tCAN message2;
  message2.id = 0x602; //formatted in HEX
  message2.header.rtr = 0;
  message2.header.length = 8; //formatted in DEC

  tCAN message3;
  message3.id = 0x603; //formatted in HEX
  message3.header.rtr = 0;
  message3.header.length = 8; //formatted in DEC

  tCAN message4;
  message4.id = 0x604; //formatted in HEX
  message4.header.rtr = 0;
  message4.header.length = 8; //formatted in DEC

  tCAN message5;
  message5.id = 0x605; //formatted in HEX
  message5.header.rtr = 0;
  message5.header.length = 8; //formatted in DEC

  tCAN message6;
  message6.id = 0x606; //formatted in HEX
  message6.header.rtr = 0;
  message6.header.length = 8; //formatted in DEC
  
  mcp2515_bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);

  uint8_t velArray[] = {34,64,96,0,15,0,0,0};
  
  tCAN setVelocity;
  setVelocity.id = 0x00; //formatted in HEX
  setVelocity.header.rtr = 0;
  setVelocity.header.length = 8; //formatted in DEC

  memcpy(message6.data, dataArray, 8*sizeof(uint8_t) );
  memcpy(message1.data, dataArray, 8*sizeof(uint8_t) );
  memcpy(message2.data, dataArray, 8*sizeof(uint8_t) );
  memcpy(message3.data, dataArray, 8*sizeof(uint8_t) );
  memcpy(message4.data, dataArray, 8*sizeof(uint8_t) );
  memcpy(message5.data, dataArray, 8*sizeof(uint8_t) );
  memcpy(setVelocity.data, velArray, 8*sizeof(uint8_t) );
  
  mcp2515_send_message(&message3);
  mcp2515_send_message(&message2);
  mcp2515_send_message(&message5);

  delay(30);
  
  setVelocity.id = 0x603;
  mcp2515_send_message(&setVelocity);

  setVelocity.id = 0x602;
  mcp2515_send_message(&setVelocity);

  setVelocity.id = 0x605;
  mcp2515_send_message(&setVelocity);
  
  delay(1000);
  
  mcp2515_send_message(&message1);
  mcp2515_send_message(&message4);
  mcp2515_send_message(&message6);

  delay(30);

  setVelocity.id = 0x601;
  mcp2515_send_message(&setVelocity);

  setVelocity.id = 0x604;
  mcp2515_send_message(&setVelocity);

  setVelocity.id = 0x606;
  mcp2515_send_message(&setVelocity);

  while(1);
}
