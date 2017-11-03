 //Maxon EPOS2

#include <mcp_can.h>
#include <SPI.h>
#include <stdio.h>
#include <SoftwareSerial.h>

//Define datatypes
#define INT8U unsigned char
#define INT32U unsigned long

// Define States
#define Startup        1
#define Homing         2
#define SetupPosCtrl   3
#define PosCtrl1       4
#define PosCtrl2       5
#define Disable        6
#define Button         7
#define ESCON          8

INT8U Flag_Recv = 0;
INT8U len = 0;
INT8U buf[8];
INT8U recieve[8];
INT32U ID=0;
INT32U NodeID=0x601;
INT32U SyncID=0x80;
char str[20];

//Define BT variables
SoftwareSerial bluetooth(3, 4); // RX, TX
int velocity = 5;
int on = 7;
int x = 0;
boolean ledState = HIGH;
String receivedBluetoothString = "";



//CAN messages
unsigned char STARTUP[2] =      {0x01, 0x00}; //CAN open message to change from pre-operational state to operational
unsigned char SYNC[2] =         {0x00, 0x00}; //CAN sync message, after which CANopen devices will respond synchronous,CAN busload can be controlled by Master, in this case its Arduino.

//timing variables
unsigned long t; //timeout within subroutines
unsigned long time; //Time in main loop
unsigned long t_prevCAN; //delta time calculation for CAN update loop
unsigned long t_timein; //start of wait for CAN receive
unsigned long t_timeout = 100; //time out value time for CAN receive message
int t_deltaCAN = 10; //update freq CAN messages.

//define CAN datatypes
struct CANOpenData {
  byte datatype;
  word Index;
  byte SubIndex;
  long data;
} 
CANdata;

union CANdatabuf
{
  CANOpenData data;
  byte bytes [8];
};

struct CANOpen {
  word ID;
  CANdatabuf msg;
} 
CANOpenMsg;

struct CANOpenSetOperational {
  word ID;
  byte bytes[2];
} 
StartUp = {
  0x00, 0x01, 0x00};

struct CANOpenSync {
  word ID;
  byte bytes[2];
} 
Sync = {
  0x80, 0x00, 0x00};

CANOpen ResetFaults =          (CANOpen){0x601, (CANOpenData){0x22, 0x6040, 0x00, 0x80}};

CANOpen PowerDown  =           (CANOpen){0x601, (CANOpenData){0x22, 0x6040, 0x00, 0x06}};
CANOpen PowerOn  =             (CANOpen){0x601, (CANOpenData){0x22, 0x6040, 0x00, 0x0F}};

CANOpen HomingMode  =          (CANOpen){0x601, (CANOpenData){0x22, 0x6060, 0x00, 0x06}};         //Enable Homing Mode
CANOpen StartHoming =          (CANOpen){0x601, (CANOpenData){0x22, 0x6040, 0x00, 0x1F}};         //Start Homing
CANOpen HomingMethod =         (CANOpen){0x601, (CANOpenData){0x22, 0x6098, 0x00, 0xFFFFFFFD}};       //Current Threshold Positive Speed (-3)
CANOpen CurrentThreshold =     (CANOpen){0x601, (CANOpenData){0x22, 0x2080, 0x00, 0x0190}};       //400 mA
CANOpen HomeOffset =           (CANOpen){0x601, (CANOpenData){0x22, 0x607C, 0x00, 0xFFFE7960}};   //-100000 qc
CANOpen ReadStatusOfHoming =   (CANOpen){0x601, (CANOpenData){0x22, 0x6041, 0x00, 0x000A}};       //Read status of bit 10/bit12, home position is reached if these have values of 1

