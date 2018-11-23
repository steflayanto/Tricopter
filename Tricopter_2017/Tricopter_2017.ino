#include <PinChangeInt.h>
#include <PID_v1.h>
#include <Servo.h>
#include <Wire.h>  //A4 - SDA, A5 - SCL
boolean a; //Needed to Initialize the Boolean Library, since it isnt done automatically for the header file
#include "Header.h"
Servo escL, escR, escB, yaw;


void setup() {
  setupPins();
  Wire.begin();
  Serial.begin(115200);
  setupOrientation();
  PIDsetup();
  calTimer = millis();
}
void serialEventRun() {}

void loop() {
  if (uncalibrated) {
    calibrate();
  } else {
    getOrientation();
    checkInput();
    //Allows direct input of control stick to center of thrust coordinates
    //centerOfThrust[0] = map(rollSetpoint, 1000, -1000, -173205, 173205);
    //centerOfThrust[1] = map(pitchSetpoint, -1000, 1000, -100000, 100000)+ COT_Y_TRIM;
    //yawAngle = map (yawSetpoint, -150, 150, -350, 350);
    calculatePID();
    convertMotorOutput();
    writeOutput();
    display();
  }
}

void display() {
  //printCombinedOrientationChart();
  //printLoopTime();
  //printAccelOrientation();
  //printGyroOrientation();
  //printFullRadioInputChart();
  //printRadioCalChart();
  //printRawRadioInputChart();
  //printRadioMicros();
  //printMicrosOutput();
  //printFlightControlOutput();
  //printRealOutput();
  //printGyroMAFVariances();
  //printGyroMAFChart();
  //printYawPropChart();
  //printCenterOfThrust();
  //printCombinedOrientation();
  //printPIDOutput();
/*  if (millis() - timer1 >= 50) {
    printPIDOutput();
    timer1 = millis();
  }*/
}

void checkInput() {
  readChannels();
  setMotorState();
  thrustSetpoint = constrain((ch2MAFAve() - ch2Cal), -1000, 1000);
  pitchSetpoint = map(constrain((ch3MAFAve() - ch3Cal), -1000, 1000), -1000, 1000, -45, 45);
  rollSetpoint = map(constrain((ch4MAFAve() - ch4Cal), -1000, 1000), -1000, 1000, -45, 45);
  yawSetpoint = map(constrain((ch1MAFAve() - ch1Cal), -1000, 1000), -1000, 1000, 150, -150); //Reverse Stick
  readSerialInput() ;
}

void setMotorState() {
  if (aux6 < 1100 && thrustSetpoint < -850) {            //This section a tad confusing. Aux 6 is a 3-state switch.
    //Setting 1                                          //Setting 1: Motors Disconnected. Don't even write any value to ESCs.(Also checks that throttle is down, to ensure no accidental cut off mid-flight.)
    motorsConnected = false;                             //Setting 2: Motors Connected, Disarmed. Write 0s to ESCs (Also checks that throttle is down, to ensure no accidental cut off mid-flight.)
    motorsArmed = false;                                 //Setting 3: Motors Connected, Armed. Normal Operation.
  } else if (aux6 < 1700 && thrustSetpoint < -850) {
    //Setting 2, < -850 is Stick Down
    motorsConnected = true;
    motorsArmed = false;
  } else if (aux6 > 1700) {
    //Setting 3
    motorsConnected = true;
    motorsArmed = true;
  }
}

