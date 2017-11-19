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

    public: void moveLeftSide(double targetAngle){
      //this->model->GetJointController()-> SetPositionTarget (leftFront->GetScopedName(), targetAngle);
      //this->model->GetJointController()-> SetPositionTarget (leftBack->GetScopedName(), targetAngle);
      //this->model->GetJointController()-> SetPositionTarget (rightMiddle->GetScopedName(), targetAngle);
    }
    public: void moveRightSide(double targetAngle){
      //this->model->GetJointController()-> SetPositionTarget (rightMiddle->GetScopedName(), targetAngle);
      this->model->GetJointController()-> SetPositionTarget (rightBack->GetScopedName(), targetAngle);
      //this->model->GetJointController()-> SetPositionTarget (leftMiddle->GetScopedName(), targetAngle);
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

      if (fmod(timeNow, 10.0) > 5.0){
        if (mode == 1){
          double curAngle = (leftFront->GetAngle(0).Degree())/57.32;
          //moveLeftSide(2*3.14);
          //std::cout << "leftFront " << leftFront->GetAngle(0) << "\n";
          mode = 0;

        }
        
      } else{
        if (mode == 0){
          count = count + 1;
          double curAngle = (rightFront->GetAngle(0).Degree())/57.32;
          moveRightSide(2*3.14);
          //std::cout << "rightFront " << rightFront->GetAngle(0) << "\n";
          mode = 1;
        }
      }

      



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
    private: physics::JointPtr leftMiddle;
    private: physics::JointPtr leftBack;
    private: physics::JointPtr rightFront;
    private: physics::JointPtr rightMiddle;
    private: physics::JointPtr rightBack;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(ModelPush)
}
