/* DISPLAY FUNCTIONS INDEX

  GENERAL
  printLoopTime()  Print time taken per loop
  printCalibrationTimer()  Function that displays the setup calibration timer
  printCalData() Print calibration parameters
  printCombinedOrientation() Orientation

  ACCELEROMETER
  printAccelOrientation() Orientation based on only Accel
  printAccelValues()    Print current accel Data X=x,Y=y,Z=z

  GYROSCOPE
  printGyroOrientation()  Orientation based on only Gyro
  printRawGyroMAF()
  printGyroRates()
  printGyroVariances()
  printMatrix()(found on tab 'F')
  printTransformativeMatrix (found on tab 'F')

  FLIGHT CONTROL
  printPIDInput()
  printOutput()
  printFlightControlOutput()
  printRealOutput()
  printMicrosOutput()
  printFullRadioInput() Only prints at 20Hz
  printCenterOfThrust() Only prints at 20Hz
  printRadioMicros()

  PRINTING TO CHART
  printAccelGyroChart()
  printCombinedOrientationChart()
  printAccelChart()
  printGyroChart()
  printGyroMAFChart()
  printRateModeChart()
  printGyroMAFVariances()
  printFullRadioInputChart()
  printCenterOfThrustChart()
*/

unsigned long lastSec = 0; //For Calibration Timer

//General
void printLoopTime() {
  Serial.print(F("Loop Duration(microsec):\t"));
  Serial.println(timer1);
  timer1 = micros() - startTime;
  startTime = micros();
}

void printCalibrationTimer() {
  if ((((millis() - calTimer) - lastSec) >= (unsigned long)1000)) {
    Serial.print(F("Calibrating: "));
    Serial.print((CAL_DURATION - lastSec) / 1000);
    Serial.println(F(" seconds remaining..."));
    lastSec += 1000;
  }
}

void printCalData() {
  Serial.print(F("Yaw Gyro: "));
  Serial.print(hCal);
  Serial.print(F("\tPitch Gyro: "));
  Serial.print(pCal);
  Serial.print(F("\tRoll Gyro: "));
  Serial.print(rCal);
  Serial.println();
}

void printCombinedOrientation() {
  Serial.print(F("Yaw: "));
  Serial.print(gyroYaw);
  Serial.print(F("\tPitch: "));
  Serial.print(pitch);
  Serial.print(F("\tRoll:"));
  Serial.print(roll);
  Serial.println();
}

//Accelerometer
void printAccelOrientation() {      //Orientation based only on Accel Data
  Serial.println(F("Yaw\tPitch\tRoll"));
  Serial.print(F("NA"));
  Serial.print(F("\t"));
  Serial.print(pitchAccelMAF());
  Serial.print(F("\t"));
  Serial.println(rollAccelMAF());
}

void printAccelValues() {     //Print current accel Data X=x,Y=y,Z=z
  Serial.print(F("X="));
  Serial.print(accelX);
  Serial.print(F(", Y="));
  Serial.print(accelY);
  Serial.print(F(", Z="));
  Serial.println(accelZ);
}

//Gyroscope
void printGyroOrientation() { //Orientation based on only Gyro
  Serial.println("Yaw\tPitch\tRoll");
  Serial.print(gyroYaw);
  Serial.print(F("\t"));
  Serial.print(gyroPitch);
  Serial.print(F("\t"));
  Serial.println(gyroRoll);
  //Serial.print(F("\t"));
  //Serial.println(temp());
}

void printRawGyroMAF() {
  Serial.println(F("H_rate\tP_rate\tR_rate"));
  Serial.print(hRawMAFAve());
  Serial.print(F("\t"));
  Serial.print(pRawMAFAve());
  Serial.print(F("\t"));
  Serial.print(rRawMAFAve());
  Serial.print(F("\t"));
  Serial.println(rawTemp);
}

void printGyroRates() {
  Serial.println(F("H rate\tP rate\tR rate"));
  Serial.print(hRate());
  Serial.print(F("\t"));
  Serial.print(pRate());
  Serial.print(F("\t"));
  Serial.println(rRate());
  /*Serial.print(F("\t"));
    Serial.println(temp());*/
}