void readSerialInput() {
  /*
     Serial Input Commands (All Require Newline):
     'c'                    = Force Calibrate
     'o'                    = Set Current Orientation as Desired Setpoint
     'x'                    = Kill Power
     'kp/ki/kd/kk,' [value] = Tune Preset PID kp/ki/kd to floatvalue. kk will just display current settings
     Automatic Mode and Manual mode both currently deactivated.
     'a'                    = Enter Automatic Mode
     'm, xThrust,yThrust'   = Enter Manual Mode, and set Thrust Settings

  */
  byte input, param; //For tuning

  if (Serial.available() > 0) {
    input = Serial.read();//Read Input Command
    if (input == 'k') {                     //PID Tuning - 'kp/ki/kd', value
      param = Serial.read();
      if (param == 'p') {
        pitchKp = Serial.parseFloat();
        //pitchRateKp = Serial.parseFloat();
      }
      if (param == 'i') {
        pitchKi = Serial.parseFloat();
        //pitchRateKi = Serial.parseFloat();
      }
      if (param == 'd') {
        pitchKd = Serial.parseFloat();
        //pitchRateKd = Serial.parseFloat();
      }

      if (Serial.read() == '\n') {
        Serial.println("Recieved");
        Serial.print("PID Values:\t");
        Serial.print(pitchKp, 4);
        //Serial.print(pitchRateKp, 4);
        Serial.print(", ");
        Serial.print(pitchKi, 4);
        //Serial.print(pitchRateKi, 4);
        Serial.print(", ");
        Serial.print(pitchKd, 4);
        //Serial.print(pitchRateKd, 4);
        Serial.println();
      }
    }

    if (input == 'c') {                    //Calibrate - 'c'
      Serial.println(F("Calibrate"));
      uncalibrated = true;
      if (Serial.read() == '\n') {
        Serial.println(F("Recieved"));
      }
    }
    if (input == 'o') {                    //Set as Setpoint - 'o'
      Serial.println(F("Set as Setpoint"));
      if (Serial.read() == '\n') {
        Serial.println(F("Recieved"));
        delay(250);
        rollSetpoint = roll;
        pitchRateSetpoint = pitch;
        yawSetpoint = gyroYaw;
        resetMatrix();
      }
    }
    if (input == 'x') {                     //Kill Power - 'x'
      Serial.println(F("Power Off"));
      if (Serial.read() == '\n') {
        Serial.println(F("Recieved"));
        limit = 3;
      }
    }
    if (input == 'b') {                     //Mode for Thrust Testing
      int weight;
      if (Serial.available() > 1) {
        weight = Serial.parseInt();
      }
      Serial.print(weight);
      Serial.print("\t");
      printMicrosOutput();
      if (Serial.read() == '\n') {
        Serial.println("Recieved");
      }
    }
    /*  if (input == 'a') {                     //Automatic Mode - 'a'
        Serial.println("Automatic Mode");
        pitchRatePID.SetMode(AUTOMATIC);
        pitchPID.SetMode(AUTOMATIC);
        rollPID.SetMode(AUTOMATIC);
        yawPID.SetMode(AUTOMATIC);
        PIDActive = true;
        if (Serial.read() == '\n') {
          Serial.println("Recieved");
        }
      }
      if (input == 'm') {                     //Manual Mode - 'm, xThrust,yThrust'
        pitchRatePID.SetMode(MANUAL);
        pitchPID.SetMode(MANUAL);
        rollPID.SetMode(MANUAL);
        yawPID.SetMode(MANUAL);
        PIDActive = false;
        limit = 75;
        if (Serial.available() > 1) {
          xInput = Serial.parseFloat();
          yInput = Serial.parseFloat();
        }
        else {
          xInput = 0.0;
          yInput = 0.0;
        }
        Serial.print("Manual Control\t");
        Serial.print(xInput);
        Serial.print(", ");
        Serial.print(yInput);
        Serial.println();
        if (Serial.read() == '\n') {
          Serial.println("Recieved");
          centerOfThrust[0] = constrain(xInput, -15, 15);
          centerOfThrust[1] = constrain(yInput, -15, 15);

        }
      }
    */
  }
}


void printYawPropChart() {
  Serial.print(yawAngle);
  Serial.print(F(","));
  Serial.print(map(yawAngle, -350, 350, 930, 1770));
  Serial.print(F(","));
  Serial.print(convertedSignal(backAdjustedThrust(backRealThrust, map(yawAngle, -350, 350, 930, 1770)), BACK_PROP_GRADIENT, BACK_PROP_INTERCEPT));
  Serial.print(F(","));
  Serial.println(backRealThrust);
}
