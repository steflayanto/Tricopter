unsigned long prev = 0;
int _interval;

//Timer
unsigned long rawTimerStart = 0;

//Raw Value MAF
float hRawMAF[GYRO_MAF_LENGTH];
float pRawMAF[GYRO_MAF_LENGTH];
float rRawMAF[GYRO_MAF_LENGTH];

//Variables for Variance
double hPrevVar = 10, pPrevVar = 10, rPrevVar = 10;
//Read Values and update Raw Moving Average Filter 200 times a second


void readRawGyroData() {
  if (millis() - rawTimerStart >= 5) {
    readRawValues();
    updateRawGyroMAF();
    rawTimerStart = millis();
  }
}

//Functions for the moving average filter (MAF)
void updateRawGyroMAF() {
  int i = 0;
  for (i = 0; i < (GYRO_MAF_LENGTH - 1); i++) { //Move MAF forwards without last term
    hRawMAF[i] = hRawMAF[i + 1];
    pRawMAF[i] = pRawMAF[i + 1];
    rRawMAF[i] = rRawMAF[i + 1];
  }
  hRawMAF[GYRO_MAF_LENGTH - 1] = zRawRate; //Plug in latest term for MAF
  pRawMAF[GYRO_MAF_LENGTH - 1] = xRawRate;
  rRawMAF[GYRO_MAF_LENGTH - 1] = yRawRate;
}

//FLOATS
//Yaw
float hRawMAFAve() { //Returns the current average of the MAF
  int i = 0;
  float total = 0;
  for (i = 0; i <= (GYRO_MAF_LENGTH - 1); i++) {
    total = total + hRawMAF[i];
  }
  return (total / GYRO_MAF_LENGTH);
}

float hRate() {
  return (hRawMAFAve() - hCal);
}

float hMAFVar() {
  int i1 = 0;
  float total = 0;
  for (i1 = 0; i1 <= (GYRO_MAF_LENGTH - 1); i1++) {
    total = total + sq(hRawMAF[i1] - hRawMAFAve());
  }
  hPrevVar = (total / GYRO_MAF_LENGTH);
  return (total / GYRO_MAF_LENGTH);
  //}
}

//Pitch
float pRawMAFAve() {
  int i = 0;
  float total = 0;
  for (i = 0; i <= (GYRO_MAF_LENGTH - 1); i++) {
    total = total + pRawMAF[i];
  }
  return (total / GYRO_MAF_LENGTH);
}

float pRate() {
  return (pRawMAFAve() - pCal);
}

float pMAFVar() {
  /*  if (moving) {
    return pPrevVar;
    }
    else{
  */
  int i1 = 0;
  float total = 0;
  for (i1 = 0; i1 <= (GYRO_MAF_LENGTH - 1); i1++) {
    total = total + sq(pRawMAF[i1] - pRawMAFAve());
  }
  pPrevVar = (total / GYRO_MAF_LENGTH);
  return (total / GYRO_MAF_LENGTH);

  //}
}

//Roll
float rRawMAFAve() {
  int i = 0;
  float total = 0;
  for (i = 0; i <= (GYRO_MAF_LENGTH - 1); i++) {
    total = total + rRawMAF[i];
  }
  return (total / GYRO_MAF_LENGTH);
}

float rRate() {
  return (rRawMAFAve() - rCal);
}

float rMAFVar() {
  /*  if (moving) {
    return rPrevVar;
    }
    else{ */
  int i1 = 0;
  float total = 0;
  for (i1 = 0; i1 <= (GYRO_MAF_LENGTH - 1); i1++) {
    total = total + sq(rRawMAF[i1] - rRawMAFAve());
  }
  rPrevVar = (total / GYRO_MAF_LENGTH);
  return (total / GYRO_MAF_LENGTH);

  //}
}

float temp() {
  return (rawTemp + tempOffset);
}

void interval() {  //Sets _interval as the time in milliseconds since this function was last called
  unsigned long tempInterval;
  tempInterval = (millis() - prev);
  prev = millis();
  _interval = int(tempInterval);
}

