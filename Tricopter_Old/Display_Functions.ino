void printFullRadioInput() {
  if(millis()-timer1>=50) {
    Serial.print("Thrust: ");
    Serial.println(totalThrust);
    Serial.print("Pitch: ");
    Serial.println(pitchSetpoint);
    Serial.print("Roll: ");
    Serial.println(rollSetpoint);
    Serial.print("Heading: ");
    Serial.println(headingSetpoint);
    Serial.println();
    timer1 = millis();
  }

}

void printCenterOfThrust() {
  if(millis()-timer1>=50) {
    Serial.print("Thrust: ");
    Serial.println(totalThrust);
    Serial.print("xThrust: ");
    Serial.println(centerOfThrust[0]);
    Serial.print("yThrust: ");
    Serial.println(centerOfThrust[1]);
    Serial.println();
    timer1 = millis();
  }

}

void printPIDInput() {
  Serial.print("pitchRate:"); 
  Serial.print(pitchRateInput);
  Serial.print("\trollRate:"); 
  Serial.print(rollRateInput);
  Serial.print("\tRoll:");
  Serial.print(rollInput);
  Serial.print("\tHeading:");
  Serial.println(headingInput);
}

void printPIDOutput() {
  Serial.print("pitchRate:"); 
  Serial.print(pitchRateOutput);
  Serial.print("\trollRate:"); 
  Serial.print(rollRateOutput);
  Serial.print("\tRoll Setpoint:");
  Serial.print(rollRateSetpoint);
  Serial.print("\tHeading:");
  Serial.println(headingOutput);
}

void printFlightControlOutput() {
  Serial.print("Left:"); 
  Serial.print(magL); 
  Serial.print("\tRight:"); 
  Serial.print(magR); 
  Serial.print("\tBack:"); 
  Serial.print(magB); 
  Serial.print("\tHeading:"); 
  Serial.println(headingThrust); 
}

void printRealOutput() {
  Serial.print("Left:"); 
  Serial.print(leftRealThrust); 
  Serial.print("\tRight:"); 
  Serial.print(rightRealThrust); 
  Serial.print("\tBack:"); 
  Serial.print(backAdjustedThrust); 
  Serial.print("\tHeading:"); 
  Serial.println(headingAngle); 
}

void printAccelGyroSerialChart() {
  Serial.print("NA");
  Serial.print(',');
  Serial.print(pitchMAF());
  Serial.print(',');
  Serial.print(rollMAF());
  Serial.print(',');
  Serial.print(totalForce());  
  Serial.print(',');
  Serial.print(gyroHeading);
  Serial.print(',');
  Serial.print(gyroPitch);
  Serial.print(',');
  Serial.print(gyroRoll);
  Serial.print(',');
  Serial.println(temp());  
}

void printCombinedOrientationSerialChart() {
  Serial.print(gyroHeading);
  Serial.print(',');
  Serial.print(pitch);
  Serial.print(',');
  Serial.print(roll);
  Serial.print(',');
  Serial.print(centerOfThrust[1]);
  Serial.println();

}

//Accelerometer
void printAccelOrientation() {
  Serial.println("Heading\tPitch\tRoll");
  Serial.print("NA");
  Serial.print('\t');
  Serial.print(pitchMAF());
  Serial.print('\t');
  Serial.println(rollMAF());
}


void printAccelValues() {
  Serial.print("X=");   
  Serial.print(accelX);
  Serial.print("  "); 
  Serial.print("Y=");   
  Serial.print(accelY);
  Serial.print("  "); 
  Serial.print("Z=");  
  Serial.println(accelZ);    
}

void printAccelSerialChart() {
  Serial.print("NA");
  Serial.print(',');
  Serial.print(pitchMAF());
  Serial.print(',');
  Serial.print(rollMAF());
  Serial.print(',');
  Serial.println(totalForce());
}

//Gyroscope
void printGyroOrientation() {
  Serial.println("Heading\tPitch\tRoll\tTemperature");
  Serial.print(gyroHeading);
  Serial.print('\t');
  Serial.print(gyroPitch);
  Serial.print('\t');
  Serial.print(gyroRoll);
  Serial.print('\t');
  Serial.println(temp());
}

void printRawMAF() {
  Serial.println("H_rate\tP_rate\tR_rate");
  Serial.print(hRawMAFAve());
  Serial.print('\t');
  Serial.print(pRawMAFAve());
  Serial.print('\t');
  Serial.print(rRawMAFAve());
  Serial.print('\t');
  Serial.println(rawTemp);
}

void printRates() {
  Serial.println("H rate\tP rate\tR rate");
  Serial.print(hRate());
  Serial.print('\t');
  Serial.print(pRate());
  Serial.print('\t');
  Serial.print(rRate());
  Serial.print('\t');
  Serial.println(temp());
}

void printRateModeSerialChart() {
  Serial.print(pRate());
  Serial.print(',');
  Serial.print(rRate());
  Serial.print(',');
  Serial.print(pitchRateOutput);
  Serial.print(',');
  Serial.print(0);
  Serial.println();
}


//printMatrix() and printTransformativeMatrix() found on Gyro Orientation Tab

void printVariances() {
  Serial.print(hMAFVar());
  Serial.print("\t"); 
  Serial.print(pMAFVar());
  Serial.print("\t"); 
  Serial.println(rMAFVar());
}

void printGyroSerialChart() {
  Serial.print(gyroHeading);
  Serial.print(',');
  Serial.print(gyroPitch);
  Serial.print(',');
  Serial.print(gyroRoll);
  Serial.print(',');
  Serial.println(temp());  
}


