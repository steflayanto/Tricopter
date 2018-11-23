unsigned long writeTimer = 0;

void writeOutput() {
  constrainValues();
  safetyLimiter(limit);
  if (motorsConnected) {
    attachESCs();
    writeESCs();
    writeServo(yawAngle);
  } else {
    writeESCs();
    detachESCs();
  }
}

void constrainValues() {
  if (motorsArmed == true) {
    leftRealThrust = constrain(leftRealThrust, 80, 1000);
    rightRealThrust = constrain(rightRealThrust, 80, 1000);
    backRealThrust = constrain(backRealThrust, 80, 1000);
    yawAngle = constrain(yawAngle, -350, 350);
  }
  if (motorsArmed == false) {
    leftRealThrust = 0;
    rightRealThrust = 0;
    backRealThrust = 0;
    yawAngle = 0;
  }
}

void safetyLimiter(int limit) {
  leftRealThrust = constrain(leftRealThrust, 2, limit);
  rightRealThrust = constrain(rightRealThrust, 2, limit);
  backRealThrust = constrain(backRealThrust, 4, limit);
}

void writeESCs() {
  escL.writeMicroseconds(convertedSignal(leftRealThrust, LEFT_PROP_GRADIENT, LEFT_PROP_INTERCEPT));
  escR.writeMicroseconds(convertedSignal(rightRealThrust,RIGHT_PROP_GRADIENT,RIGHT_PROP_INTERCEPT));

  //For the Back Prop, "backAdjustedThrust" function first multiplies the backRealThrust in order to compensate for thrust lost due to tilt, before converting signal.
 //escB.writeMicroseconds(convertedSignal(backAdjustedThrust(backRealThrust, map(yawAngle, -350, 350, 930, 1770)), BACK_PROP_GRADIENT, BACK_PROP_INTERCEPT));

  escB.writeMicroseconds(convertedSignal(backRealThrust,BACK_PROP_GRADIENT,BACK_PROP_INTERCEPT)); //Without backAdjustedThrust Function

}

void writeServo(int _angle) {
  if (aux5 < 1500 && uncalibrated != true) {
    yaw.attach(yawPin);
  } else { //If High, then detach servo to make it shut up
    yaw.detach();
  }
  yaw.writeMicroseconds(map(_angle, -350, 350, 930, 1770));
}

//Function generates Prop-Specific  Signal to Achieve Desired Thrust. Gradient + Intercepts Based on Experimental Data  (See excel sheet).
int convertedSignal(int input, float gradient, float intercept) {
  float answer = (gradient * map(input, 0, 1000, 0, 480)) + intercept;
  answer = constrain(answer, 700, 2000);
  return int(answer);
}

//Function generates BackProp Output to compensate for vertical power lost due to tilt.
int backAdjustedThrust(int vertThrust, int servoSignal) {
  float _mFactor = (((1455380.03589241 * (servoSignal - 200) * (servoSignal - 100)) / 1000000000000) - ((3583186.01973847 * (servoSignal - 100)) / 1000000000) + 3.24324902475430);
  float _increase = constrain((_mFactor - 1.0), 0, 2);
  return int((1 + _increase) * vertThrust);
}
