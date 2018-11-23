//Matrix
double mt00, mt01, mt02, mt10, mt11, mt12, mt20, mt21, mt22;
double m00, m01, m02, m10, m11, m12, m20, m21, m22;

/* Coordinate System: right hand
  Positive angle: right hand
  Order of euler angles: yaw first, then pitch, then roll
  matrix row column ordering:
  [mt00 mt01 mt02]
  [mt10 mt11 mt12]
  [mt20 mt21 mt22]*/

void updateOrientation() {
  interval();
  float _rIncrement = (rRate() * _interval) / 1000;
  float _pIncrement = (pRate() * _interval) / 1000;
  float _hIncrement = (hRate() * _interval) / 1000;

  //1 Dimensional Orientation Calculation
  /*  gyroRoll=gyroRoll + _rIncrement;
    gyroPitch= gyroPitch + _pIncrement;
    gyroYaw=gyroYaw + _hIncrement;
    temp=rawTemp+tempOffset;
      //  delay(250); */

  //Matrix Calculation
  eulerToMatrix(gyroYaw, pitch, roll);
  eulerToTransformativeMatrix(_hIncrement, _pIncrement, _rIncrement);
  multiplyMatrixByTransformativeMatrix();
  //multiplyTransformativeMatrixByMatrix();
  matrixToEuler();
  //   wrapOrientation();
}

void wrapOrientation() {
  if (gyroYaw >= 360) {
    gyroYaw -= 360;
  }
  if (gyroYaw < 0) {
    gyroYaw += 360;
  }
  if (roll > 180) {
    roll -= 360;
  }
  if (roll <= -180) {
    roll += 360;
  }
}

void eulerToMatrix(double _yaw, double _pitch, double _roll) {
  double ch = cos(rad(_yaw));
  double sh = sin(rad(_yaw));
  double cp = cos(rad(_pitch));
  double sp = sin(rad(_pitch));
  double cr = cos(rad(_roll));
  double sr = sin(rad(_roll));
  m00 = ch * cp;
  m01 = (sh * sr) - (ch * sp * cr);
  m02 = (ch * sp * sr) + (sh * cr);
  m10 = sp;
  m11 = cp * cr;
  m12 = -cp * sr;
  m20 = -sh * cp;
  m21 = (sh * sp * cr) + (ch * sr);
  m22 = (-sh * sp * sr) + (ch * cr);

}

void eulerToTransformativeMatrix(double _yaw, double _pitch, double _roll) {
  double ch = cos(rad(_yaw));
  double sh = sin(rad(_yaw));
  double cp = cos(rad(_pitch));
  double sp = sin(rad(_pitch));
  double cr = cos(rad(_roll));
  double sr = sin(rad(_roll));
  mt00 = ch * cp;
  mt01 = (sh * sr) - (ch * sp * cr);
  mt02 = (ch * sp * sr) + (sh * cr);
  mt10 = sp;
  mt11 = cp * cr;
  mt12 = -cp * sr;
  mt20 = -sh * cp;
  mt21 = (sh * sp * cr) + (ch * sr);
  mt22 = (-sh * sp * sr) + (ch * cr);
}

void matrixToEuler() {
  if (m10 > 0.998) { // singularity at north pole
    gyroYaw = deg(atan2(m02, m22));
    gyroPitch = deg(PI / 2);
    gyroRoll = 0;
  }
  else if (m10 < -0.998) { // singularity at south pole
    gyroYaw = deg(atan2(m02, m22));
    gyroPitch = deg(-PI / 2);
    gyroRoll = 0;
  }
  else {
    gyroYaw = deg(atan2(-m20, m00)); //Yaw
    gyroRoll = deg(atan2(-m12, m11)); //Pitch
    gyroPitch = deg(asin(m10)); //Roll
  }
}

