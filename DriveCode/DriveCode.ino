
// Constants
#define Y_JOYSTICK	3
#define X_JOYSTICK	4
#define L_MOTOR_PWM	3
#define R_MOTOR_PWM	11
#define L_MOTOR_DIR	13
#define R_MOTOR_DIR	12
#define L_MOTOR_BRK	9
#define R_MOTOR_BRK	8
#define X_JOY_FLIP	1
#define Y_JOY_FLIP	1
#define X_JOY_LOW	0
#define Y_JOY_LOW	0
#define X_JOY_MID	512
#define Y_JOY_MID	512
#define X_JOY_HI	1023
#define Y_JOY_HI	1023
#define X_JOY_DBND	15
#define Y_JOY_DBND	10

// Variable Declarations
int yJoystick = 0;
int xJoystick = 0;

int lSpd = 0;
int rSpd = 0;

void setup(){
	pinMode(L_MOTOR_DIR, OUTPUT);
	pinMode(L_MOTOR_PWM, OUTPUT);
	pinMode(L_MOTOR_BRK, OUTPUT);

	pinMode(R_MOTOR_DIR, OUTPUT);
	pinMode(R_MOTOR_PWM, OUTPUT);
	pinMode(R_MOTOR_BRK, OUTPUT);

	Serial.begin(57600);
}

void loop(){
	yJoystick = analogRead(Y_JOYSTICK);
	xJoystick = analogRead(X_JOYSTICK);
	
	// Scale and flip joystick values to fit [-100, 100]
	
	// Begin mapping!
	lSpd = (yJoystick == 0 ? 1 : sign(yJoystick)) * mag(xJoystick, yJoystick);
	rSpd = (yJoystick == 0 ? 1 : sign(yJoystick)) * mag(xJoystick, yJoystick);

	if(abs(yJoystick) <= Y_JOY_DBND && abs(xJoystick) <= X_JOY_DBND){	// Set deadband
		lSpd = rSpd = 0;
	}else if(abs(yJoystick) >= abs(xJoystick)){	// if mostly forward
		if(xJoystick < 0){
			lSpd *= 1 + xJoystick / 100.0;
		}else{
			rSpd *= 1 - xJoystick / 100.0;
		}
	}else{	// if turning
		if(xJoystick < 0){
			lSpd *= -1 * (1 + yJoystick / 100.0);
		}else{
			rSpd *= -1 * (1 + yJoystick / 100.0);
		}
	}
	if(yJoystick < 0){	// fix reverse direction
		int temp = lSpd;
		lSpd = rSpd;
		rSpd = temp;
	}

	// Set motor speeds
	// Left Motor
	if(lSpd > 0){
		digitalWrite(L_MOTOR_DIR, HIGH);
		digitalWrite(L_MOTOR_BRK, LOW);
		analogWrite(L_MOTOR_PWM, map(abs(lSpd), 0, 100, 0, 255));
	}else if(lSpd < 0){
		digitalWrite(L_MOTOR_DIR, LOW);
		digitalWrite(L_MOTOR_BRK, LOW);
		analogWrite(L_MOTOR_PWM, map(abs(lSpd), 0, 100, 0, 255));
	}else{
		digitalWrite(L_MOTOR_BRK, HIGH);
		analogWrite(L_MOTOR_PWM, 0);
	}

	// Right Motor
	if(rSpd > 0){
		digitalWrite(R_MOTOR_DIR, HIGH);
		digitalWrite(R_MOTOR_BRK, LOW);
		analogWrite(R_MOTOR_PWM, map(abs(lSpd), 0, 100, 0, 255));
	}else if(rSpd < 0){
		digitalWrite(R_MOTOR_DIR, LOW);
		digitalWrite(R_MOTOR_BRK, LOW);
		analogWrite(R_MOTOR_PWM, map(abs(lSpd), 0, 100, 0, 255));
	}else{
		digitalWrite(R_MOTOR_BRK, HIGH);
		analogWrite(R_MOTOR_PWM, 0);
	}
}

// TODO: implement sign(int)
int sign(int val){
	return abs(val) / val;
}
// TODO: implement mag(int, int)
int mag(int xVal, int yVal){
	return sqrt(xVal * xVal + yVal * yVal);
}

