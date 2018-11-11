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
  mcp2515_bit_modify(CANCTRL, (1<<REQOP2)|(1<<REQOP1)|(1<<REQOP0), 0);

  for (int i = 1; i < 7; i++) {
  	sendVelocityCommand(i);
  	delay(30);
  }

  for (int i = 1; i < 7; i++) {
  	commitVelocity(i);
  	delay(30);
  }
  

  while(1);
}

//********************************Helper Methods*********************************//

//Sets velocity to 1000
void sendVelocityCommand(int nodeID) {
  tCAN message;
  message.id = 0x600 + nodeID;
  message.header.rtr = 0;
  message.header.length = 8;
  uint8_t dataArray[] = {34,255,96,0,232,3,0,0};
  memcpy(message.data, dataArray, 8 * sizeof(uint8_t));
  mcp2515_send_message(&message);
}

//Commits velocity
void commitVelocity(int nodeID) {
  tCAN message;
  message.id = 0x600 + nodeID; //formatted in HEX
  message.header.rtr = 0;
  message.header.length = 8; //formatted in DEC
  uint8_t velArray[] = {34,64,96,0,15,0,0,0};
  memcpy(message.data, velArray, 8 * sizeof(uint8_t));
  mcp2515_send_message(&message);
}
