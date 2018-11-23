#include <PinChangeInt.h>
#include <PID_v1.h>
#include <Servo.h>
#include <Wire.h>  //A4 - SDA, A5 - SCL
boolean a; //Needed to Initialize the Boolean Library, since it isnt done automatically for the header file
#include "Header.h"
Servo escL, escR, escB, heading;


void setup() {
  escL.attach(escLPin);
  escR.attach(escRPin);
  escB.attach(escBPin);
  heading.attach(headingPin);
  Wire.begin();
  Serial.begin(9600);
  setupRadio();
  setupOrientation();
  PIDsetup();
  //  setupESCs();
  calTimer = millis();
}

void loop() {
  startTime = millis();
  getOrientation();
  readChannels();
  checkForInput();
  if (autoControl) {
    calculatePID();
  }
  convertOutput();
  writeOutput();
  display();
  currentTime = startTime - millis();
  Serial.print("Loop Time:\t");
  Serial.println(currentTime);
}

void display() {
  //printPIDOutput();
  //printRealOutput();
  //printAccelGyroSerialChart();
  //printRateModeSerialChart();
  //printCombinedOrientationSerialChart();
  //printFlightControlOutput();
  //printFullRadioInput();
  //printCenterOfThrust();

}

void checkForInput() {
  byte input, param;
  float xInput, yInput;
  if (radioControl) {
    if (aux > 1750) {
      _limit = 2;
    } else {
      _limit = limit;
    }
    totalThrust = int(constrain(map(ch2, 1080, 1900, 0, 1000), 0, 1000) / 10.0); //Maps total thrust to int of 0 - 100

    //Allows direct input of control stick to center of thrust coordinates
    //    centerOfThrust[0]=(map(ch4,1000+ch4Cal,1900+ch4Cal,-1500,1500)/100.0);
    //    centerOfThrust[1]=(map(ch3,1000+ch3Cal,1900+ch3Cal,1500,-1500)/100.0);
    //    headingOutput=

    //Normal Operation where stick inputs feed to PID controllers. -100 to 100
    pitchSetpoint = int(map(ch3, 1000 + ch3Cal, 1900 + ch3Cal, 1000, -1000) / 10.0);
    rollSetpoint = int(map(ch4, 1000 + ch1Cal, 1900 + ch4Cal, -1000, 1000) / 10.0);
    headingSetpoint = int(map(ch1, 1000 + ch4Cal, 1900 + ch1Cal, -1000, 1000) / 10.0);

  }
  //else{
  /*  if (Serial.available()>0) {
      input = Serial.read();//Read Input Command

        if (input=='c'){                       //Calibrate - 'c'
        Serial.println("Calibrate");
        if (Serial.read()=='\n') {
          Serial.println("Recieved");
        }
      }
      if(input=='o') {                       //Set as Setpoint - 'o'
        Serial.println("Set as Setpoint");
        if (Serial.read()=='\n') {
          Serial.println("Recieved");
          delay(250);
          rollSetpoint=roll;
          pitchRateSetpoint=pitch;
          headingSetpoint=gyroHeading;
          resetMatrix();
        }
      }
      if (input=='x') {                       //Kill Power - 'x'
        Serial.println("Power Off");
        if (Serial.read()=='\n') {
          Serial.println("Recieved");
          limit=3;
        }
      }
      if (input=='a') {                       //Automatic Mode - 'a'
        Serial.println("Automatic Mode");
        pitchRatePID.SetMode(AUTOMATIC);
        pitchPID.SetMode(AUTOMATIC);
        rollPID.SetMode(AUTOMATIC);
        headingPID.SetMode(AUTOMATIC);
        autoControl=true;
        if (Serial.read()=='\n') {
          Serial.println("Recieved");
        }
      }
      if (input=='m') {                       //Manual Mode - 'm, xThrust,yThrust'
        pitchRatePID.SetMode(MANUAL);
        pitchPID.SetMode(MANUAL);
        rollPID.SetMode(MANUAL);
        headingPID.SetMode(MANUAL);
        autoControl=false;
        limit=75;
        if (Serial.available()>1) {
          xInput = Serial.parseFloat();
          yInput = Serial.parseFloat();
        }
        else{
          xInput = 0.0;
          yInput = 0.0;
        }
        Serial.print("Manual Control\t");
        Serial.print(xInput);
        Serial.print(", ");
        Serial.print(yInput);
        Serial.println();
        if (Serial.read()=='\n') {
          Serial.println("Recieved");
          centerOfThrust[0]=constrain(xInput,-15,15);
          centerOfThrust[1]=constrain(yInput,-15,15);

        }
      }
    }

    if (input=='k') {                       //PID Tuning - 'kp/ki/kd', value
      param = Serial.read();
      if (param=='p') {
        kp = Serial.parseFloat();
        pitchKp=kp;
      }
      if (param=='i') {
        ki = Serial.parseFloat();
        pitchKi=ki;
      }
      if (param=='d') {
        kd = Serial.parseFloat();
        pitchKd=kd;
      }

      Serial.print("PID Values:\t");
      Serial.print(kp,4);
      Serial.print(", ");
      Serial.print(ki,4);
      Serial.print(", ");
      Serial.print(kd,4);
      Serial.println();
      if (Serial.read()=='\n') {
        Serial.println("Recieved");
        Serial.println(pitch);
        Serial.println(pitchSetpoint);

      }
    }*/
}


void setupESCs() {
  Serial.println("Send Any Value to Initialize Motors");
  while (Serial.available() == 0) {
    int _a = 1;
    escL.writeMicroseconds(map(_a, 0, 100, 700, 2000));
    escR.writeMicroseconds(map(_a, 0, 100, 700, 2000));
    escB.writeMicroseconds(map(_a, 0, 100, 700, 2000));
    //    Serial.println(map(_a,0,100,700,2000));    //Print Outputs To Serial Monitor During Setup
  }
  Serial.flush();
}









