
int ch1; // Here's where we'll keep our channel values
int ch2;
int ch3;
int forwardBackwardSwitch;
int chswitch;
int forwardCutoff = 1400;
int backwardCutoff = 1000;
int leftCutoff = 1450;
int rightCutoff = 1530;

int deadSpaceval = 20; //CHANGE increase to increase deadspace on arm

int servolinear1Min = 275; //CHANGE based on test
int servolinear1Max = 365;
int servolinear2Min = 250; 
int servolinear2Max = 475;
int servoWristPanMin = 130;
int servoWristPanMax = 600;
int servoWristTiltMin = 190;
int servoWristTiltMax = 600;
int servoContinousMin = 100;
int servoContinousMax = 300;
int servoContinousStop =150;
int servolinear1MiddleVal = (servolinear1Max+servolinear1Min)/2;
int servolinear2MiddleVal = (servolinear2Max+servolinear2Min)/2;
int servoWristTiltMiddleVal = (servoWristTiltMin+servoWristTiltMax)/2;
int servoWristPanMiddleVal = (servoWristPanMin+servoWristPanMax)/2;
int servoContinousMiddleVal = (servoContinousMin+servoContinousMax)/2;


void setup() {

  pinMode(2, INPUT); // Set our input pins as such need to CHANGE add ch4 on reciever here
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(8, INPUT);
  Serial.begin(9600); // Pour a bowl of Serial ha get it?

}

void loop() {

  ch1 = pulseIn(2, HIGH); // throttle (channel 1 on reciever)
  ch2 = pulseIn(3, HIGH); // channel 2  left and right on right joystick (channel 4 on reciever)
  ch3 = pulseIn(4, HIGH); // channel 3 up and down on right joystick   
  int ch4 = pulseIn(8, HIGH); //CHANGE missing ch4(idk what pin it is) gonna say its 7 for now
  int forwardBackwardSwitch = pulseIn(5, HIGH);  //swtich to forward  (SFswitch ch5 on reciever)
  int chswitch = pulseIn(6, HIGH); // three way swtich for grabber  979-1480ish-1991 (three states) ch6 on reciever
  int speed_servo = map(ch1, 980, 2000, 1000, 3000); //this is the speed mapping CHANGE last argument to increase speed
  int servolinear1= map(ch1, 980, 2000, servolinear1Min, servolinear1Max); // need to CHANGE min max values here (thrid and forth args)
  int servolinear2= map(ch3, 980, 2000, servolinear2Min, servolinear2Max);  // again need to CHANGE last two (min max args)
  int servoWristPan = map(ch2, 980, 2000, servoWristPanMin, servoWristPanMax); //again need to CHANGE last two (min max args)
  int servoWristTilt = map(ch3, 980, 2000, servoWristTiltMin, servoWristTiltMax); 
  int servoContinous = map(ch4, 980, 2000, servoContinousMin, servoContinousMax); // increase last 2 args to decrease dead zone
  int gripper = map(ch1, 980, 2000, 0, 100);
  // debugging stuff below
  //int servo2 = map(ch2, 980, 2000, 0, 180); 
  // Serial.print("Channel 1 ");
  // Serial.print(ch1);
  // Serial.print("Channel 2 ");
  // Serial.print(ch2);
  // Serial.print("Channel 3 ");
  // Serial.print(ch3);
  //Serial.print("chswitch ");
  //Serial.println(chswitch);

  if(chswitch < 1000) // if in "walking mode"
  {
    if(ch1 >= 1900){  // if the throttle is (near fully down send stop)
        Serial.print("x-");   
      Serial.println(speed_servo);
    }
    else{
      if(forwardBackwardSwitch> 1900 && ch2 <= 1530 && ch2 >= 1450){
      if(speed_servo > 1200){
        
        Serial.print("s-");   
        Serial.println(speed_servo);
      }
      else{
        Serial.print("x-");   
        Serial.println(speed_servo);
      }  
      }
      else if(forwardBackwardSwitch < 1500  && ch2 <= 1530 && ch2 >= 1450){
        if(speed_servo > 1200){
           Serial.print("w-");   
        Serial.println(speed_servo);   
      }
      else{
        Serial.print("x-");   
        Serial.println(speed_servo);
      }
      
      }
      else if(ch2 > 1530 || ch2 < 1450){
        if(ch2 > rightCutoff){
        Serial.print("d-");   
        Serial.println(speed_servo); 
        }
        else if(ch2 < leftCutoff){
        Serial.print("a-");   
        Serial.println(speed_servo);  
        }
        else{
        Serial.print("x-");   
        Serial.println(speed_servo);
        }
      }
    }
            
  }
  else if(chswitch>1000 && chswitch < 1900) // in arm mode 1, 
  {
    if(servoContinous > 180 && servoContinous < 140){
    Serial.print("b-");   
      Serial.println(servoContinous);
  }
   Serial.print("c-");   
   Serial.println(s);
   Serial.print("v-");   
   Serial.println(servolinear2); 

  } 
  else if(chswitch >= 1900) // in arm mode 2. (gripper and wrist pan/ wrist tilt)
  { // basically higher precision mode
    if(gripper <= 60 && servoContinous >=40){
    Serial.println("l-");
  }
  else if(gripper > 60 ){
    Serial.println("k-");
  }
  else if(gripper < 40){
    Serial.println("j-");
  }
   Serial.print("n-");   
   Serial.println(servoWristTilt);
   Serial.print("m-");   
   Serial.println(servoWristPan); 

  } 
  delay(100); //probably don't need this here, but you know, why not

}
      