CANOpen PositionMode =          (CANOpen){0x601, (CANOpenData){0x22, 0x6060, 0x00, 0x01}};          //Set Operation Mode to Profile Position Mode value 1
CANOpen ProfileSpeed =          (CANOpen){0x601, (CANOpenData){0x22, 0x6081, 0x00, 0x1F40}};        //8000 rpm
CANOpen PositionSet1  =         (CANOpen){0x601, (CANOpenData){0x22, 0x607A, 0x00, 0xFFFE17B8}};       //Target Position 1 -125000
CANOpen PositionSet2  =         (CANOpen){0x601, (CANOpenData){0x22, 0x607A, 0x00, 0x124F8}};      //Target Position 2 75000
CANOpen PositionDo  =           (CANOpen){0x601, (CANOpenData){0x22, 0x6040, 0x00, 0x1F}};          //Start Movement
CANOpen ReadStatusOfPosition1 = (CANOpen){0x601, (CANOpenData){0x22, 0x6041, 0x00, 0x000A}};          //Read status of bit 10

CANOpen Enable =                (CANOpen){0x601, (CANOpenData){0x22, 0x2071, 0x01, 0x00}};          //Read status of Digital Input 1

//Servodrive states
boolean Fault = false;
boolean PowerstageOn = false;
boolean TargetReached = false;
boolean HomingAttained = false;
boolean HomingError = false;
boolean Halt = false;
byte    ModeOfOperation = 0;

byte State = Button;

MCP_CAN CAN(9); 

void setup(void)
{
  Serial.begin(9600);
  //init BT
  bluetooth.begin(9600);
  pinMode(on, OUTPUT);
  // init can bus, baudrate: 1000k
  if(CAN.begin(CAN_1000KBPS) == CAN_OK){ 
  
  //initialise mask and filters to only recieve message 0x581 and 0x381
    CAN.init_Mask(0,0,0x7FF);//Mask0 all ID bits will be checked
    CAN.init_Mask(1,0,0x7FF);//Mask1 all ID bits will be checked
    
    CAN.init_Filt(0,0,0x581);//Filter 0, Mask0, will receive only this ID
    CAN.init_Filt(1,0,0x7FF);//Filter 1, Mask0, additional ID that is never used
    
    CAN.init_Filt(2,0,0x381);//Filter 2, Mask1, will receive only this ID
    CAN.init_Filt(3,0,0x7FF);//Filter 3, Mask1, additional ID that is never used
    CAN.init_Filt(4,0,0x7FF);//Filter 4, Mask1, additional ID that is never used
    CAN.init_Filt(5,0,0x7FF);//Filter 5, Mask1, additional ID that is never used
    delay(100);
}
}

//********************************************
//               Startup EPOS2
//********************************************
void doStartup(){
  
    //Reset Faults
  CAN.sendMsgBuf(ResetFaults.ID, 0, 8, ResetFaults.msg.bytes);
 
  //Setoperational
  CAN.sendMsgBuf(0x00, 0, 2, STARTUP);
  delay(500);
  
  CAN.sendMsgBuf(PowerDown.ID,0,8,PowerDown.msg.bytes);
  delay(10);
  
  CAN.sendMsgBuf(PowerOn.ID,0,8,PowerOn.msg.bytes);
  delay(10);

State = Homing;
}

//********************************************
//               Disable EPOS2
//********************************************
void doDisable(void){
  CAN.sendMsgBuf(PowerDown.ID,0,8,PowerDown.msg.bytes);
  delay(500);
  Serial.println("Disable");

  x = 0;
  receivedBluetoothString = "";
  State = Button;
}

//********************************************
//               ESCON PWM
//********************************************
void doEscon(void){
while (bluetooth.available() > 0) {

    char receivedBluetoothChar = bluetooth.read();
    receivedBluetoothString += receivedBluetoothChar;

    if (receivedBluetoothChar == '\n') {

      if (receivedBluetoothString.toInt() == 588044) {
        digitalWrite(on, (ledState) ? HIGH : LOW);
          Serial.println("PWM output!!\r\n");
        ledState = !ledState;
      }
    }
    if (receivedBluetoothChar == '\n') {
      if (receivedBluetoothString.endsWith("Slider\n")) {
        analogWrite(velocity , receivedBluetoothString.toInt());
      }
      receivedBluetoothString = "";
    }
  }
  if (on == false) {
    State = Button;
  }
  else if (on == true) {
    State = ESCON;    
  }
}

