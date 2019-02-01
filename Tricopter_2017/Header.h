#include <stdbool.h>

//STATIC SETTINGS

//Pin Assignment
#define CH_ONE_PIN 3    //Radio Channel input pins (based on the Turnigy 9X receiver)
#define CH_TWO_PIN 4    
#define CH_THREE_PIN 5  
#define CH_FOUR_PIN 7   
#define AUX5_IN_PIN 8   //These are the two channels for auxiliary inputs on the Turnigy 9X receiver
#define AUX6_IN_PIN 2   
#define escLPin 6       //Left ESC signal output pin (all ESCs behave like servo objects)
#define escRPin 9       //Right ESC signal output pin 
#define escBPin 10      //Back ESC signal output pin 
#define yawPin 11       //Yaw servo signal output pin

//Safety
int limit = 1000; //this can be changed to limit thrust for safety (0-1000 -> 0.0%-100.0%)

//Trim Values. Set through trial and error.
#define COT_X_TRIM 0
#define COT_Y_TRIM -20000
#define YAW_TRIM -15

//MAF Settings
#define ACCEL_MAF_LENGTH 5
#define GYRO_MAF_LENGTH 10
#define RADIO_MAF_LENGTH 10

#define ACCEL_FILTER 0x3 //[0x5,0x4,0x3,0x2,0x1,0x0] is [32Hz,64Hz,125Hz,250Hz,500Hz,1000Hz] sample rate respectively.

//Gyro Settings
#define MOUNTING -1  //Set -1 if Gyro is on top of breadboard. Set 1 if Gyro is under breadboard.
#define CAL_DURATION 10000 //Duration of calibration period in milliseconds

//Motor Output Settings (Used to Calculate Motor-specific Signals for Desired Thrusts). Determined Through Experimentation
//Originally LEFT_PROP_GRADIENT 2.6008300412, LEFT_PROP_INTERCEPT 775.941460576
#define LEFT_PROP_GRADIENT 2.825 //Experimentally Determined.
#define LEFT_PROP_INTERCEPT 725
#define RIGHT_PROP_GRADIENT 2.25//Originally 1.98
#define RIGHT_PROP_INTERCEPT 724.7349052142
#define BACK_PROP_GRADIENT 2.425     //2.4537599172
#define BACK_PROP_INTERCEPT 720     //712.7239283580

//PID
#define PIDSampleTime 5

//PID Settings
//double pitchRateKp = 0.5, pitchRateKi = 0.75, pitchRateKd = 0.15;
double pitchRateKp = 2, pitchRateKi = 0.6, pitchRateKd = 0.05;

//double rollRateKp = 2.1, rollRateKi = 2.75, rollRateKd = 0.25;
double rollRateKp = 2.75, rollRateKi = 2, rollRateKd = 0.15;

//double yawKp = 0.525, yawKi = 0.5, yawKd = 0.025;
double yawKp = 2.5, yawKi = 1.5, yawKd = 0.05;

//double pitchKp = 4, pitchKi = 0.4, pitchKd = 0.2;
double pitchKp = 4, pitchKi = 1.25, pitchKd = 0.1 ;

//double rollKp = 2 , rollKi = 1.8, rollKd = 0.01;
double rollKp = 6.5 , rollKi = 1.5, rollKd = 0.25;



//GLOBAL VARIABLES

//Boolean Modes
boolean PIDActive;
boolean standardMode=true; //If False then Copter is in RateMode
boolean motorsArmed;
boolean motorsConnected;
boolean uncalibrated = true;

//Flight Control
long centerOfThrust[2] = { //centerOfThrust is scaled up by 1000.
  0, 0
};
int thrustSetpoint;
double pitchRateSetpoint, pitchSetpoint, rollRateSetpoint, rollSetpoint, yawSetpoint;

//PID IO
double pitchRateInput, pitchRateOutput;
double pitchInput, pitchOutput;
double rollRateInput, rollRateOutput;
double rollInput, rollOutput;
double yawInput, yawOutput;

//Timers
unsigned long startTime;
unsigned long calTimer;
unsigned long blinkTimer; //For the Blink Function
unsigned long timer1 = 0; //Used in Display Commands

//Gyroscope Global Data Variables
float xRawRate, yRawRate, zRawRate, rawTemp; //Used in reading data from gyro into MAFs
float hCal = 0, pCal = 0, rCal = 0, tempOffset = 0; //Gyro Calibration Values

int ch1Cal, ch2Cal, ch3Cal, ch4Cal; //Radio Calibration Values

//Sensor Output Variables
float pitch, roll;
float gyroYaw, gyroPitch, gyroRoll;
float accelX, accelY, accelZ;
uint16_t ch1, ch2, ch3, ch4, aux5, aux6;

//Motor Output Variables
int thrustFloor, leftRealThrust, rightRealThrust, backRealThrust, yawAngle; //backAdjustedThrust

//Vector Variables. Vector Maths will be in integer math up to 6 digits. example: 1.5 represented as 1500. Divide by 1000 to get real value
int magL, magR, magB;
long leftPropVector[2] = { //Actual values are (-cos30,sin30) both divided by (cos30+sin30) to yield unit vector
  -(634), 366
};
long rightPropVector[2] = { //Actual values are (cos30,sin30) both divided by (cos30+sin30) to yield unit vector
  634, 366
};
//backPropVector is simply -1, and is simplified in the convertMotorOutput tab. Hence the bottom being commented out below to save space.
//long backPropVector[2] = {0,-1000}; //Actual values are (0,1) both divided by (0+1) to yield unit vector.
