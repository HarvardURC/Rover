
int ch1; // Here's where we'll keep our channel values
int ch2;
int ch3;
int ch5;
int chswitch;
int forwardCutoff = 1400;
int backwardCutoff = 1000;
int leftCutoff = 1450;
int rightCutoff = 1530;




void setup() {

  pinMode(2, INPUT); // Set our input pins as such
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);

  Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {

  ch1 = pulseIn(2, HIGH); // throttle
  ch2 = pulseIn(3, HIGH); // channel 2  left and right on right joystick
  ch3 = pulseIn(4, HIGH); // channel 3 up and down on right joystick 
  ch5 = pulseIn(5, HIGH);  //swtich to forward 
  chswitch = pulseIn(6, HIGH); // three way swtich for grabber
  int speed = map(ch1, 980, 2000, 1000, 3000);
  int servo1 = map(ch1, 980, 2000, 0, 180);
  int servo2 = map(ch2, 980, 2000, 0, 180); 
  // Serial.print("Channel 1 ");
  // Serial.print(ch1);
  // Serial.print("Channel 2 ");
  // Serial.print(ch2);
  // Serial.print("Channel 3 ");
  // Serial.print(ch3);
  Serial.print("chswitch ");
  Serial.println(chswitch);
  if(chswitch >= 1900){
  		Serial.print("x-");   
		Serial.println(servo1);
  }
  else{
	  if(ch5> 1900 && ch2 <= 1530 && ch2 >= 1450){
		if(speed > 1200){
		  
		  Serial.print("s-");   
		  Serial.println(speed);
		}
		else{
		  Serial.print("x-");   
		  Serial.println(speed);
		}  
	  }
	  else if(ch5 < 1500  && ch2 <= 1530 && ch2 >= 1450){
	    if(speed > 1200){
		     Serial.print("w-");   
		  Serial.println(speed);   
		}
		else{
		  Serial.print("x-");   
		  Serial.println(speed);
		}
		
	  }
	  else if(ch2 > 1530 || ch2 < 1450){
		  if(ch2 > rightCutoff){
			Serial.print("d-");   
		  Serial.println(speed); 
		  }
		  else if(ch2 < leftCutoff){
			Serial.print("a-");   
		  Serial.println(speed);  
		  }
		  else{
		  Serial.print("x-");   
		  Serial.println(speed);
		  }
	  }
	}
  delay(100);
              
}
			
