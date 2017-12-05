#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>
#include <ncurses.h>

#include <iostream>
#include <fstream>

#include <tuple>
#include <vector>

typedef std::vector <std::pair<int,int>> pair_list;

// CONVERT FROM ALBERT TRAJECTORY TO SUITABLE C++ TRAJECTORY USING THESE PYTHON COMMANDS:
// str = ALBERT TRAJECTORY STRING
// str = str.replace(')', '{')
// str = str.replace('(', '}')
// str = str.replace(']', '}')
// str = str.replace('[', '{')
// return str

pair_list trajectory = {{2, 0}, {2, 2}, {-2, 2}, {-2, -2}, {2, -2}, {0,0}};

const double trajSize = trajectory.size();

// how close to trajectory point rover moves to before moving on to next point in trajectory
const double distanceThreshold = .2;


// CONSTANTS
const double pi = 3.14159;
const double legAirSpeed = 7.0;
// 20 degrees before home pos
const double landingAngle = 0.349;
const double legGroundSpeed = legAirSpeed * (2*landingAngle/(2*pi - 2*landingAngle));

const double k_p = 5.0;


// GLOBAL VARIABLES
int state = 1;
int legStates[6] = {0,0,0,0,0,0};
double timeNow;

const double maxTurningThreshold = 0.3;
double turningThreshold = maxTurningThreshold;

int trajIndex = 0;

// variable for reading from gamepad, set usingGamepad = true if you want to control with gamecontroller
// USING LOGITECH F310 Gamepad
std::ifstream myfile;
int gameData;
const bool MANUAL_CONTROL = true;


