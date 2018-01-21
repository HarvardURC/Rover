/*
 * EposLibTest.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: Matthew Giles & John Alex Keszler
 */

#include "Definitions.h"
#include "EposDriveTrain.h"
#include <unistd.h>

using namespace std;

//time to wait in ms
long sleepTime = 3000;
int MODVALUE = 176128;
int offsetvalue1 = 15105;
int offsetvalue2 = 15105;
int offsetvalue3 = 15105;
int offsetvalue4 = 15105;
int offsetvalue5 = 15105;
int offsetvalue6 = 15105;

int state = 1;
int legStates[6] = {0,0,0,0,0,0};
double legAngles [6];

const double pi = 3.14159;
const double legAirSpeed = 7.0*100;
// 20 degrees before home pos
const double landingAngle = 0.349*1.5;
const double legGroundSpeed = legAirSpeed *(2*landingAngle/(2*pi - 2*landingAngle));

  int getActualID(int willsStupidNumbering)
  {
    switch(willsStupidNumbering) {
        case 0 : return 4; 
        case 1 : return 6;
        case 2 : return 2; 
        case 3 : return 1;
        case 4 : return 5; 
        case 5 : return 3;
    }
  }


int main () {
  //Declare new drive train object. The constructor accepts a USB port name as a string
  //USB0 is typically the device that the motor controller connects to
  EposDriveTrain driveTrain("USB0");

  cout << "Initializing..." << endl;

  //Initialize drive train object
  //Note: this object must be initialized by calling init() before trying to move any motors!
  driveTrain.init();

  //Enable all nodes, and clear any faults
  //Note: before a motor can be moved, it must be enabled!
  driveTrain.enableAll();
  driveTrain.clearAllFaults();

  for (int i = 1; i<=6; i++) {
    //Set the i-th node to velocity mode
    driveTrain.setMode(i, PROFILE_VELOCITY_MODE);

    //Set the velocity of the i-th node
    //driveTrain.setVelocity(i, 1000);
  }

  // //Wait for a few seconds
  // usleep(sleepTime * 1000);

  // for (int i = 4; i <= 6; i++) {
  //   //Stop each motor
  //   //stopAllMotors() would also work here; replace this for loop with it
  //   driveTrain.setVelocity(i, 0);
  // }

  // //Set node 1 to profile position mode
  // driveTrain.setMode(2, PROFILE_POSITION_MODE);

  // //Move the motor on node 1 back and forth a few times in position mode
  // //ABSOLUTE is a macro declared in EposDriveTrain.h
  // //For relative motion, use RELATIVE instead
  // for (int i=0; i<2; i++) {
  //   usleep(3000*1000);

  //   driveTrain.setPosition(2, 0, ABSOLUTE);

  //   usleep(3000*1000);

  //   driveTrain.setPosition(2, 100000, ABSOLUTE);
  //   //Units used in setPosition are encoder units.
  //   //What angle this actually corresponds to depends on the encoder, gear ratio, etc.
  // }
  
  
  // Converts wills stupid ID to actual motor ID


  while(true)
  {

    
    //cout << "Count1 = " << motor1 % modValue << "Count2= " << motor2 % modValue<< "Count3 = " << motor3 % modValue<< "Count4 = " << motor4 % modValue<< "Count5 = " << motor5 % modValue<< "Count6 = " << motor6 % modValue  << endl;
    int motor0WillPos = -(driveTrain.getPosition(getActualID(0)) % MODVALUE);
    int motor1WillPos = -(driveTrain.getPosition(getActualID(1)) % MODVALUE);
    int motor2WillPos = driveTrain.getPosition(getActualID(2)) % MODVALUE;
    int motor3WillPos = driveTrain.getPosition(getActualID(3)) % MODVALUE;
    int motor4WillPos = -(driveTrain.getPosition(getActualID(4)) % MODVALUE);
    int motor5WillPos = driveTrain.getPosition(getActualID(5)) % MODVALUE;
    
    legAngles[0] = 2*pi * ((float)motor0WillPos/MODVALUE);
    legAngles[1] = 2*pi * ((float)motor1WillPos/MODVALUE);
    legAngles[2] = 2*pi * ((float)motor2WillPos/MODVALUE);
    legAngles[3] = 2*pi * ((float)motor3WillPos/MODVALUE);
    legAngles[4] = 2*pi * ((float)motor4WillPos/MODVALUE);
    legAngles[5] = 2*pi * ((float)motor5WillPos/MODVALUE);
    
    // in case angle is negative convert to positive
      legAngles[0] = legAngles[0] + 2*pi*(legAngles[0] < 0);
      legAngles[1] = legAngles[1] + 2*pi*(legAngles[1] < 0);
      legAngles[2] = legAngles[2] + 2*pi*(legAngles[2] < 0);
      legAngles[3] = legAngles[3] + 2*pi*(legAngles[3] < 0);
      legAngles[4] = legAngles[4] + 2*pi*(legAngles[4] < 0);
      legAngles[5] = legAngles[5] + 2*pi*(legAngles[5] < 0);

    //cout << "test" << endl;
    //driveTrain.setVelocity(getActualID(0), 1000);
    //cout << "legGroundSpeed " << legGroundSpeed <<" state: " << state << "  legAngle[0] : " <<  legAngles[0]  << "  legAngle[1] : " <<  legAngles[1] <<"  legAngle[2] : " <<  legAngles[2] << " legAngle[3] : " <<  legAngles[3] <<"  legAngle[4] : " <<  legAngles[4] <<"  legAngle[5] : " <<  legAngles[5] <<endl;
      cout << "motor:2 = " << legAngles[2] << endl;
    //cout << "realvalue 0 : " << driveTrain.getPosition(getActualID(0))  <<" state: " << state << "  legAngle[0] : " <<  legAngles[0] << "  legAngle[1] : " <<  legAngles[1] <<"  legAngle[2] : " <<  legAngles[2] <<"  legAngle[3] : " <<  legAngles[3] <<"  legAngle[4] : " <<  legAngles[4] <<"  legAngle[5] : " <<  legAngles[5] <<endl;
    
    if (state == 1){

          if (legAngles[0] > pi){
            driveTrain.halt(getActualID(0));
            legStates[0] = 1;
          }
          else 
            driveTrain.setVelocity(getActualID(0), -legAirSpeed);

          if (legAngles[1] > pi){
            driveTrain.halt(getActualID(1));
            legStates[1] = 1;
          }
          else 
            driveTrain.setVelocity(getActualID(1), -legAirSpeed);

          if (legAngles[2] > pi){
            driveTrain.halt(getActualID(2));
            legStates[2] = 1;
          }
          else 
              driveTrain.setVelocity(getActualID(2), legAirSpeed);

          // make left legs go to home position
          if ((legAngles[3] < landingAngle) or (legAngles[3] > 2*pi - landingAngle)){
              driveTrain.halt(getActualID(3));
              legStates[3] = 1;
          }
          else 
              driveTrain.setVelocity(getActualID(3), legAirSpeed);


          if ((legAngles[4] < landingAngle) or (legAngles[4] > 2*pi - landingAngle)){
              driveTrain.halt(getActualID(4));
              legStates[4] = 1;
          }
          else 
              driveTrain.setVelocity(getActualID(4), -legAirSpeed);


          if ((legAngles[5] < landingAngle) or (legAngles[5] > 2*pi - landingAngle)){
              driveTrain.halt(getActualID(5));
              legStates[5] = 1;
          }
          else 
              driveTrain.setVelocity(getActualID(5), legAirSpeed);


          if (legStates[0] and legStates[1] and legStates[2] and legStates[3] and legStates[4] and legStates[5]){
            legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
            state = 2;
          }
      }
      // move right feet to ground and move left feet to the air
      else if (state == 2){

          // right legs finish air
          if (legAngles[0] > 2*pi - landingAngle){
            driveTrain.halt(getActualID(0));
            legStates[0] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(0), -legAirSpeed);
            legStates[0] = 0;
          }

          if (legAngles[1] > 2*pi - landingAngle){
            driveTrain.halt(getActualID(1));
            legStates[1] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(1), -legAirSpeed);
            legStates[1] = 0;
          }

          if (legAngles[2] > 2*pi - landingAngle){
            driveTrain.halt(getActualID(2));
            legStates[2] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(2), legAirSpeed);
            legStates[2] = 0;
          }

          // left legs finish ground step
          if ((legAngles[3] > landingAngle) and (legAngles[3] < landingAngle * 2)){
            driveTrain.halt(getActualID(3));
            legStates[3] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(3), legGroundSpeed);
            legStates[3] = 0;
          }

          if ((legAngles[4] > landingAngle) and (legAngles[4] < landingAngle * 2)){
            driveTrain.halt(getActualID(4));
            legStates[4] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(4), -legGroundSpeed);
            legStates[4] = 0;
          }

          if ((legAngles[5] > landingAngle) and (legAngles[5] < landingAngle * 2)){
            driveTrain.halt(getActualID(5));
            legStates[5] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(5), legGroundSpeed);
            legStates[5] = 0;
          }


          if (legStates[0] and legStates[1] and legStates[2] and legStates[3] and legStates[4] and legStates[5]){
            legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
            state = 3;
          }

      } 
      // move left feet to ground and move right feet to the air
      else if (state == 3){
          // right legs finish ground step
          if ((legAngles[0] > landingAngle) and (legAngles[0] < landingAngle * 2)){
            driveTrain.halt(getActualID(0));
            legStates[0] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(0), -legGroundSpeed);
            legStates[0] = 0;
          }

          if ((legAngles[1] > landingAngle) and (legAngles[1] < landingAngle * 2)){
            driveTrain.halt(getActualID(1));
            legStates[1] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(1), -legGroundSpeed);
            legStates[1] = 0;
          }

          if ((legAngles[2] > landingAngle) and (legAngles[2] < landingAngle * 2)){
            driveTrain.halt(getActualID(2));
            legStates[2] = 1;
          }
          else{ 
            driveTrain.setVelocity(getActualID(2), legGroundSpeed);
            legStates[2] = 0;
          }

          // left legs finish air
          if (legAngles[3] > 2*pi - landingAngle){
            driveTrain.halt(getActualID(3));
            legStates[3] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(3), legAirSpeed);
            legStates[3] = 0;
          }

          if (legAngles[4] > 2*pi - landingAngle){
            driveTrain.halt(getActualID(4));
            legStates[4] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(4), -legAirSpeed);
            legStates[4] = 0;
          }

          if (legAngles[5] > 2*pi - landingAngle){
            driveTrain.halt(getActualID(5));
            legStates[5] = 1;
          }
          else{
            driveTrain.setVelocity(getActualID(5), legAirSpeed);
            legStates[5] = 0;
          }


          if (legStates[0] and legStates[1] and legStates[2] and legStates[3] and legStates[4] and legStates[5]){
            legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
            state = 2;
          }
      } 
      //usleep(1000);
    }

    return 0;
}
