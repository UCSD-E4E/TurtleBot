/**
 * TURTLEBOT DRIVE CODE
 *
 * Copyright 2014 by Nathan Hui
 * Written for the TurtleBot project run by Engineers 4 Exploration at UCSD
 * www.e4e.ucsd.edu
 *
 * This code is designed to provide a single joystick interface for maneuvering
 * the TurtleBot.  This code constantly polls the joysticks for new values, and
 * calculates the proper motor outputs to effect the desired motion.  This
 * drive code is known as arcade style drive.  Pushing the stick full forward
 * causes movement directly forward.  Pushing the stick to the front left
 * causes the robot to turn to the left pivoting on the left tread.  Pushing
 * the stick directly to the left causes the robot to spin to the left in
 * place.  Pushing the stick to the rear left causes the robot to move to the
 * right in the reverse direction pivoting on the right tread.  Full rear
 * deflection causes the robot to move rearwards.  Right rear deflection causes
 * the robot to turn to the left in the reverse direction while pivoting on the
 * left read.  Full right deflection causes the robot to turn to the right in
 * position.  Forward right deflection causes the robot to turn to the right
 * while pivoting on the right tread.  This control scheme allows for fully
 * continuous driving, with no discontinuities in the motion of the robot as
 * the joystick is manipulated.
 *
 * This code requires the following hardware:
 * 	x axis joystick (analog) on Analog Pin 4
 * 	Y axis joystick (analog) on Analog Pin 3
 * 	R3 Arduino Motor Shield
 */

// Constants
#define Y_JOYSTICK	3
#define X_JOYSTICK	4
#define L_MOTOR_PWM	3
#define R_MOTOR_PWM	11
#define L_MOTOR_DIR	13
#define R_MOTOR_DIR	12
#define L_MOTOR_BRK	9
#define R_MOTOR_BRK	8

// Joystick Characteristics
#define X_JOY_LOW	0
#define Y_JOY_LOW	0
#define X_JOY_MID	506
#define Y_JOY_MID	510
#define X_JOY_HI	1023
#define Y_JOY_HI	1023
#define X_JOY_DBND	15
#define Y_JOY_DBND	10

// Variable Declarations
// Joystick Values (used for calculation)
int yJoystick = 0;
int xJoystick = 0;

// Internal representation of motor setpoint
int lSpd = 0;
int rSpd = 0;

/**
 * Setup script.  This function configures the left and right motor control
 * pins and configures the Serial connection at a baud rate of 57600 baud at
 * 8N1.
 */
void setup(){
	pinMode(L_MOTOR_DIR, OUTPUT);
	pinMode(L_MOTOR_PWM, OUTPUT);
	pinMode(L_MOTOR_BRK, OUTPUT);

	pinMode(R_MOTOR_DIR, OUTPUT);
	pinMode(R_MOTOR_PWM, OUTPUT);
	pinMode(R_MOTOR_BRK, OUTPUT);

	Serial.begin(57600);
}

/**
 * Control loop function.  This function is called contiuously during the
 * execution of the program.  See Arduino documentation for more details.
 *
 * This function constantly polls the joystick for a new XY value.  Upon
 * receiving the new value, the function scales the falue to represent a
 * percentage of full deflection.  Initial motor setpoint is set to be equal to
 * the magnitude of deflection.  The left or right setpoints are them modified
 * to allow for the desired turn radius as defined by the user.
 *
 * See www.tinorobotics.org/main_wiki/index.php?title=Single-joystick_Drive_Code
 * for explanation of full algorithm.
 */
