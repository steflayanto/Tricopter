/*
CONVENTIONS:
 Right handed Coordinate System
 Heading = Rotation about Y-axis
 Pitch = Rotation about X-axis
 Roll = Rotation about Z-axis
 NOTE: Gyro is read in terms of x=pitch,y=roll,z=heading but is adjusted into proper conventions as stated above.
 
 Accelerometer Output: pitchMAF(), rollMAF(), totalForce()
 Gyroscope Output: gyroHeading, gyroPitch, gyroRoll, temp()
 
 */

void setupOrientation() {
  setupAccel();
  setupGyro();
  resetMatrix();
}

void getOrientation() {
  //Accelerometer
  readAccel(); 
  dataUnitConversion();
  deducePitchRoll();
  updateAccelMAF();

  //Gyroscope
  readRawGyroData();   //Data Reading Function, always runs irregardless of calibration state. Updates raw Moving Average Filter 100times a seconds
  if(uncalibrated) { //If uncalibrated, then calibrate it ("no shit, sherlock.") Calibration function prints "Calibrated"
    calibrate();
  }
  else{             
    //If already calibrated                            
    updateOrientation();    //Update Orientation Information
    combineOrientationData(); 

    //Data Printing
    //printAccelGyroSerialChart();
//    printCombinedOrientationSerialChart();

    //Accelerometer
    //printAccelValues();
    //printAccelOrientation();
    //printAccelSerialChart();
    //intervalTimer();

    //Gyroscope
    //printGyroOrientation();
    //printGyroSerialChart();
    //printVariances();
    //printMatrix();
    //Serial.println();
    //printTransformativeMatrix();
    //Serial.println();
    //Serial.println();
  }
}

void combineOrientationData() {
  pitch=(0.98*gyroPitch)+(0.02*pitchMAF());
  roll=(0.98*gyroRoll)+(0.02*rollMAF());
}

