//Encoder units per rotation
const int MODVALUE = 176128;

const double pi = 3.14159265;
const float START_OFFSET_ANGLE = pi/2;

const double legAirSpeed = 7.0*200;
const double landingAngle = 0.349;
const double legGroundSpeed = legAirSpeed *(2*landingAngle/(2*pi - 2*landingAngle));

const int FRONTLEFT   = 1;
const int MIDDLELEFT  = 2;
const int BACKLEFT    = 3;
const int FRONTRIGHT  = 4;
const int MIDDLERIGHT = 5;  
const int BACKRIGHT   = 6;
const int accel = 10000;
const int deccel = 10000;


//time to wait in ms
const long sleepTime = 3000;

