unsigned long prev=0;
int _interval;
float hCal=0, pCal=0, rCal=0, tempOffset=0;        //Calibration Data

//Timer
unsigned long rawTimerStart=0;

//Raw Value MAF
float hRawMAF[gyroMAFLength];
float pRawMAF[gyroMAFLength];
float rRawMAF[gyroMAFLength];

//Read Values and update Raw Moving Average Filter 200 times a second
void readRawGyroData() {
  if (millis()-rawTimerStart>=5) {  
    readRawValues();
    updateRawMAF();
    rawTimerStart = millis();
  }
}

//Reads RawMAF Values and sets the calibration variables as the average.
void calibrate() {
  //Only Calibrate if its been at least 2 seconds into the program
  if((millis()-calTimer)>2000) {
    Serial.println("Calibrated");
    hCal=hRawMAFAve();
    pCal=pRawMAFAve();
    rCal=rRawMAFAve();
    tempOffset = 25.0 - rawTemp;
    uncalibrated = false;
      calibrateRadioInput();
  }
  printRawMAF();             //Print Values for calibrating
  Serial.println(rawTemp);
}


//This function checks to see if the measured angle rates exceed the variance. 
//If they exceed the variances, then it will set boolean "moving" to true, 
//and will output non-zero values of hRate,pRate,zRate.
/*
void checkIfMoving() {
 float hSensitivity = 1;
 float pSensitivity = 1;
 float rSensitivity = 1;
 moving=false;
 if (hMAFVar()>hSensitivity) {
 moving=true;
 }
 if (pMAFVar()>pSensitivity) {
 moving=true;
 }
 if (rMAFVar()>rSensitivity) {
 moving=true;
 }  
 }
 */
//Variables for Variance
double hPrevVar=10,pPrevVar=10,rPrevVar=10;

//Functions for the moving average filter (MAF)
void updateRawMAF() {
//  readRawValues();
  int __i=0;
  for (__i=0; __i<(gyroMAFLength-1);__i++) { //Move MAF forwards without last term
    hRawMAF[__i]=hRawMAF[__i+1];
    pRawMAF[__i]=pRawMAF[__i+1];
    rRawMAF[__i]=rRawMAF[__i+1];
  } 
  hRawMAF[gyroMAFLength-1]= zRawRate; //Plug in latest term for MAF
  pRawMAF[gyroMAFLength-1]= xRawRate;
  rRawMAF[gyroMAFLength-1]= yRawRate;
}


//FLOATS
//Heading
float hRawMAFAve() {
  int i__=0;
  float total=0;
  for(i__=0;i__<=(gyroMAFLength-1);i__++) {
    total = total + hRawMAF[i__];
  }
  return (total/gyroMAFLength);
}

float hRate() {
  return (zRawRate-hCal);
}

float hMAFVar() {
  /*  if (moving) {
   return rPrevVar;
   }
   else{ */
  int i1=0;
  float total = 0;
  for (i1=0;i1<=(gyroMAFLength-1);i1++) {
    total = total+abs(hRawMAF[i1]-hRawMAFAve());
  }
  hPrevVar = (total/gyroMAFLength);
  return (total/gyroMAFLength);
  //}
}

//Pitch
float pRawMAFAve() {
  int i__=0;
  float total=0;
  for(i__=0;i__<=(gyroMAFLength-1);i__++) {
    total = total + pRawMAF[i__];
  }
  return (total/gyroMAFLength);
}

float pRate() {
  return (xRawRate-pCal);
}

float pMAFVar() {
  /*  if (moving) {
   return pPrevVar;
   }
   else{ 
   */
  int i1=0;
  float total = 0;
  for (i1=0;i1<=(gyroMAFLength-1);i1++) {
    total = total+abs(pRawMAF[i1]-pRawMAFAve());
  }
  pPrevVar = (total/gyroMAFLength);
  return (total/gyroMAFLength);

  //}
}

//Roll
float rRawMAFAve() {
  int i__=0;
  float total=0;
  for(i__=0;i__<=(gyroMAFLength-1);i__++) {
    total = total + rRawMAF[i__];
  }
  return (total/gyroMAFLength);
}

float rRate() {
  return (yRawRate-rCal);
}

float rMAFVar() {
  /*  if (moving) {
   return rPrevVar;
   }
   else{ */
  int i1=0;
  float total = 0;
  for (i1=0;i1<=(gyroMAFLength-1);i1++) {
    total = total+abs(rRawMAF[i1]-rRawMAFAve());
  }
  rPrevVar = (total/gyroMAFLength);
  return (total/gyroMAFLength);

  //}
}

float temp() {
  return (rawTemp+tempOffset); 
}

void interval() {  //Sets _interval as the time in milliseconds since this function was last called
  unsigned long tempInterval;
  tempInterval= (millis() - prev);
  prev=millis();
  _interval =int(tempInterval);
}

