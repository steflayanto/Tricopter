
void PIDsetup() {
  pitchRatePID.SetMode(AUTOMATIC);
  pitchRatePID.SetSampleTime(sampleTime);
  pitchRatePID.SetOutputLimits(-50,50);

  pitchPID.SetMode(AUTOMATIC);
  pitchPID.SetSampleTime(sampleTime);
  pitchPID.SetOutputLimits(-50,50);


  rollRatePID.SetMode(AUTOMATIC);
  rollRatePID.SetSampleTime(sampleTime);
  rollRatePID.SetOutputLimits(-50,50);
  rollRatePID.SetControllerDirection(REVERSE);

  rollPID.SetMode(AUTOMATIC);
  rollPID.SetSampleTime(sampleTime);
  rollPID.SetOutputLimits(-50,50);


  headingPID.SetMode(AUTOMATIC);
  headingPID.SetSampleTime(sampleTime);
  headingPID.SetOutputLimits(-50,50);
}


//Function Description
//Input: Orientation, Desired orientation.
//Output: pitchRateOutput, rollRateOutput, headingOutput
void calculatePID() {

  setTunings();
  
//Input for higher order pitch & roll PID, which is approximated orientation by accelerometer & gyroscope
  pitchInput=pitch;
  rollInput=roll;
//Input for rate PIDs: gyro measured rates of rotation
  pitchRateInput=pRate();
  rollRateInput=rRate();
  headingInput=hRate();

//Setpoints for pitch, roll, and heading: set directly to stick inputs, on main tab under "checkForInput" function

//Compute first high order pitch/roll PIDs, to get pitchOutput & rollOutput (to feed into pitchRate/rollRate setpoints)
  pitchPID.Compute();
  rollPID.Compute();


//Setpoint for pitchRate & rollRate temporarily set to stick inputs
  //pitchRateSetpoint=pitchOutput;
  //rollRateSetpoint=rollOutput;
  pitchRateSetpoint=pitchSetpoint;
  rollRateSetpoint=rollSetpoint;
  
//Compute remaining PIDs
//  headingPID.Compute();

  pitchRatePID.Compute();
  rollRatePID.Compute();

  if(autoControl){
    centerOfThrust[0]=rollRateOutput+cotXTrim;
//    centerOfThrust[1]=pitchRateOutput+cotYTrim;
    centerOfThrust[1]=cotYTrim;
  }
}



void setTunings() {
  pitchRatePID.SetTunings(pitchRateKp, pitchRateKi, pitchRateKd);
  pitchPID.SetTunings(pitchKp, pitchKi, pitchKd);
  rollRatePID.SetTunings(rollRateKp, rollRateKi, rollRateKd);
  rollPID.SetTunings(consKp, consKi, consKd);
  headingPID.SetTunings(headingKp, headingKi, headingKd); 
}