void loop(){
	// Get new joystick values
	yJoystick = analogRead(Y_JOYSTICK);
	xJoystick = analogRead(X_JOYSTICK);
	
	// Scale and flip joystick values to fit [-100, 100]
	// TODO: write a new algorithm to take the characteristics of the joystick
	//		 and use them properly to scale deflection.
	yJoystick -= Y_JOY_MID;
	yJoystick = yJoystick / (Y_JOY_HI - Y_JOY_LOW) * 200;
	xJoystick -= X_JOY_MID;
	xJoystick = xJoystick / (X_JOY_HI - X_JOY_LOW) * 200;
	
	// Begin mapping!
	lSpd = (yJoystick == 0 ? 1 : sign(yJoystick)) * mag(xJoystick, yJoystick);
	rSpd = (yJoystick == 0 ? 1 : sign(yJoystick)) * mag(xJoystick, yJoystick);

	// Pretrim
	if(abs(lSpd) >= 100){
		lSpd = 100 * sign(lSpd);
	}
	if(abs(rSpd) >= 100){
		rSpd = 100 * sign(rSpd);
	}


	if(abs(yJoystick) <= Y_JOY_DBND && abs(xJoystick) <= X_JOY_DBND){	// Set deadband
		lSpd = rSpd = 0;

	}else if(abs(yJoystick) >= abs(xJoystick)){	// if mostly forward
		if(xJoystick < 0){
			lSpd *= 1 + xJoystick / 100.0;
		}else{
			rSpd *= 1 - xJoystick / 100.0;
		}
		if(yJoystick < 0){	// fix reverse direction
			int temp = lSpd;
			lSpd = rSpd;
			rSpd = temp;
		}
	}else{	// if turning
		if(xJoystick < 0 && yJoystick < 0){
			rSpd *= (yJoystick + 100) / 100.0;
			lSpd *= -1;
		}else if(xJoystick < 0 && yJoystick >= 0){
			lSpd *= (yJoystick - 100) / 100.0;
			lSpd *= -1;
			rSpd *= -1;
		}else if(xJoystick > 0 && yJoystick < 0){
			rSpd *= (yJoystick + 100) / 100.0;
		}else if(xJoystick > 0 && yJoystick >= 0){
			lSpd *= (yJoystick - 100) / 100.0;
		}
	}

	// Trim speed
	if(abs(lSpd) >= 100){
		lSpd = 100 * sign(lSpd);
	}
	if(abs(rSpd) >= 100){
		rSpd = 100 * sign(rSpd);
	}

	// Add motor deadband
	if(abs(lSpd) <= 10){
		lSpd = 0;
	}
	if(abs(rSpd) <= 10){
		rSpd = 0;
	}

	// Set motor speeds
	// Left Motor
	if(lSpd < 0){
		digitalWrite(L_MOTOR_DIR, HIGH);
		digitalWrite(L_MOTOR_BRK, LOW);
		analogWrite(L_MOTOR_PWM, map(abs(lSpd), 0, 100, 0, 255));
	}else if(lSpd > 0){
		digitalWrite(L_MOTOR_DIR, LOW);
		digitalWrite(L_MOTOR_BRK, LOW);
		analogWrite(L_MOTOR_PWM, map(abs(lSpd), 0, 100, 0, 255));
	}else{
		digitalWrite(L_MOTOR_BRK, HIGH);
		analogWrite(L_MOTOR_PWM, 0);
	}

	// Right Motor
	if(rSpd < 0){
		digitalWrite(R_MOTOR_DIR, HIGH);
		digitalWrite(R_MOTOR_BRK, LOW);
		analogWrite(R_MOTOR_PWM, map(abs(rSpd), 0, 100, 0, 255));
	}else if(rSpd > 0){
		digitalWrite(R_MOTOR_DIR, LOW);
		digitalWrite(R_MOTOR_BRK, LOW);
		analogWrite(R_MOTOR_PWM, map(abs(rSpd), 0, 100, 0, 255));
	}else{
		digitalWrite(R_MOTOR_BRK, HIGH);
		analogWrite(R_MOTOR_PWM, 0);
	}

	delay(50);
	return;
}

// Convenience functions
/**
 * Returns the sign of val.  If val is positive, return 1, else return -1.
 */
int sign(int val){
	return abs(val) / val;
}

/**
 * Returns the magnitude of the 2D integer vector as an integer.  Uses the
 * Pythagorean theorem to calculate the magnitude.
 */
int mag(int xVal, int yVal){
	return sqrt(xVal * xVal + yVal * yVal);
}

