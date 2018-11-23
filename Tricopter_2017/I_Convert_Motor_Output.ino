//Function Description
//Input: PID Outputs(pitchRateOutput, yawAngle, rollOutput), Desired Total Thrust
//Output: leftThrust, rightThrust, backThrust
void convertMotorOutput() {
  if (motorsArmed) {
    magL = 0;
    magR = 0;
    magB = 0;
    setMagLR();  //Calculate Unit Thrust of Left and Right Propellers
    setMagB(); //Calculate Unit Thrust of Back Propeller AND add Left and Right Unit Thrust if pitching up
    //thrustFloor = constrain (map(thrustSetpoint, -1000, 1000, 0, 1000), 0, 775); //Thrust Floor Set to be thrustSetpoint up to 770 signal units. PID will draw up to 200 units, and BackProp will draw up to 230 units due to yaw requirements. See PID Tab for explanation.
    thrustFloor = constrain (map(thrustSetpoint, -1000, 1000, 0, 1000), 0, 700); //Thrust Floor Set to be thrustSetpoint up to 550 signal units. PID will draw up to 400 units, and BackProp will draw up to 588 units due to yaw requirements. See PID Tab for explanation.
    leftRealThrust = magL + (thrustFloor);
    rightRealThrust = magR + (thrustFloor);
    backRealThrust = magB + (thrustFloor);
    //backProp(backRealThrust, yawAngle);
    //backAdjustedThrust = backRealThrust;
  }
}

void setMagLR() {
  //If Center of Thrust = 0, no balancing required
  if (centerOfThrust[0] == 0) {
    magL = 0;
    magR = 0;
    magB = 0;
  }
  //if Center of Thrust positive, balance right prop horizontal component
  else if (centerOfThrust[0] > 0) {
    magR = (centerOfThrust[0] * rightPropVector[0]) / 1000000L; //Center of Thrust * Vector. To avoid float math, both variables scaled up by 1000. Divide by 1000,000L
    magL = 0;
  }
  //if Center of Thrust negative, balance left prop horizontal component
  else if (centerOfThrust[0] < 0) {
    magL = (centerOfThrust[0] * leftPropVector[0]) / 1000000L; //Center of Thrust * Vector. To avoid float math, both variables scaled up by 1000. Divide by 1000,000L
    magR = 0;
  }
}

void setMagB() {
  //Calculates required vertical vector after factoring Left and Right props (reqVertVect is still scaled up by 1000 to avoid float math)
  long reqVertVect = centerOfThrust[1] - ((magL * leftPropVector[1]) + (magR * rightPropVector[1])); //all variables are still scaled by 1000

  if (reqVertVect > 0) { //If required vector is positive then instead increase left prop and right prop thrust equally.
    magL += ((reqVertVect / 2) / leftPropVector[1]); //1000 Scaling factor cancels itself out. with division
    magR += ((reqVertVect / 2) / rightPropVector[1]); //1000 Scaling factor cancels itself out. with division
    magB = 0;
  } else {
    //If required vertical vector is negative, then magB will be positive since backpropvector is -1 To avoid float math, centerOfThrust scaled up by 1000 and cast as long.
    //Proper operation is "magB = (reqVertVect*backPropVector[1])/(1000000L)" but since backpropvector is simply -1000, it can be simplified as such.
    magB = (-1 * reqVertVect) / 1000;
  }
}

//Function
//Input: backRealThrust, yawAngle
//Output: backAdjustedThrust
//void backProp(long _vertThrust, float _yawAngle) {
  //long _multiplier = long(cos(rad(abs(_yawAngle))) * 1000);
  //backAdjustedThrust = ((_vertThrust * 1000) / _multiplier); //VertThrust scaled up by  1000 in so that when divided by multiplier, scale factor will cancel out.
//}
