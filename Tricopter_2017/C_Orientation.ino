/*
  CONVENTIONS:
  Right handed Coordinate System
  Yaw = Rotation about Y-axis
  Pitch = Rotation about X-axis
  Roll = Rotation about Z-axis
  NOTE: Gyro is read in terms of x=pitch,y=roll,z=yaw but is adjusted into proper conventions as stated above.

  Accelerometer Output: pitchAccelMAF(), rollAccelMAF(), totalForce()
  Gyroscope Output: gyroYaw, gyroPitch, gyroRoll, temp()

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
  readRawGyroData();   //Data Reading Function Updates raw Moving Average Filter 200times a second
  updateOrientation();    //Update Orientation Information
  combineOrientationData();

  //Data Printing
  //printAccelGyroChart();
  //printCombinedOrientationChart();

  //Accelerometer
  //printAccelValues();
  //printAccelOrientation();
  //printAccelChart();
  //intervalTimer();

  //Gyroscope
  //printGyroOrientation();
  //printGyroChart();
  //printGyroVariances();
  //printMatrix();
  //Serial.println();
  //printTransformativeMatrix();
  //Serial.println();
  //Serial.println();

}

void combineOrientationData() {
  pitch = (0.98 * gyroPitch) + (0.02 * pitchAccelMAF());
  roll = (0.98 * gyroRoll) + (0.02 * rollAccelMAF());
}