void moveForward(gazebo::physics::JointPtr legs[6], double legAngles[6], int legStates[6]){
    // MOVE FORWARD
    
    // right side in the air
    if (state == 1){

        if (legAngles[0] > pi){
          legs[0]->SetVelocity(0, 0);
          legStates[0] = 1;
        }
        else 
          legs[0]->SetVelocity(0, legAirSpeed);

        if (legAngles[1] > pi){
          legs[1]->SetVelocity(0, 0);
          legStates[1] = 1;
        }
        else 
          legs[1]->SetVelocity(0, legAirSpeed);

        if (legAngles[2] > pi){
          legs[2]->SetVelocity(0, 0);
          legStates[2] = 1;
        }
        else 
          legs[2]->SetVelocity(0, legAirSpeed);

        // make left legs go to home position
        if ((legAngles[3] > landingAngle) and (legAngles[3] < 2*pi - 2*landingAngle)){
            legs[3]->SetVelocity(0, 0.0);
            legStates[3] = 1;
        }
        else 
            legs[3]->SetVelocity(0, legAirSpeed);

        if ((legAngles[4] > landingAngle) and (legAngles[4] < 2*pi - 2*landingAngle)){
            legs[4]->SetVelocity(0, 0.0);
            legStates[4] = 1;
        }
        else 
            legs[4]->SetVelocity(0, legAirSpeed);

        if ((legAngles[5] > landingAngle) and (legAngles[5] < 2*pi - 2*landingAngle)){
            legs[5]->SetVelocity(0, 0.0);
            legStates[5] = 1;
        }
        else 
            legs[5]->SetVelocity(0, legAirSpeed);
        

        if (legStates[0] and legStates[1] and legStates[2]){
          legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
          state = 2;
        }
    }
    // move right feet to ground and move left feet to the air
    else if (state == 2){
      
        // right legs finish air
        if (legAngles[0] > 2*pi - landingAngle){
          legs[0]->SetVelocity(0, 0);
          legStates[0] = 1;
        }
        else 
          legs[0]->SetVelocity(0, legAirSpeed);

        if (legAngles[1] > 2*pi - landingAngle){
          legs[1]->SetVelocity(0, 0);
          legStates[1] = 1;
        }
        else 
          legs[1]->SetVelocity(0, legAirSpeed);

        if (legAngles[2] > 2*pi - landingAngle){
          legs[2]->SetVelocity(0, 0);
          legStates[2] = 1;
        }
        else 
          legs[2]->SetVelocity(0, legAirSpeed);

        // left legs finish ground step
        if ((legAngles[3] > landingAngle) and (legAngles[3] < landingAngle * 2)){
          legs[3]->SetVelocity(0, 0);
          legStates[3] = 1;
        }
        else 
          legs[3]->SetVelocity(0, legGroundSpeed);

        if ((legAngles[4] > landingAngle) and (legAngles[4] < landingAngle * 2)){
          legs[4]->SetVelocity(0, 0);
          legStates[4] = 1;
        }
        else 
          legs[4]->SetVelocity(0, legGroundSpeed);

        if ((legAngles[5] > landingAngle) and (legAngles[5] < landingAngle * 2)){
          legs[5]->SetVelocity(0, 0);
          legStates[5] = 1;
        }
        else 
          legs[5]->SetVelocity(0, legGroundSpeed);
        

        if (legStates[0] and legStates[1] and legStates[2] and legStates[3] and legStates[4] and legStates[5]){
          legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
          state = 3;
        }
      
    } 
    // move left feet to ground and move right feet to the air
    else if (state == 3){
        // right legs finish ground step
        if ((legAngles[0] > landingAngle) and (legAngles[0] < landingAngle * 2)){
          legs[0]->SetVelocity(0, 0);
          legStates[0] = 1;
        }
        else 
          legs[0]->SetVelocity(0, legGroundSpeed);

        if ((legAngles[1] > landingAngle) and (legAngles[1] < landingAngle * 2)){
          legs[1]->SetVelocity(0, 0);
          legStates[1] = 1;
        }
        else 
          legs[1]->SetVelocity(0, legGroundSpeed);

        if ((legAngles[2] > landingAngle) and (legAngles[2] < landingAngle * 2)){
          legs[2]->SetVelocity(0, 0);
          legStates[2] = 1;
        }
        else 
          legs[2]->SetVelocity(0, legGroundSpeed);

        // left legs finish air
        if (legAngles[3] > 2*pi - landingAngle){
          legs[3]->SetVelocity(0, 0);
          legStates[3] = 1;
        }
        else 
          legs[3]->SetVelocity(0, legAirSpeed);

        if (legAngles[4] > 2*pi - landingAngle){
          legs[4]->SetVelocity(0, 0);
          legStates[4] = 1;
        }
        else 
          legs[4]->SetVelocity(0, legAirSpeed);

        if (legAngles[5] > 2*pi - landingAngle){
          legs[5]->SetVelocity(0, 0);
          legStates[5] = 1;
        }
        else 
          legs[5]->SetVelocity(0, legAirSpeed);
        

        if (legStates[0] and legStates[1] and legStates[2] and legStates[3] and legStates[4] and legStates[5]){
          legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
          state = 2;
        }
    } 
}