//printMatrix();(found on tab 'F')
//printTransformativeMatrix();(found on tab 'F')


//Flight Control

void printPIDInput() {
    Serial.print(F("pitchRate:"));
    Serial.print(pitchRateInput);
    Serial.print(F("\trollRate:"));
    Serial.print(rollRateInput);
    Serial.print(F("\tRoll:"));
    Serial.print(rollInput);
    Serial.print(F("\tYaw:"));
    Serial.println(yawInput);
}

void printPIDOutput() {
  /*
    Serial.print(F("\tMeasured Yaw:"));
    Serial.print(gyroYaw);
    Serial.print(F("\tYaw Setpoint:"));
    Serial.print(yawSetpoint);
    Serial.print(F("\tyawOutput:"));
    Serial.println(yawOutput);
    Serial.print(F("\tYaw Angle:"));
    Serial.println(yawAngle);
    */
    Serial.print(F("\tMeasured Roll:"));
    Serial.print(roll);
    Serial.print(F("\tRoll Setpoint:"));
    Serial.print(rollSetpoint);
    Serial.print(F("\tRoll Output:"));
    Serial.println(rollOutput);
    Serial.println();
 }

void printPIDRateModeOutput() {
  Serial.print(F("pitchRate:"));
  Serial.print(pitchRateOutput);
  Serial.print(F("\trollRate:"));
  Serial.print(rollRateOutput);
  Serial.print(F("\tYaw:"));
  Serial.println(yawAngle);
}


void printFlightControlOutput() {
  Serial.print(F("Left:"));
  Serial.print(magL);
  Serial.print(F("\tRight:"));
  Serial.print(magR);
  Serial.print(F("\tBack:"));
  Serial.print(magB);
  Serial.print(F("\tYaw:"));
  Serial.println(yawAngle);
}

void printRealOutput() {
  Serial.print(F("Left:"));
  Serial.print(leftRealThrust);
  Serial.print(F("\tRight:"));
  Serial.print(rightRealThrust);
  Serial.print(F("\tBack:"));
  Serial.print(backRealThrust);
  Serial.print(F("\tYaw:"));
  Serial.println(yawAngle);
}

void printMicrosOutput() {
  Serial.print(F("Left:"));
  Serial.print(convertedSignal(leftRealThrust, LEFT_PROP_GRADIENT, LEFT_PROP_INTERCEPT));
  Serial.print(F("\tRight:"));
  Serial.print(convertedSignal(rightRealThrust, RIGHT_PROP_GRADIENT, RIGHT_PROP_INTERCEPT));
  Serial.print(F("\tBack:"));
  Serial.print(convertedSignal(backAdjustedThrust(backRealThrust, map(yawAngle, -350, 350, 980, 1820)), BACK_PROP_GRADIENT, BACK_PROP_INTERCEPT));
  //Serial.print(convertedSignal(backRealThrust,BACK_PROP_GRADIENT,BACK_PROP_INTERCEPT)); //Without backAdjustedThrust Function
  Serial.print(F("\tYaw:"));
  Serial.println(map(yawAngle, -450, 450, 940, 1760));
}

void printFullRadioInput() {
  if (millis() - timer1 >= 50) {
    Serial.print(F("Thrust: "));
    Serial.println(thrustSetpoint);
    Serial.print(F("Pitch: "));
    Serial.println(pitchSetpoint);
    Serial.print(F("Roll: "));
    Serial.println(rollSetpoint);
    Serial.print(F("Yaw: "));
    Serial.println(yawSetpoint);
    Serial.println();
    timer1 = millis();
  }
}

void printCenterOfThrust() {
  if (millis() - timer1 >= 50) {
    Serial.print(F("Thrust: "));
    Serial.print(thrustFloor);
    Serial.print(F("\tX-Axis: "));
    Serial.print(centerOfThrust[0]);
    Serial.print(F("\tY-Axis: "));
    Serial.print(centerOfThrust[1]);
    Serial.print(F("\tYaw Angle: "));
    Serial.print(yawAngle);
    Serial.println();
    timer1 = millis();
  }
}

