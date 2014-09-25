
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
#define X_JOY_MID	506
#define Y_JOY_MID	510
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
	yJoystick -= 512;
	yJoystick = yJoystick / 512.0 * 100;
	xJoystick -= 512;
	xJoystick = xJoystick / 512.0 * 100;
	
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

int sign(int val){
	return abs(val) / val;
}
int mag(int xVal, int yVal){
	return sqrt(xVal * xVal + yVal * yVal);
}