void rotateLeft(gazebo::physics::JointPtr legs[6], double legAngles[6], int legStates[6]){
    // COUNTERCLOCKWISE

    // right side in the air
    if (state == 1){

        if (legAngles[0] > pi){
          legs[0]->SetVelocity(0, 0);
          legStates[0] = 1;
        }
        else 
          legs[0]->SetVelocity(0, legAirSpeed);

        if (legAngles[1] > pi){
          legs[1]->SetVelocity(0, 0);
          legStates[1] = 1;
        }
        else 
          legs[1]->SetVelocity(0, -legAirSpeed);

        if (legAngles[2] > pi){
          legs[2]->SetVelocity(0, 0);
          legStates[2] = 1;
        }
        else 
          legs[2]->SetVelocity(0, legAirSpeed);

        
        // make left legs go to home position
        if ((legAngles[3] > landingAngle) and (legAngles[3] < 2*pi - 2*landingAngle)){
            legs[3]->SetVelocity(0, 0.0);
            legStates[3] = 1;
        }
        else 
            legs[3]->SetVelocity(0, -legAirSpeed);

        if ((legAngles[4] > landingAngle) and (legAngles[4] < 2*pi - 2*landingAngle)){
            legs[4]->SetVelocity(0, 0.0);
            legStates[4] = 1;
        }
        else 
            legs[4]->SetVelocity(0, legAirSpeed);

        if ((legAngles[5] > landingAngle) and (legAngles[5] < 2*pi - 2*landingAngle)){
            legs[5]->SetVelocity(0, 0.0);
            legStates[5] = 1;
        }
        else 
            legs[5]->SetVelocity(0, -legAirSpeed);
        

        if (legStates[0] and legStates[1] and legStates[2]){
          legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
          state = 2;
        }
    }
    // move right feet to ground and move left feet to the air
    else if (state == 2){
      
        // right legs finish air
        if (legAngles[0] > 2*pi - landingAngle){
          legs[0]->SetVelocity(0, 0);
          legStates[0] = 1;
        }
        else 
          legs[0]->SetVelocity(0, legAirSpeed);

        if (legAngles[1] < landingAngle){
          legs[1]->SetVelocity(0, 0);
          legStates[1] = 1;
        }
        else 
          legs[1]->SetVelocity(0, -legAirSpeed);

        if (legAngles[2] > 2*pi - landingAngle){
          legs[2]->SetVelocity(0, 0);
          legStates[2] = 1;
        }
        else 
          legs[2]->SetVelocity(0, legAirSpeed);

        // left legs finish ground step
        if ((legAngles[3] > 2 * landingAngle) and (legAngles[3] < 2*pi - landingAngle)){
          legs[3]->SetVelocity(0, 0);
          legStates[3] = 1;
        }
        else 
          legs[3]->SetVelocity(0, -legGroundSpeed);

        if ((legAngles[4] > landingAngle) and (legAngles[4] < landingAngle * 2)){
          legs[4]->SetVelocity(0, 0);
          legStates[4] = 1;
        }
        else 
          legs[4]->SetVelocity(0, legGroundSpeed);

        if ((legAngles[5] > 2 * landingAngle) and (legAngles[5] < 2*pi - landingAngle)){
          legs[5]->SetVelocity(0, 0);
          legStates[5] = 1;
        }
        else 
          legs[5]->SetVelocity(0, -legGroundSpeed);
        

        if (legStates[0] and legStates[1] and legStates[2] and legStates[3] and legStates[4] and legStates[5]){
          legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
          state = 3;
        }
      
    } 
    // move left feet to ground and move right feet to the air
    else if (state == 3){
        // right legs finish ground step. Used AND to ensure that when its greater than landing angle, it 
        //had just finished off the ground and not at the start (since the angle is around 6 radians at beginning of 
       // ground movement)
    
        if ((legAngles[0] > landingAngle) and (legAngles[0] < landingAngle * 2)){
          legs[0]->SetVelocity(0, 0);
          legStates[0] = 1;
        }
        else 
          legs[0]->SetVelocity(0, legGroundSpeed);

        if ((legAngles[1] > landingAngle * 2) and (legAngles[1] < 2*pi - landingAngle)){
          legs[1]->SetVelocity(0, 0);
          legStates[1] = 1;
        }
        else 
          legs[1]->SetVelocity(0, -legGroundSpeed);

        if ((legAngles[2] > landingAngle) and (legAngles[2] < landingAngle * 2)){
          legs[2]->SetVelocity(0, 0);
          legStates[2] = 1;
        }
        else 
          legs[2]->SetVelocity(0, legGroundSpeed);

        // left legs finish air
        if (legAngles[3] < landingAngle){
          legs[3]->SetVelocity(0, 0);
          legStates[3] = 1;
        }
        else 
          legs[3]->SetVelocity(0, -legAirSpeed);

        if (legAngles[4] > 2*pi - landingAngle){
          legs[4]->SetVelocity(0, 0);
          legStates[4] = 1;
        }
        else 
          legs[4]->SetVelocity(0, legAirSpeed);

        if (legAngles[5] < landingAngle){
          legs[5]->SetVelocity(0, 0);
          legStates[5] = 1;
        }
        else 
          legs[5]->SetVelocity(0, -legAirSpeed);
        

        if (legStates[0] and legStates[1] and legStates[2] and legStates[3] and legStates[4] and legStates[5]){
          legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
          state = 2;
        }
    }
}