//********************************************
//               Button Press
//********************************************
void doButton(void){
while (bluetooth.available() > 0) {
  
    char receivedBluetoothChar = bluetooth.read();
    receivedBluetoothString += receivedBluetoothChar;

    if (receivedBluetoothChar == '\n') {
      if (receivedBluetoothString.toInt() == 123) {
        Serial.println(receivedBluetoothString.toInt());
        State = Startup;
      }
      else if (receivedBluetoothString.toInt() == 588044) {
        digitalWrite(on, (ledState) ? HIGH : LOW);
        ledState = !ledState;
        receivedBluetoothString = "";
        Serial.println(receivedBluetoothString.toInt());  
        State = ESCON;    
      }
      }
    }
}

//********************************************
//                CAN message Receive
//********************************************
void CANreceive(void){
    //Wait for message
    t_timein = millis();
    while(CAN.checkReceive()==4){ // 3=Message available, 4 = No message available, as difined in mcp_can_dfs.h form CAN_BUS_SHIELD lib from SeeedStudio
        //Check for timeout
        if ((millis()-t_timein) > t_timeout) return;
        //Check button states?
        //Update leds?
    }
    
    if(Flag_Recv>=0)                   // check if get data via Interupt line
    {
      Flag_Recv = 0;                // clear flag
      CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
      ID = CAN.getCanId();
      
      switch (ID) {
        case 0x381:
          Fault = bitRead(buf[0], 3);
          PowerstageOn = bitRead(buf[0], 4);
          Halt = bitRead(buf[0], 6);
          TargetReached = bitRead(buf[1], 2);
          HomingAttained = bitRead(buf[1], 4);
          HomingError = bitRead(buf[1], 5);
          break;
        case 0x481:
          ModeOfOperation = buf[0];
          break;
        //default: 
          // if nothing else matches, do the default
          // default is optional
      }
    }
}

//****************************************************************
//    Homing
//****************************************************************
void doHoming(void){

//Change Mask
  CAN.init_Filt(0,0,0x581); // responce from command
  CAN.init_Filt(2,0,0x481); // only wait for status message 0x481
  delay(10);  
  
//Select Homing Mode
  CAN.sendMsgBuf(HomingMode.ID, 0, 8, HomingMode.msg.bytes);
  CANreceive(); // wait for 581
  t = millis();
  
//change MASK
  CAN.init_Filt(0,0,0x581); // responce from command
  CAN.init_Filt(2,0,0x381); // only wait for status message 0x381
  delay(10);
 
//Select HomingMethod
  CAN.sendMsgBuf(HomingMethod.ID, 0, 8, HomingMethod.msg.bytes);
  CANreceive(); // wait for 581
  t = millis();

//Select HomingMethod
  CAN.sendMsgBuf(HomeOffset.ID, 0, 8, HomeOffset.msg.bytes);
  CANreceive(); // wait for 581
  t = millis();

//Select HomingMethod
  CAN.sendMsgBuf(CurrentThreshold.ID, 0, 8, CurrentThreshold.msg.bytes);
  CANreceive(); // wait for 581
  t = millis();

//  Poweron
  CAN.sendMsgBuf(PowerOn.ID,0,8,PowerOn.msg.bytes);
  CANreceive(); // wait for 581
  t = millis();
         while (!PowerstageOn){
    CAN.sendMsgBuf(Sync.ID, 0, 2, Sync.bytes);
    CANreceive(); // wait for 381, PowerstageOn == 1;
    if ((millis()-t) > 1000) {
      State = Startup;
      return; // if not within 10 second homed, start again.
    }
  }
  
CAN.sendMsgBuf(StartHoming.ID, 0, 8, StartHoming.msg.bytes);
  CANreceive(); // wait for 581
  t = millis();
  
 HomingAttained = false;
 TargetReached = false;
  while (!HomingAttained && !TargetReached){ //&& HomingTime < 10sec
    CAN.sendMsgBuf(Sync.ID, 0, 2, Sync.bytes);  
    CANreceive(); // wait for 381, TargetReached == 1  && HomingAttained == 1;
    if ((millis()-t) > 60000) {
      State = Startup;
      return;   
  }
} 
  State = SetupPosCtrl;
}

