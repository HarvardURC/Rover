# EposLib

This is the library for controlling the drive motors of the rover through the EPOS command library.
To include this library in a project/program, copy the EposDriveTrain .h and .cpp files into the same directory as the other source files.
Add this line to the rest of the include statements:

#include "EposDriveTrain.h"

## Usage:

First, a drive train object must be declared. This class' constructor accepts a string as an argument.
This string is the USB port name that the motor controllers are connected to. So far, USB0 has worked on all occasions.
But, this could possibly change if other USB devices are connected. This library has not been tested with any other USB devices plugged in (besides keyboards/mice)

Once the object has been declared, it must be initialized by calling its init() method.
This library will not be able to talk to the motor controllers if init() is not called.
init() returns an unsigned int as the error code given by the device.
0 means no error, the other error codes are in a table in the EPOS command library (Sect. 8, starting at page 135).

Before any motors can be moved, they must be enabled. This can be accomplished by calling enableAll(), which enables all nodes.
If a node is enabled, its LED will be solid green (disabled nodes are flashing green).
If the LED on a node is red, that node is in a fault state and will not move its motor. The fault must be cleared before the motor can move.
This can be accomplished by calling clearFault(i), where i is an integer corresponding to the node number.
Calling the clearAllFaults() method is a convenient way to clear the fault states on all of the nodes.

If a node is enabled and fault free, then its motor can be moved.

#### Mode setting
Each motor can be moved using a different number of modes (position, velocity, etc.).
Currently, there are 4 macros defined in EposDriveTrain.h:
- Velocity mode (VELOCITY_MODE)
- Position mode (POSITION_MODE)
- Profile velocity mode (PROFILE_VELOCITY_MODE)
- Profile position mode (PROFILE_POSITION_MODE)

To set a node's mode, call the setMode(i, MODE) function. i is the node number, and MODE is one of the above macros

Once a node's mode has been set, the motor on that node can be moved.
NOTE: a motor can only be moved in the same mode as it was set in; ie, calling setVelocity() when a node is in position mode will not work

#### Velocity mode
Once a node has been set to velocity mode (or profile velocity mode), its velocity can be set by calling the setVelocity() function.
The first argument taken by this function is the node ID as an int, and the second argument is the desired velocity in RPM as a long.
It returns the error code returned by the controller (an unsigned int), 0 means no error.
Other errors are in a table in the EPOS command library (Sect. 8, starting at page 135).

When a node has been given a velocity, it will try to move that motor at a constant velocity (control loop is internal to the motor controllers).

#### Position mode
Once a node has been set to position mode (or profile position mode), its desired position can be set using the setPositon() function.
The first argument taken by this function is the node ID as an int, and the second agrument is the desired position (in encoder units)
as a long, and the third argument is a bool denoting relative or absolute motion.
Use the macros RELATIVE and ABSOLUTE, which are defined in EposDriveTrain.h

When a node has been given a position target, it will try to move to that target (control loop is internal to the motor controller).
To check to see if a node has reached its position target, call the isAtTarget() function, which takes the node ID as an int and returns
a bool denoting whether it has reached its target.

#### Other funcitons
For all of the following, i denotes the node ID, and it is always an int
- getVelocity(i) returns the velocity of the motor at node i as an int
- getPosition(i) returns the position of the motor at node i as an int
- getCurrent(i) returns the current draw of the motor at node i as a short
- stopAllMotors() sets all motors to velocity 0. This may only work if motors are in velocity mode.