void rotateRight(gazebo::physics::JointPtr legs[6], double legAngles[6], int legStates[6]){
    // CLOCKWISE

    // right side in the air
    if (state == 1){

        if (legAngles[0] > pi){
          legs[0]->SetVelocity(0, 0);
          legStates[0] = 1;
        }
        else 
          legs[0]->SetVelocity(0, -legAirSpeed);

        if (legAngles[1] > pi){
          legs[1]->SetVelocity(0, 0);
          legStates[1] = 1;
        }
        else 
          legs[1]->SetVelocity(0, legAirSpeed);

        if (legAngles[2] > pi){
          legs[2]->SetVelocity(0, 0);
          legStates[2] = 1;
        }
        else 
          legs[2]->SetVelocity(0, -legAirSpeed);

        
        // make left legs go to home position
        if (legAngles[3] > 2*pi - 2*landingAngle){
            legs[3]->SetVelocity(0, 0.0);
            legStates[3] = 1;
        }
        else 
            legs[3]->SetVelocity(0, legAirSpeed);

        if (legAngles[4] < landingAngle){
            legs[4]->SetVelocity(0, 0.0);
            legStates[4] = 1;
        }
        else 
            legs[4]->SetVelocity(0, -legAirSpeed);

        if (legAngles[5] > 2*pi - 2*landingAngle){
            legs[5]->SetVelocity(0, 0.0);
            legStates[5] = 1;
        }
        else 
            legs[5]->SetVelocity(0, legAirSpeed);
        

        if (legStates[0] and legStates[1] and legStates[2]){
          legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
          state = 2;
        }
    }
    // move right feet to ground and move left feet to the air
    else if (state == 2){
      
        // right legs finish air
        if (legAngles[0] < landingAngle){
          legs[0]->SetVelocity(0, 0);
          legStates[0] = 1;
        }
        else 
          legs[0]->SetVelocity(0, -legAirSpeed);

        if (legAngles[1] > 2*pi - landingAngle){
          legs[1]->SetVelocity(0, 0);
          legStates[1] = 1;
        }
        else 
          legs[1]->SetVelocity(0, legAirSpeed);

        if (legAngles[2] < landingAngle){
          legs[2]->SetVelocity(0, 0);
          legStates[2] = 1;
        }
        else 
          legs[2]->SetVelocity(0, -legAirSpeed);

        // left legs finish ground step
        if ((legAngles[3] > landingAngle) and (legAngles[3] < landingAngle * 2)){
          legs[3]->SetVelocity(0, 0);
          legStates[3] = 1;
        }
        else 
          legs[3]->SetVelocity(0, legGroundSpeed);

        if ((legAngles[4] > 2 * landingAngle) and (legAngles[4] < 2*pi - landingAngle)){
          legs[4]->SetVelocity(0, 0);
          legStates[4] = 1;
        }
        else 
          legs[4]->SetVelocity(0, -legGroundSpeed);

        if ((legAngles[5] > landingAngle) and (legAngles[5] < landingAngle * 2)){
          legs[5]->SetVelocity(0, 0);
          legStates[5] = 1;
        }
        else 
          legs[5]->SetVelocity(0, legGroundSpeed);
        

        if (legStates[0] and legStates[1] and legStates[2] and legStates[3] and legStates[4] and legStates[5]){
          legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
          state = 3;
        }
      
    } 
    // move left feet to ground and move right feet to the air
    else if (state == 3){
        // right legs finish ground step. Used AND to ensure that when its greater than landing angle, it 
        //had just finished off the ground and not at the start (since the angle is around 6 radians at beginning of 
       // ground movement)
    
        if ((legAngles[0] > landingAngle * 2) and (legAngles[0] < 2*pi - landingAngle)){
          legs[0]->SetVelocity(0, 0);
          legStates[0] = 1;
        }
        else 
          legs[0]->SetVelocity(0, -legGroundSpeed);

        if ((legAngles[1] > landingAngle) and (legAngles[1] < landingAngle * 2)){
          legs[1]->SetVelocity(0, 0);
          legStates[1] = 1;
        }
        else 
          legs[1]->SetVelocity(0, legGroundSpeed);

        if ((legAngles[2] > landingAngle * 2) and (legAngles[2] < 2*pi - landingAngle)){
          legs[2]->SetVelocity(0, 0);
          legStates[2] = 1;
        }
        else 
          legs[2]->SetVelocity(0, -legGroundSpeed);

        // left legs finish air
        if (legAngles[3] > 2*pi - landingAngle){
          legs[3]->SetVelocity(0, 0);
          legStates[3] = 1;
        }
        else 
          legs[3]->SetVelocity(0, legAirSpeed);

        if (legAngles[4] < landingAngle){
          legs[4]->SetVelocity(0, 0);
          legStates[4] = 1;
        }
        else 
          legs[4]->SetVelocity(0, -legAirSpeed);

        if (legAngles[5] > 2*pi - landingAngle){
          legs[5]->SetVelocity(0, 0);
          legStates[5] = 1;
        }
        else 
          legs[5]->SetVelocity(0, legAirSpeed);
        

        if (legStates[0] and legStates[1] and legStates[2] and legStates[3] and legStates[4] and legStates[5]){
          legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
          state = 2;
        }
    }
}



