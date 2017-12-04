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
      this->goal_coor = math::Pose(10, -10, 5, 0, 0 ,0);
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

      moveLeftForward(4);
      moveRightForward(4);


      


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
    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
