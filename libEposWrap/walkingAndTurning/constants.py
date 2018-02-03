#Encoder units per rotation
MODVALUE = 176128;

pi = 3.14159265;
START_OFFSET_ANGLE = pi/2;

legAirSpeed = int(7.0*200);
landingAngle = 0.349;
legGroundSpeed = int(legAirSpeed *(2*landingAngle/(2*pi - 2*landingAngle)));

FRONTLEFT   = 1;
MIDDLELEFT  = 2;
BACKLEFT    = 3;
FRONTRIGHT  = 4;
MIDDLERIGHT = 5;  
BACKRIGHT   = 6;
accel = 10000;
deccel = 10000;


#time to wait in ms
sleepTime = 1000;