namespace gazebo
{
  class ModelPush : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
      // Store the pointer to the model
      this->model = _parent;
      this->leftFront = this->model->GetJoint("left_front_wheel_hinge");
      this->leftMiddle = this->model->GetJoint("left_middle_wheel_hinge");
      this->leftBack = this->model->GetJoint("left_back_wheel_hinge");
      this->rightFront = this->model->GetJoint("right_front_wheel_hinge");
      this->rightMiddle = this->model->GetJoint("right_middle_wheel_hinge");
      this->rightBack = this->model->GetJoint("right_back_wheel_hinge");

      // initialize variables
      legs[0] = this->rightFront;
      legs[1] = this->leftMiddle;
      legs[2] = this->rightBack;
      legs[3] = this->leftFront;
      legs[4] = this->rightMiddle;
      legs[5] = this->leftBack;

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&ModelPush::OnUpdate, this, _1));
    }


    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
      // Apply a small linear velocity to the model.
      //this->model->SetLinearVel(math::Vector3(1, 0, 0));
      common::Time current_time = this->model->GetWorld()->GetSimTime();
      timeNow = current_time.Double();

      legAngles[0] = fmod(legs[0]->GetAngle(0).Radian(), 2*pi);
      legAngles[1] = fmod(legs[1]->GetAngle(0).Radian(), 2*pi);
      legAngles[2] = fmod(legs[2]->GetAngle(0).Radian(), 2*pi);
      legAngles[3] = fmod(legs[3]->GetAngle(0).Radian(), 2*pi);
      legAngles[4] = fmod(legs[4]->GetAngle(0).Radian(), 2*pi);
      legAngles[5] = fmod(legs[5]->GetAngle(0).Radian(), 2*pi);

      // in case angle is negative convert to positive
      legAngles[0] = legAngles[0] + 2*pi*(legAngles[0] < 0);
      legAngles[1] = legAngles[1] + 2*pi*(legAngles[1] < 0);
      legAngles[2] = legAngles[2] + 2*pi*(legAngles[2] < 0);
      legAngles[3] = legAngles[3] + 2*pi*(legAngles[3] < 0);
      legAngles[4] = legAngles[4] + 2*pi*(legAngles[4] < 0);
      legAngles[5] = legAngles[5] + 2*pi*(legAngles[5] < 0);

      // wait two seconds before allowing rover to move 
      if (timeNow < 2){
          return;
      }
      
      if (MANUAL_CONTROL){
          if (timeNow - saveTimeShorter > .15){
               saveTimeShorter = timeNow;

              // read from controlValues.txt which is being updated by the gameController
              myfile.open ("controlValues.txt");
              myfile >> gameData;
              myfile.close();
          }
      }

      // controller for following a point
      this->model_coor = this->model->GetWorldPose();
      math::Vector3 modelPos(0,0,0);
      modelPos = this->model_coor.pos;

      // get current goal from the trajectory using trajIndex
      double xgoal = trajectory[trajIndex].first;
      double ygoal = trajectory[trajIndex].second;

      double xdisp, ydisp;
      xdisp = xgoal - modelPos.x;
      ydisp = ygoal - modelPos.y;

      double dist2Goal = pow(xdisp*xdisp + ydisp*ydisp, .5);
      if (dist2Goal < distanceThreshold){
          trajIndex += 1;
      }

      if (trajIndex == trajSize){
          return;
      }

      double actualYaw = this->model_coor.rot.GetYaw();

      double desYaw = atan2(ydisp, xdisp);

      int turningState = 0;
      double yawError;

      

      double turningStateSave = turningState;

      // if usingGamepad control through gameData variable, else do controller to follow trajectory
      if (MANUAL_CONTROL){
          // button X
          if (gameData == 0){
              rotateLeft(legs, legAngles, legStates);
              turningState = 1;
          } 
          // button A
          else if (gameData == 1){
              for (int i = 0; i < 6; i++){
                 legs[i]->SetVelocity(0, 0);
              }
          }
          // button B
          else if (gameData == 2){
              rotateRight(legs, legAngles, legStates);
              turningState = 2;
          }
          else if (gameData == 3){
              moveForward(legs, legAngles, legStates);
              turningState = 3;
          }
      } else{

          // if the desired yaw is to the left of the permitted yaw interval, rotateLeft
          if ((desYaw > actualYaw + turningThreshold) or (actualYaw - desYaw > pi))
          {
            rotateLeft(legs, legAngles, legStates);
            turningState = 1;

            // shrink turning threshold so rover turns to very close to correct angle
            turningThreshold = maxTurningThreshold/4.0;
          } 
          // else if the desired yaw is to the right of the permitted yaw interval, rotateRight
          else if ((desYaw < actualYaw - turningThreshold) or (desYaw - actualYaw > pi))
          {
            rotateRight(legs, legAngles, legStates);
            turningState = 2;

            // shrink turning threshold so rover turns to very close to correct angle
            turningThreshold = maxTurningThreshold/4.0;
          } 
          else {
            moveForward(legs, legAngles, legStates);
            turningState = 3;

            // expand turning threshold so rover doesn't have to keep rotating
            turningThreshold = maxTurningThreshold;
          }

          if (timeNow - saveTime > .5){
               saveTime = timeNow;
               std::cout << "actualYaw: " << actualYaw << " desYaw: " << desYaw << " TurningState: " << turningState << " xgoal: " << xgoal << " ygoal: " << ygoal << " xdisp: " << xdisp << " ydisp: " << ydisp << " dis2Goal: " << dist2Goal << std::endl;
          }
      }
      
    
      // reset state to 1 if type of turningState has changed
      if (turningStateSave != turningState){
        state = 1;
        legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
      }      


    }

    // Pointer to the model
    private: physics::ModelPtr model;

    /// \brief Pointer to the joint.
    private: physics::JointPtr joint;

    /// \brief A PID controller for the joint.
    private: common::PID pid;

    // Will- Point to joint
    private: physics::JointPtr leftFront;
    private: physics::JointPtr leftMiddle;
    private: physics::JointPtr leftBack;
    private: physics::JointPtr rightFront;
    private: physics::JointPtr rightMiddle;
    private: physics::JointPtr rightBack;


    private: physics::JointPtr legs[6]; 

    // variables for walking
    private: int state;
    private: double legAngles [6];


    private: math::Pose model_coor;
    private: math::Pose goal_coor;
    private: math::Vector3 goalVector;
    private: double saveTime;
    private: double saveTimeShorter;


    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
