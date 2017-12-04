#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>

#include <math.h>
#include <ncurses.h>
#include <cmath>
#include <iostream>


namespace gazebo
{
  class ModelPush : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr /*_sdf*/)
    {
      // Store the pointer to the model
      this->model = _parent;
      this->leftFront = this->model->GetJoint("left_front_wheel_hinge");
      this->leftBack = this->model->GetJoint("left_back_wheel_hinge");
      this->rightFront = this->model->GetJoint("right_front_wheel_hinge");
      this->rightBack = this->model->GetJoint("right_back_wheel_hinge");
      this->model_coor = this->model->GetWorldPose();

      // Setup a P-controller, with these parameters
      this->pid = common::PID(.1, 0, 0);

      //this->joint = model->GetJoints()[0];

      // Apply the P-controller to the joint.
      this->model->GetJointController()->SetPositionPID(
          leftFront->GetScopedName(), this->pid);

      // SetJointPosition will set the joint to the value instantly, which is not realistic. Use for setting up
      // the robot's intitial configuration
      //this->model->GetJointController()-> SetJointPosition (leftFront->GetScopedName(), 0.0);
      mode = 0;
      count = 0;
      saveTime = 0.0;

      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&ModelPush::OnUpdate, this, _1));
    }

    public: void moveLeftSide(double targetForce){
      this->leftFront->SetForce(0, targetForce);
      this->leftBack->SetForce(0, targetForce);
    }
    public: void moveRightSide(double targetForce){
      this->rightFront->SetForce(0, targetForce);
      this->rightBack->SetForce(0, targetForce);
    }

    // this doesn't really rotate like you expect, because it skids on the floor
    public: void turnClockwise(double targetForce){
      this->rightFront->SetForce(0, -targetForce);
      this->rightBack->SetForce(0, -targetForce);
      this->leftFront->SetForce(0, targetForce);
      this->leftBack->SetForce(0, targetForce);
    }
    public: void turnCounterClockwise(double targetForce){
      this->rightFront->SetForce(0, targetForce);
      this->rightBack->SetForce(0, targetForce);
      this->leftFront->SetForce(0, -targetForce);
      this->leftBack->SetForce(0, -targetForce);
    }
     public: void moveRightForward(double targetForce){
      this->rightFront->SetForce(0, 1.5*targetForce);
      this->rightBack->SetForce(0, 1.5*targetForce);
      this->leftFront->SetForce(0, targetForce);
      this->leftBack->SetForce(0, targetForce);
    }
     public: void moveLeftForward(double targetForce){
      this->rightFront->SetForce(0, targetForce);
      this->rightBack->SetForce(0, targetForce);
      this->leftFront->SetForce(0, 1.5*targetForce);
      this->leftBack->SetForce(0, 1.5*targetForce);
    }
    public: void diffMove(double targetLeft,double targetRight){
      this->rightFront->SetVelocity(0, targetRight);
      this->rightBack->SetVelocity(0, targetRight);
      this->leftFront->SetVelocity(0, targetLeft);
      this->leftBack->SetVelocity(0, targetLeft);
    }


    // Called by the world update start event
    public: void OnUpdate(const common::UpdateInfo & /*_info*/)
    {
      // Apply a small linear velocity to the model.
      //this->model->SetLinearVel(math::Vector3(1, 0, 0));
      common::Time current_time = this->model->GetWorld()->GetSimTime();
      double timeNow = current_time.Double();

      //this->rightFront->SetParam("vel", 0, 1.0);

      // get angles
      //std::cout << "rightFront " << rightBack->GetAngle(0) << "\n";
      /*
      this->leftFront->SetForce(0, targetForce);
      this->leftBack->SetForce(0, targetForce);
      */
      this->model_coor = this->model->GetWorldPose();


      math::Vector3 goalVector(10.0,10.0,5.0);

      math::Vector3 modelPos(0,0,0);
      modelPos = this->model_coor.pos;
      //double yaw = model.rot.yaw;
      double x, y, z;
      x = goalVector.x - modelPos.x;
      y = goalVector.y - modelPos.y;
      z = goalVector.z - modelPos.z;

     // math::Vector3 ModelOrientation = this->model_coor.rot.GetYaw();
      double actualYaw = this->model_coor.rot.GetYaw();//odelOrientation.x; 

      double desYaw = atan2(y, x);

      int turning = 0;
      double yawError;

      double turningThreshold = 0.08;
      double k_p = 5.0;

      
      if (desYaw > actualYaw + turningThreshold)
      {
        yawError = (desYaw - actualYaw) * k_p;

        diffMove(0.0, 1.0 + yawError);
        turning = 1;

      } 
      else if (actualYaw - desYaw > 3.1415){

        yawError = (actualYaw - desYaw) * k_p;

        diffMove(0.0, 1.0 + yawError);
        turning = 1;
      }
      else if (desYaw < actualYaw - turningThreshold)
      {
        // multiply by negative because this is negative
        yawError = -1.0 * (desYaw - actualYaw) * k_p;

        diffMove(1.0 + yawError, 0.0);
        turning = 2;
      } 
      else if (desYaw - actualYaw > 3.1415){
        yawError = (desYaw - actualYaw) * k_p;

        diffMove(1.0 + yawError, 0.0);
        turning = 2;
      }
      else {
        diffMove(3.0, 3.0);
        turning = 3;
      }

      if (timeNow - saveTime > .4){
           saveTime = timeNow;
           std::cout << "actualYaw: " << actualYaw << " desYaw: " << desYaw << " P_err: " << yawError << " Turning Index: " << turning << " x: " << x << " y: " << y << " z: " << z << std::endl;
      }
      //int points = [(0, 0), (0, -1), (0, -2), (0, -3), (-1, -4), (-2, -5), (-3, -6), (-3, -7), (-3, -8), (-2, -9), (-1, -10), (0, -10), (1, -10), (2, -10), (3, -11), (4, -11), (5, -12), (5, -13), (5, -14), (6, -15), (7, -16), (7, -17), (7, -18), (7, -19), (7, -20), (7, -21), (8, -22), (9, -23), (10, -24), (11, -25), (11, -26), (11, -27), (11, -28), (12, -29), (13, -30), (13, -31), (14, -32), (15, -33), (16, -34), (16, -35), (16, -36), (16, -37), (16, -38), (16, -39), (16, -40), (17, -41), (17, -42), (18, -43), (19, -44), (20, -44), (21, -44), (22, -44), (23, -45), (24, -46), (24, -47), (24, -48), (24, -49), (25, -50), (26, -51), (27, -52), (28, -53), (29, -54), (30, -55), (31, -56), (32, -57), (33, -58), (34, -57), (35, -57), (36, -57), (37, -57), (38, -58), (39, -59), (40, -60), (41, -60), (42, -59), (43, -58), (44, -58), (45, -59), (46, -59), (47, -59), (48, -59), (49, -60), (50, -61), (51, -62), (52, -62), (53, -62), (54, -62), (55, -62), (56, -63), (57, -63), (58, -63), (59, -64), (60, -64), (61, -63), (62, -62), (63, -63), (64, -64)]


      

      

    //   bool flag = false;
    // if (fmod(timeNow, 2.0) < 1.0){
    //        flag = true

      //moveLeftSide(-1.0);
      //moveRightSide(-1.0);

     

     

      //std::cout << "x: " << x  << "y: " << y << "z: " << z << std::endl;
       //   } 
      // if (fmod(timeNow, 10.0) > 5.0){
      //   if (mode == 1){

      //     //double curAngle = (leftFront->GetAngle(0).Degree())/57.32;
          
      //     //std::cout << "leftFront " << leftFront->GetAngle(0) << "\n";
      //     mode = 0;

      //   }
      //   turnClockwise(3.0);
        
      // } else{
      //   if (mode == 0){
          
      //     //std::cout << "rightFront " << rightFront->GetAngle(0) << "\n";
      //     mode = 1;
      //   }
      //   turnCounterClockwise(6.0);
      // }  

      //std::cout << "rightFront " << rightBack->GetAngle(0) << "\n";

      //moveLeftForward(4);


      


      //step left
      /*
      this->rightMiddle->SetForce(0, 2);
      this->leftBack->SetForce(0, 2);
      this->leftFront->SetForce(0, 2);
      */



      //this->leftFront->SetForce(0, 2);
      


    }

    private: int count;
    private: int mode;

    // Pointer to the model
    private: physics::ModelPtr model;

    /// \brief Pointer to the joint.
    private: physics::JointPtr joint;

    /// \brief A PID controller for the joint.
    private: common::PID pid;

    // Will- Point to joint
    private: physics::JointPtr leftFront;
    private: physics::JointPtr leftBack;
    private: physics::JointPtr rightFront;
    private: physics::JointPtr rightBack;
    private: math::Pose model_coor;
    private: math::Pose goal_coor;

    private: math::Vector3 goalVector;

    private: double saveTime;
    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
