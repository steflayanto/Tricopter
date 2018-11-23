unsigned long writeTimer = 0;

void writeOutput() {
  constrainValues();
  safetyLimiter(_limit);
  writeESCs();
  writeServo(headingAngle);
}

void constrainValues() {
  leftRealThrust=constrain(leftRealThrust,0,100);
  rightRealThrust=constrain(rightRealThrust,0,100);
  backAdjustedThrust=constrain(backAdjustedThrust,0,100);
  headingAngle=constrain(headingAngle,-90.0,90.0);
}

void safetyLimiter(int _limit) {
  leftRealThrust=constrain(leftRealThrust,2,_limit);
  rightRealThrust=constrain(rightRealThrust,2,_limit);
  backAdjustedThrust=constrain(backAdjustedThrust,2,_limit);
}

void writeESCs() {
  escL.writeMicroseconds(map(leftRealThrust,0,100,700,2000)); 
  escR.writeMicroseconds(map(rightRealThrust,0,100,700,2000));
  escB.writeMicroseconds(map(backAdjustedThrust,0,100,700,2000));
}

void writeServo(float _angle) {
  _angle=_angle*100.0;
  _angle=map(_angle,-3000,3000, 1600+headingTrim, 2400+headingTrim);
    heading.writeMicroseconds(constrain(_angle,1600,2400));

  /*  _angle=_angle*10.0;
   int _write = map(int(_angle), -300, 300, 1500+headingTrim, 2000+headingTrim);
   Serial.println(_angle);
   Serial.println(_write);
   Serial.println(heading.read());
   heading.writeMicroseconds(_write);*/
}


