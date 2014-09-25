
// Constants
#define Y_JOYSTICK 3
#define X_JOYSTICK 4
#define L_MOTOR_PWM 3
#define R_MOTOR_PWM 11
#define L_MOTOR_DIR 13
#define R_MOTOR_DIR 12
#define L_MOTOR_BRK 9
#define R_MOTOR_BRK 8

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

	