void multiplyMatrixByTransformativeMatrix() {
  double _m00 = (m00 * mt00) + (m01 * mt10) + (m02 * mt20);
  double _m01 = (m00 * mt01) + (m01 * mt11) + (m02 * mt21);
  double _m02 = (m00 * mt02) + (m01 * mt12) + (m02 * mt22);
  double _m10 = (m10 * mt00) + (m11 * mt10) + (m12 * mt20);
  double _m11 = (m10 * mt01) + (m11 * mt11) + (m12 * mt21);
  double _m12 = (m10 * mt02) + (m11 * mt12) + (m12 * mt22);
  double _m20 = (m20 * mt00) + (m21 * mt10) + (m22 * mt20);
  double _m21 = (m20 * mt01) + (m21 * mt11) + (m22 * mt21);
  double _m22 = (m20 * mt02) + (m21 * mt12) + (m22 * mt22);
  m00 = _m00;
  m01 = _m01;
  m02 = _m02;
  m10 = _m10;
  m11 = _m11;
  m12 = _m12;
  m20 = _m20;
  m21 = _m21;
  m22 = _m22;
  /*  -----------------------

    1.00	0.00	0.00

    0.00	1.00	0.00

    0.00	0.00	1.00



    -----------------------

    0.98	-0.20	-0.07

    0.21	0.95	0.25

    0.02	-0.26	0.97
  */
}

void multiplyTransformativeMatrixByMatrix() {
  double _m00 = (mt00 * m00) + (mt01 * m10) + (mt02 * m20);
  double _m01 = (mt00 * m01) + (mt01 * m11) + (mt02 * m21);
  double _m02 = (mt00 * m02) + (mt01 * m12) + (mt02 * m22);
  double _m10 = (mt10 * m00) + (mt11 * m10) + (mt12 * m20);
  double _m11 = (mt10 * m01) + (mt11 * m11) + (mt12 * m21);
  double _m12 = (mt10 * m02) + (mt11 * m12) + (mt12 * m22);
  double _m20 = (mt20 * m00) + (mt21 * m10) + (mt22 * m20);
  double _m21 = (mt20 * m01) + (mt21 * m11) + (mt22 * m21);
  double _m22 = (mt20 * m02) + (mt21 * m12) + (mt22 * m22);
  m00 = _m00;
  m01 = _m01;
  m02 = _m02;
  m10 = _m10;
  m11 = _m11;
  m12 = _m12;
  m20 = _m20;
  m21 = _m21;
  m22 = _m22;
}

void resetMatrix() {
  double ch = cos(0);
  double sh = sin(0);
  double cp = cos(0);
  double sp = sin(0);
  double cr = cos(0);
  double sr = sin(0);
  m00 = ch * cp;
  m01 = (sh * sr) - (ch * sp * cr);
  m02 = (ch * sp * sr) + (sh * cr);
  m10 = sp;
  m11 = cp * cr;
  m12 = -cp * sr;
  m20 = -sh * cp;
  m21 = (sh * sp * cr) + (ch * sr);
  m22 = (-sh * sp * sr) + (ch * cr);
}

void printMatrix() {
  Serial.println(F("-----------------------"));
  Serial.print(m00);
  Serial.print(F("\t"));
  Serial.print(m01);
  Serial.print(F("\t"));
  Serial.print(m02);
  Serial.println(F("\t"));
  Serial.print(m10);
  Serial.print(F("\t"));
  Serial.print(m11);
  Serial.print(F("\t"));
  Serial.print(m12);
  Serial.println("\t");
  Serial.print(m20);
  Serial.print(F("\t"));
  Serial.print(m21);
  Serial.print(F("\t"));
  Serial.print(m22);
  Serial.println("\t");
}

void printTransformativeMatrix() {
  Serial.println(F("-----------------------"));
  Serial.print(mt00);
  Serial.print(F("\t"));
  Serial.print(mt01);
  Serial.print(F("\t"));
  Serial.print(mt02);
  Serial.println(F("\t"));
  Serial.print(mt10);
  Serial.print(F("\t"));
  Serial.print(mt11);
  Serial.print(F("\t"));
  Serial.print(mt12);
  Serial.println(F("\t"));
  Serial.print(mt20);
  Serial.print(F("\t"));
  Serial.print(mt21);
  Serial.print(F("\t"));
  Serial.print(mt22);
  Serial.println(F("\t"));
}
