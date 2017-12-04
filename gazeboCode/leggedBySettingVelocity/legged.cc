#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>

#include <ncurses.h>

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

      // Setup a P-controller, with these parameters
      this->pid = common::PID(.1, 0, 0);

      //this->joint = model->GetJoints()[0];

      // Apply the P-controller to the joint.
      this->model->GetJointController()->SetPositionPID(
          leftFront->GetScopedName(), this->pid);

      // initialize variables
      state = 0;
      for (int i = 0; i < 6; i++) {
        legAngles[i] = 0.0;
        legStates[i] = 0;
      }
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
      double timeNow = current_time.Double();

      // 20 degrees before home pos
      const double landingAngle = 0.349;
      const double pi = 3.14159;
      const double legAirSpeed = 10.0;
      double legGroundSpeed = legAirSpeed * (2*landingAngle/(2*pi - 2*landingAngle));

      legAngles[0] = fmod(this->rightFront->GetAngle(0).Radian(), 2*pi);
      legAngles[1] = fmod(this->leftMiddle->GetAngle(0).Radian(), 2*pi);
      legAngles[2] = fmod(this->rightBack->GetAngle(0).Radian(), 2*pi);
      legAngles[3] = fmod(this->leftFront->GetAngle(0).Radian(), 2*pi);
      legAngles[4] = fmod(this->rightMiddle->GetAngle(0).Radian(), 2*pi);
      legAngles[5] = fmod(this->leftBack->GetAngle(0).Radian(), 2*pi);

      if (state == 0){
         if (timeNow > 2)
            state = 1;
      }
      // right side in the air
      else if (state == 1){

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

          
          this->leftFront->SetVelocity(0, 0.0);
          this->rightMiddle->SetVelocity(0, 0.0);
          this->leftBack->SetVelocity(0, 0.0);
          

          if (legStates[0] and legStates[1] and legStates[2]){
            //std::cout << std::endl << "CHANGED to state2" << std::endl;
            legStates[0] = legStates[1] = legStates[2] = 0;
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
            //std::cout << "CHANGED to state3" << std::endl;
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
            //std::cout << "CHANGED to state2" << std::endl;
            legStates[0] = legStates[1] = legStates[2] = legStates[3] = legStates[4] = legStates[5] = 0;
            state = 2;
          }
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
    private: int count;
    private: int mode;
    private: int state;
    private: int leftStepCount = 0;
    private: int rightStepCount = 0;
    private: double legAngles [6];
    private: int legStates [6];


    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