void printRadioMicros() {
  Serial.print(F("CHANNEL ONE: "));
  Serial.print(ch1);
  Serial.print(F("\tCHANNEL TWO: "));
  Serial.print(ch2);
  Serial.print(F("\tCHANNEL THREE: "));
  Serial.print(ch3);
  Serial.print(F("\tCHANNEL FOUR: "));
  Serial.print(ch4);
  Serial.print(F("\tAUX ONE: "));
  Serial.print(aux5);
  Serial.print(F("\tAUX TWO: "));
  Serial.print(aux6);
  Serial.println();
}

//Print to Chart

void printAccelGyroChart() {
  Serial.print(pitchAccelMAF());
  Serial.print(F(","));
  Serial.print(rollAccelMAF());
  Serial.print(F(","));
  Serial.print(totalForce());
  Serial.print(F(","));
  Serial.print(gyroYaw);
  Serial.print(F(","));
  Serial.print(gyroPitch);
  Serial.print(F(","));
  Serial.println(gyroRoll);
  //Serial.print(',');
  //Serial.println(temp());
}

void printCombinedOrientationChart() {
  Serial.print(gyroYaw);
  Serial.print(F(","));
  Serial.print(pitch);
  Serial.print(F(","));
  Serial.print(roll);
  Serial.println();
}

void printAccelChart() {
  Serial.print(F("NA"));
  Serial.print(F(","));
  Serial.print(pitchAccelMAF());
  Serial.print(F(","));
  Serial.print(rollAccelMAF());
  Serial.print(F(","));
  Serial.println(totalForce());
}

void printGyroChart() {
  Serial.print(gyroYaw);
  Serial.print(F(","));
  Serial.print(gyroPitch);
  Serial.print(F(","));
  Serial.print(gyroRoll);
  Serial.print(F(","));
  Serial.println(temp());
}

void printGyroMAFChart() {
  Serial.print(hRate());
  Serial.print(F(","));
  Serial.print(pRate());
  Serial.print(F(","));
  Serial.print(rRate());
  Serial.println();
}

void printRateModeChart() {
  Serial.print(pRate());
  Serial.print(F(","));
  Serial.print(rRate());
  Serial.print(F(","));
  Serial.print(pitchRateOutput);
  Serial.print(F(","));
  Serial.print(rollRateOutput);
  Serial.println();
}

void printGyroMAFVariances() {
  Serial.print(hMAFVar());
  Serial.print(F("\t"));
  Serial.print(pMAFVar());
  Serial.print(F("\t"));
  Serial.println(rMAFVar());
}

void printFullRadioInputChart() {
  Serial.print(thrustSetpoint);
  Serial.print(F(", "));
  Serial.print(pitchSetpoint);
  Serial.print(F(", "));
  Serial.print(rollSetpoint);
  Serial.print(F(", "));
  Serial.println(yawSetpoint);
}
void printRawRadioInputChart() {
  Serial.print(ch1MAFAve());
  Serial.print(F(", "));
  Serial.print(ch2MAFAve());
  Serial.print(F(", "));
  Serial.print(ch3MAFAve());
  Serial.print(F(", "));
  Serial.println(ch4MAFAve());
}

void printRadioCalChart() {
  Serial.print(ch1Cal);
  Serial.print(F(", "));
  Serial.print(ch2Cal);
  Serial.print(F(", "));
  Serial.print(ch3Cal);
  Serial.print(F(", "));
  Serial.println(ch4Cal);
}


void printCenterOfThrustChart() {
  Serial.print(thrustFloor);
  Serial.print(F("\t"));
  Serial.print(centerOfThrust[0]);
  Serial.print(F("\t"));
  Serial.print(centerOfThrust[1]);
  Serial.print(F("\t"));
  Serial.print(yawAngle);
  Serial.println();
}
