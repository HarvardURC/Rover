
int ch1;
int ch2;
int ch3;
int forwardBackwardSwitch;
int chswitch;
int forwardCutoff = 1400;
int backwardCutoff = 1000;
int leftCutoff = 1450;
int rightCutoff = 1530;

int deadSpaceval = 20; //doesn't matter not used

int servolinear1Min = 275; 
int servolinear1Max = 365;
int servolinear2Min = 250; 
int servolinear2Max = 475;
int servoWristPanMin = 130;
int servoWristPanMax = 600;
int servoWristTiltMin = 190;
int servoWristTiltMax = 600;
int servoContinuousMin = 120;
int servoContinuousMax = 200;
int servoContinuousStop =150;
int servolinear1MiddleVal = (servolinear1Max+servolinear1Min)/2;
int servolinear2MiddleVal = (servolinear2Max+servolinear2Min)/2;
int servoWristTiltMiddleVal = (servoWristTiltMin+servoWristTiltMax)/2;
int servoWristPanMiddleVal = (servoWristPanMin+servoWristPanMax)/2;
int servoContinuousMiddleVal = (servoContinuousMin+servoContinuousMax)/2;
int gripperMaxValue = 400;
int gripperMinValue = 200;
int gripperMiddleValue = (gripperMaxValue+gripperMinValue)/2;

void setup() {

  pinMode(2, INPUT); // Set our input pins as such need to CHANGE add ch4 on reciever here
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  Serial.begin(9600); // Pour a bowl of Serial ha get it?
  int servoWristPanWriteValue = servoWristPanMiddleVal;
  int servoWristTiltWriteValue = servoWristTiltMiddleVal;
  int gripperWriteValue = gripperMiddleValue; 
}

void loop() {

  ch1 = pulseIn(2, HIGH); // throttle (channel 1 on reciever ) up down on left joystick
  ch2 = pulseIn(3, HIGH); // channel 2  left and right on right joystick (channel 4 on reciever)
  ch3 = pulseIn(4, HIGH); // channel 3 up and down on right joystick   
  int ch4 = pulseIn(8, HIGH); //left and right on left joystick
  int forwardBackwardSwitch = pulseIn(10, HIGH);  //swtich to forward  (SFswitch ch5 on reciever)
  int chswitch = pulseIn(6, HIGH); // three way swtich for walking/armmode1/2  979-1480ish-1991 (three states) ch6 on reciever
  int speed_servo = map(ch1, 980, 2000, 1000, 8000); //this is the speed mapping CHANGE last argument to increase speed
  int servolinear1= map(ch1, 980, 2000, servolinear1Min, servolinear1Max); 
  int servolinear2= map(ch3, 980, 2000, servolinear2Min, servolinear2Max);  
  int servoWristPan = map(ch2, 980, 2000, servoWristPanMin, servoWristPanMax);
  int servoWristTilt = map(ch3, 980, 2000, servoWristTiltMin, servoWristTiltMax); 
  int servoContinuous = map(ch4, 980, 2000, servoContinuousMin, servoContinuousMax); // increase last 2 args to decrease dead zone
  int gripper = map(ch1, 980, 2000, 400, 200); // change 3rd and 4th var to change limits of gripper, 200 is closed 400 is open
  int servoCamPan = map(ch4, 980, 2000, 130, 600);// change 3rd and 4th var to change limits 
  int servoCamTilt = map(ch2, 980, 2000, 130, 600);
  int resetmotor = pulseIn(9,HIGH); // reset button on controller
  
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
  if(resetmotor > 1500) // reset button is pressed
  {
  	 Serial.print("p-");   
     Serial.println(0);
  }
  if(ch1 != 0){ // if reciever is not connected

	  if(chswitch < 1000) // if in "walking mode"
	  {
	    if(ch1 <= 1000){  // if the throttle is (near fully down send stop)
	        Serial.print("x-");   
	        Serial.println(speed_servo);
	    }
	    else{
	      if(forwardBackwardSwitch> 1900 && ch2 <= 1530 && ch2 >= 1450){ //backward drive mode
	      if(speed_servo > 1200){
	        
	        Serial.print("s-");     
	        Serial.println(speed_servo);
	      }
	      else{
	        Serial.print("x-");   
	        Serial.println(speed_servo);
	      }  
	      }
	      else if(forwardBackwardSwitch >= 1000 && forwardBackwardSwitch <= 1900) // camera control pan and tile mode
	      {

	      		Serial.print("u-");
	        	Serial.println(servoCamTilt);

	      	Serial.print("i-");
	        Serial.println(servoCamPan);
	      }
	      else if(forwardBackwardSwitch < 1000  && ch2 <= 1530 && ch2 >= 1450){ //driving forward mode
	        if(speed_servo > 1200){
	           Serial.print("w-");   
	        Serial.println(speed_servo);   
	      }
	      else{
	        Serial.print("x-");   
	        Serial.println(speed_servo);
	      }
	      
	      }
	      else if((ch2 > 1530 || ch2 < 1450) && (forwardBackwardSwitch < 1000)){ //turn left or right
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
      Serial.print("b-");
	    if(servoContinuous > 180 || servoContinuous < 140){   	
	      Serial.println(servoContinuous);
	    } else {
        Serial.println(servoContinuousMiddleVal);
	    }
	   Serial.print("c-");   
	   Serial.println(servolinear1);
	   Serial.print("v-");   
	   Serial.println(servolinear2); 

	  } 
	  else if(chswitch >= 1900) // in arm mode 2. (gripper and wrist pan/ wrist tilt)
	  { // basically higher precision mode
		  if(gripper < 380){
		    Serial.print("l-");
		    Serial.println(gripper);
		  }
		  if (servoWristPanMiddleVal + 15 < servoWristPan)
		   {
		   		servoWristPanWriteValue++;
		   		Serial.print("m-");   
		   		Serial.println(servoWristPanWriteValue); 
		   }
		   else if (servoWristPanMiddleVal - 15 > servoWristPan)
		   {
		   		servoWristPanWriteValue--;
		   		Serial.print("m-");   
		   		Serial.println(servoWristPanWriteValue); 
		   }

		   if (servoWristTiltMiddleVal + 15 < servoWristTilt)
		   {
		   		servoWristTiltWriteValue++;
		   		Serial.print("n-");   
		   		Serial.println(servoWristTiltWriteValue); 
		   }
		   else if (servoWristTiltMiddleVal - 15 > servoWristTilt)
		   {
		   		servoWristTiltWriteValue--;
		   		Serial.print("n-");   
		   		Serial.println(servoWristTiltWriteValue); 
		   }			   			   

	  }
  } 
  delay(100); //probably don't need this here, but you know, why not

}
      
