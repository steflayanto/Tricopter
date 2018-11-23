#include <stdbool.h>

//Pin Assignment
#define CH_ONE_PIN 3
#define CH_TWO_PIN 4
#define CH_THREE_PIN 5
#define CH_FOUR_PIN 6
#define AUX_IN_PIN 7
#define escLPin 8
#define escRPin 9
#define escBPin 10
#define headingPin 11

//Constant Settings
/*
#define width 240  //Width of front of Tricopter
#define sensDist 50  //Distance of sensor from front of Tricopter
*/
float centerOfThrust[2] = { 
  0,0};

//Accelerometer Settings
const int accelMAFLength = 15;

//Gyro Settings 
const int mounting=-1;  //Set -1 if Gyro is on top of breadboard. Set 1 if Gyro is under breadboard.
const int gyroMAFLength=10;

//PID Settings
float kp=0,ki=0,kd=0; //INPUT PID Values

double consKp=0.25, consKi=0.02, consKd=0.01;

double pitchKp=0, pitchKi=0, pitchKd=0;

double headingKp=0.2, headingKi=0.2, headingKd=0.01;

double pitchRateKp=0.1, pitchRateKi=0, pitchRateKd=0;
double rollRateKp=0.1, rollRateKi=0, rollRateKd=0;

int sampleTime = 10;

//Center of Thrust trim values
float cotXTrim = 0.0;
float cotYTrim = 10.0;


//Orientation Output
float pitch,roll;

//Gyro Output
float gyroHeading, gyroPitch, gyroRoll;

//Controlled Settings
int limit=90;
float totalThrust;
double pitchRateSetpoint, pitchSetpoint, rollRateSetpoint, rollSetpoint, headingSetpoint;
boolean autoControl = true;
int headingTrim = 50;

//PID IO
double pitchRateInput, pitchRateOutput;
double pitchInput, pitchOutput;
double rollRateInput, rollRateOutput;
double rollInput, rollOutput;
double headingInput, headingOutput;

//Clock Variables
unsigned long startTime;
unsigned long currentTime;
unsigned long calTimer;
unsigned long timer1 = 0;

//Gyroscope
//Data Variables
boolean uncalibrated=true;
float xRawRate, yRawRate, zRawRate, rawTemp; //Used in reading data from gyro into MAFs

//Rate PID
PID pitchRatePID(&pitchRateInput, &pitchRateOutput, &pitchRateSetpoint, consKp, consKi, consKd, DIRECT);
PID rollRatePID(&rollRateInput, &rollRateOutput, &rollRateSetpoint, consKp, consKi, consKd, DIRECT);

//Attitude PID
PID pitchPID(&pitchInput, &pitchOutput, &pitchSetpoint, consKp, consKi, consKd, DIRECT);
PID rollPID(&rollInput, &rollOutput, &rollSetpoint, consKp, consKi, consKd, DIRECT);
PID headingPID(&headingInput, &headingOutput, &headingSetpoint, headingKp, headingKi, headingKd, REVERSE);

//Radio Reciever
boolean radioControl = true;
// Assign your channel out pins
/*#define CH_OUT_PIN 5
 #define STEERING_OUT_PIN 6
 #define AUX_OUT_PIN 7
 */

// These bit flags are set in bUpdateFlagsShared to indicate which
// channels have new signals
#define CH_ONE_FLAG 1
#define CH_TWO_FLAG 2
#define CH_THREE_FLAG 4
#define CH_FOUR_FLAG 8
#define AUX_FLAG 16

// holds the update flags defined above
volatile uint8_t bUpdateFlagsShared;

volatile uint16_t unCh1InShared;
volatile uint16_t unCh2InShared;
volatile uint16_t unCh3InShared;
volatile uint16_t unCh4InShared;
volatile uint16_t unAuxInShared;

uint32_t ulCh1Start;
uint32_t ulCh2Start;
uint32_t ulCh3Start;
uint32_t ulCh4Start;
uint32_t ulAuxStart;

//Values
uint16_t ch1, ch2, ch3, ch4, aux;
uint16_t ch1Cal,ch3Cal,ch4Cal; //Calibration Values

int _limit; //Temporary '_limit' which is by default set to value for int 'limit'