//****************************************************************
//    Setup Position Control
//****************************************************************

void setupPositionControl(void){

//Change Mask
  CAN.init_Filt(0,0,0x581); // responce from command
  CAN.init_Filt(2,0,0x481); // only wait for status message 0x481
  delay(10);  
//Select PositionControlMode
  CAN.sendMsgBuf(PositionMode.ID, 0, 8, PositionMode.msg.bytes);
  CANreceive(); // wait for 581
  t = millis();

//change MASK
  CAN.init_Filt(0,0,0x581); // responce from command
  CAN.init_Filt(2,0,0x381); // only wait for status message 0x381
  delay(10);  
  
//SetProfileVelocity
  CAN.sendMsgBuf(ProfileSpeed.ID, 0, 8, ProfileSpeed.msg.bytes);
  CANreceive(); // wait for 581
  delay(10);
  
//Change State
  State = PosCtrl1;
}

//****************************************************************
//    Play Profile 1
//****************************************************************
void doProfile1(void){

      CAN.sendMsgBuf(PositionSet1.ID, 0, 8, PositionSet1.msg.bytes);
      CANreceive(); // wait for 581
      t = millis();
      
    //Execute Setpoint
      CAN.sendMsgBuf(PositionDo.ID, 0, 8, PositionDo.msg.bytes);
      CANreceive(); // wait for 581
      t = millis();

      TargetReached = false;
      while (!TargetReached){ //&& MovementTime < 13sec
      CAN.sendMsgBuf(Sync.ID, 0, 2, Sync.bytes);
      CANreceive(); // wait for 381, TargetReached == 1;
          if ((millis()-t) > 20000) {
          State = PosCtrl1;
          return;
          }
       }     
State = PosCtrl2;
}

//****************************************************************
//    Play Profile2
//****************************************************************
void doProfile2(void){

      CAN.sendMsgBuf(PositionSet2.ID, 0, 8, PositionSet2.msg.bytes);
      CANreceive(); // wait for 581
      t = millis();
      
    //Execute Setpoint
      CAN.sendMsgBuf(PositionDo.ID, 0, 8, PositionDo.msg.bytes);
      CANreceive(); // wait for 581
      t = millis();
        
        TargetReached = false;
        while (!TargetReached){ //&& MovementTime < 13sec
        CAN.sendMsgBuf(Sync.ID, 0, 2, Sync.bytes);
        CANreceive(); // wait for 381, TargetReached == 1;
          if ((millis()-t) > 20000) {    
          State = PosCtrl2;
          return;
          }
      }
    x++;
    State = PosCtrl1;
if (x >= 3) {
  State = Disable;
}
}

void loop() {
 
time = millis();
    //if its time, send and receive CAN messages
  if ((time - t_prevCAN) >= t_deltaCAN){
    t_prevCAN = time;
    
      //Statemachine based on switch case   
    switch (State) {

      case Button:
        doButton();
        break;

      case ESCON:
        doEscon();
        break;
        
      case Startup:
        doStartup();
        break;

       case Homing:
        doHoming();
        break;
               
      case SetupPosCtrl:
        setupPositionControl();
        break;
  
      case PosCtrl1:
        doProfile1();
        break;
        
      case PosCtrl2:
       doProfile2();
        break;

      case Disable:
        doDisable();
        break;
    }
}
}

  
