/*******************************************************
 *  Name: Tevon Hodge  
 *  Created: 7-31-2014
 *  Desc: Simulate tank drive for a two motor vehicle
           that is controlled by two potentiometers
 *  Notes: 
 ******************************************************/

/*************************
|  VARIABLE DECLARATIONS |
*************************/

int leftpot = 3;
int rightpot = 4;
int valueleft = 0;
int valueright = 0;
int ledpin = 13;
int SpeedA = 0;
int SpeedB = 0;
int MotorA = 3;
int MotorB = 11;



//Motor A is the motor on the left side of the bot
//Motor B is the motor on the right side of the bot

void setup (){
  pinMode (13,OUTPUT); //Direction motor A
  pinMode (12,OUTPUT); //Direction motor B
  pinMode (11,OUTPUT); //Motor B PWM
  pinMode (9,OUTPUT);  //Motor A Brake
  pinMode (8,OUTPUT);  //Motor B brake
  pinMode (3,OUTPUT);  //Motor A PWM
  Serial.begin(9600);
}

/*************
|  MAIN LOOP |
*************/

void loop(){

valueleft = analogRead(leftpot);  // Read left joystick value
valueright = analogRead(rightpot);  // Read Right joystick value
SpeedA= analogRead(leftpot);
SpeedB= analogRead(rightpot);



// Left wheel move forward
if (valueleft >= 520) { 
  digitalWrite(12, HIGH);
  SpeedA= map(SpeedA, 500,1023,0,255);//Scaling PWM Signal
  analogWrite(MotorA, SpeedA);
 
}

// Left wheel move backward
if (valueleft <= 500) {  
  digitalWrite(12, LOW);
  SpeedA= map(SpeedA, 499,0,0,255);//Scaling PWM Signal
  analogWrite(MotorA, SpeedA); 
  
}


// Right wheel move forward
if (valueright >= 515) { 
  digitalWrite (13, HIGH);
  SpeedB= map(SpeedB, 500,1023,0,255); //Scaling PWM Signal
  analogWrite(MotorB, SpeedB);
  
}

// Right wheel move backward
if (valueright <= 490) {  
  digitalWrite (13, LOW);
  SpeedB= map(SpeedB, 499,0,0,255); //Scaling PWM Signal
  analogWrite(MotorB, SpeedB);
}


// Left Wheel brake
if (valueleft < 520 && valueleft > 500){
  digitalWrite (9, HIGH);
  analogWrite (MotorA, 0);
} else {
  digitalWrite (9, LOW);
}

// Right  Wheel brake
if (valueright < 515 && valueright > 490) {
  digitalWrite (8, HIGH);
  SpeedA=0;
  analogWrite (MotorB, 0);
} else {
  digitalWrite (8, LOW);
}

}
