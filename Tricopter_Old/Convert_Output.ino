//Back Propeller Variables
double backAdjustedThrust, headingAngle;
double leftRealThrust, rightRealThrust, backRealThrust, headingThrust;
double thrustFloor;

//Vector Variables
double magL, magR,magB;
float leftPropVector[2] = { 
  -(sqrt(2)/2), (sqrt(2)/2)};
float rightPropVector[2] ={ 
  (sqrt(2)/2), (sqrt(2)/2) };
float backPropVector[2] = { 
  0,-1};


//Function Description
//Input: PID Outputs(pitchRateOutput, headingOutput, rollOutput), Desired Total Thrust
//Output: leftThrust, rightThrust, backThrust, headingAngle
void convertOutput() {
  magL=0;
  magR=0;
  magB=0;
  setMagLR();  //Calculate Unit Thrust of Left and Right Propellers
  setMagB(); //Calculate Unit Thrust of Back Propeller AND add Left and Right Unit Thrust if pitching up
  if(totalThrust<25.0) {  //To ensure smooth motor starts, all PID adjustment is voided until totalThrust exceeds 25%
    magL = 0.0;
    magR = 0.0;
    magB = 0.0;
  }
  thrustFloor = (totalThrust*3)-(magL+magR+magB); //Operation to ensure output is equivalent to totalThrust value
  leftRealThrust = magL+(thrustFloor/3);
  rightRealThrust = magR+(thrustFloor/3);
  backRealThrust = magB+(thrustFloor/3);
  headingThrust=map(headingOutput, -50.0, 50.0, -40.0, 40.0);
  backProp(backRealThrust,headingThrust); //Note, function may cause overshoot of motor output over 100%
  /*  if (backAdjustedThrust>100) {
   float difference=backAdjustedThrust-100;
   backAdjustedThrust=100;
   leftRealThrust = leftRealThrust - difference/2; 
   rightRealThrust = rightRealThrust - difference/2;
   }*/
}

//Function
//Input: backThrust, headingThrust
//Output: backAdjustedThrust, headingAngle
void backProp(float _vertThrust, float _headingThrust) {
  float _realThrust;
  double _angle;
  if(_vertThrust<10.0||_vertThrust<_headingThrust) {
    _realThrust=0.0;  
    _headingThrust=0.0;
    _angle=0.0;
  }
  else{
    _angle=deg(atan2(_headingThrust,_vertThrust)); //Calculates Servo Angle. If vertThrust=1 and headingThrust=1,angle = 45. If vertThrust=1 and headingThrust=-1, angle=-45.
    _realThrust = sqrt((_vertThrust*_vertThrust)+(_headingThrust*_headingThrust));
  }
  backAdjustedThrust=_realThrust ;
  headingAngle=constrain(_angle, -30.0, 30.0) ;

}

void setMagLR() {
  //If Center of Thrust = 0, no balancing required
  if(centerOfThrust[0]==0&&centerOfThrust[1]==0) {
    magL=0;
    magR=0;
    magB=0;
  }
  //if Center of Thrust positive, balance right prop horizontal component
  else if (centerOfThrust[0]>0) {
    magR=centerOfThrust[0]/rightPropVector[0];
    magL=0;
  }
  //if Center of Thrust negative, balance left prop horizontal component
  else if (centerOfThrust[0]<0) {
    magL=centerOfThrust[0]/leftPropVector[0];
    magR=0;
  }
}

void setMagB() {
  //balance back prop. If prop output is negative, then instead increase left prop and right prop thrust equally.
  magB=((magL*leftPropVector[1])+(magR*rightPropVector[1]))-centerOfThrust[1]; 
  if (magB<0) {
    magL+=((centerOfThrust[1]/2)/leftPropVector[1]);
    magR+=((centerOfThrust[1]/2)/rightPropVector[1]);
    magB=0;
  }
}





