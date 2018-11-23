//Rate PID
PID pitchRatePID(&pitchRateInput, &pitchRateOutput, &pitchRateSetpoint, pitchRateKp, pitchRateKi, pitchRateKd, DIRECT);
PID rollRatePID(&rollRateInput, &rollRateOutput, &rollRateSetpoint, rollRateKp, rollRateKi, rollRateKd, REVERSE);
PID yawPID(&yawInput, &yawOutput, &yawSetpoint, yawKp, yawKi, yawKd, DIRECT);

//Attitude PID
PID pitchPID(&pitchInput, &pitchOutput, &pitchSetpoint, pitchKp, pitchKi, pitchKd, DIRECT);
PID rollPID(&rollInput, &rollOutput, &rollSetpoint, rollKp, rollKi, rollKd, DIRECT);

void PIDsetup() {
  /* Output limits are set on the 0-1000 signal unit scale such that Each motor has a 400 unit range set aside for maneuvering.
     Formula (Experimental) relating thrust (grams) to signal units: Thrust(grams) = 0.5319 Units - 84.353. (Intercept means no measured thrust below 160 units)
     200 units corresponds to 212.76 grams of thrust set aside for maneuvering.
     Because Tricopter props are at 120 degree angles, output limits are set at +-346.410 so that the 400 unit upperbound is not breached (range 692.820 which is 400*sqrt3).
     See diagram "CenterOfThrustGrid(Visualization).png"
     //Max Motor Pull is at diagonals of square thrust grid, and with side length 141.421, diagonal is 200.
     Yaw is tricky. Since I need to be able to set aside 400 units of thrust for vertical maneuvering, I need to give additional power to back propellor so it can spool up as it tilts to maintain vertical power.
     Mathematically, if I set 35 degrees as max tilt, I'll need to give the back prop up to 488.310 units (400/cos35) for maneuvering. This means my thrust floor can never exceed 512 units (But I will set it at 550).
  */

  pitchRatePID.SetMode(AUTOMATIC);
  pitchRatePID.SetSampleTime(PIDSampleTime);
  pitchRatePID.SetOutputLimits(-200, 200);

  pitchPID.SetMode(AUTOMATIC);
  pitchPID.SetSampleTime(PIDSampleTime*4);
  pitchPID.SetOutputLimits(-200, 200);

  rollRatePID.SetMode(AUTOMATIC);
  rollRatePID.SetSampleTime(PIDSampleTime);
  rollRatePID.SetOutputLimits(-346.410, 346.410);
  rollRatePID.SetControllerDirection(REVERSE);

  rollPID.SetMode(AUTOMATIC);
  rollPID.SetSampleTime(PIDSampleTime*4);
  rollPID.SetOutputLimits(-200, 200);
  rollPID.SetControllerDirection(DIRECT);


  yawPID.SetMode(AUTOMATIC);
  yawPID.SetSampleTime(PIDSampleTime);
  yawPID.SetOutputLimits(-75, 75);
}


//Function Description
//Input: Orientation, Desired orientation.
//Output: pitchRateOutput, rollRateOutput, yawAngle
void calculatePID() {
  setTunings();

  if (thrustFloor > 100) {
    PIDActive = true;
  } else {
    PIDActive = false;
  }

  //Input for higher order pitch & roll PID, which is approximated orientation by accelerometer & gyroscope
  pitchInput = pitch;
  rollInput = roll;

  //Input for rate PIDs: gyro measured rates of rotation
  pitchRateInput = pRate();
  rollRateInput = rRate();
  yawInput = hRate();

  //Compute first high order pitch/roll PIDs, to get pitchOutput & rollOutput (to feed into pitchRate/rollRate setpoints)
  pitchPID.Compute();
  rollPID.Compute();

  //Setpoints for pitch, roll, and yaw: set directly to stick inputs, on main tab under "checkForInput" function
  if (PIDActive && standardMode) {
    pitchPID.SetMode(AUTOMATIC);
    rollPID.SetMode(AUTOMATIC);

    pitchRateSetpoint = pitchOutput;
    rollRateSetpoint = rollOutput;
  } else {
    pitchPID.SetMode(MANUAL);
    rollPID.SetMode(MANUAL);

    pitchRateSetpoint = map(pitchSetpoint, -45, 45, -200, 200);
    //pitchRateSetpoint = 0.0;

    rollRateSetpoint = map(rollSetpoint, -45, 45, -200, 200);
    //rollRateSetpoint = 0.0;
  }

  pitchRatePID.Compute();
  rollRatePID.Compute();
  yawPID.Compute();

  if (PIDActive) {
    pitchRatePID.SetMode(AUTOMATIC); //AUTOMATIC
    rollRatePID.SetMode(AUTOMATIC);//AUTOMATIC
    yawPID.SetMode(AUTOMATIC); //AUTOMATIC
  } else {
    pitchRatePID.SetMode(MANUAL);
    rollRatePID.SetMode(MANUAL);
    yawPID.SetMode(MANUAL);
    rollRateOutput=0;
    pitchRateOutput=0;
    yawOutput = 0;
  }
  centerOfThrust[0] = constrain(long((rollRateOutput) * 1000L) + COT_X_TRIM, -200000, 200000);
  centerOfThrust[1] = constrain(long((pitchRateOutput) * 1000L) + COT_Y_TRIM, -173205, 173205);
  yawAngle = 10 * round( atan2 (yawOutput + YAW_TRIM, backRealThrust) * 180 / 3.14159265 ); //New Yaw Angle Function. Converts a -75 to 75 unit yaw output to an angle based on the back thrust.
}



void setTunings() {
  pitchRatePID.SetTunings(pitchRateKp, pitchRateKi, pitchRateKd);
  pitchPID.SetTunings(pitchKp, pitchKi, pitchKd);
  rollRatePID.SetTunings(rollRateKp, rollRateKi, rollRateKd);
  rollPID.SetTunings(rollKp, rollKi, rollKd);
  yawPID.SetTunings(yawKp, yawKi, yawKd);
}
