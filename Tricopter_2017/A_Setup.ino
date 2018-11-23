int calCounter = 0;
float  hCalSum = 0, pCalSum = 0, rCalSum = 0;
float ch1CalSum = 0, ch2CalSum = 0, ch3CalSum = 0, ch4CalSum = 0;

void setupPins() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(escLPin, OUTPUT);
  pinMode(escRPin, OUTPUT);
  pinMode(escBPin, OUTPUT);
  pinMode(yawPin, OUTPUT);
  setupRadioPins();
  attachESCs();
  //yaw Servo attaches within calibrate() function
}

void attachESCs() {
  escL.attach(escLPin);
  escR.attach(escRPin);
  escB.attach(escBPin);
}

void detachESCs() {
  escL.detach();
  escR.detach();
  escB.detach();
}

void setupRadioPins() {
  PCintPort::attachInterrupt(CH_ONE_PIN, calcCh1, CHANGE);
  PCintPort::attachInterrupt(CH_TWO_PIN, calcCh2, CHANGE);
  PCintPort::attachInterrupt(CH_THREE_PIN, calcCh3, CHANGE);
  PCintPort::attachInterrupt(CH_FOUR_PIN, calcCh4, CHANGE);
  PCintPort::attachInterrupt(AUX5_IN_PIN, calcAux5, CHANGE);
  PCintPort::attachInterrupt(AUX6_IN_PIN, calcAux6, CHANGE);
}


//Main Calibration Function
void calibrate() {
  readRawGyroData(); //Reads Gyro and Updates raw gyro MAF
  readChannels(); //Reads Radio Channels and Updates Radio MAF
  if ((millis() - calTimer) > CAL_DURATION) {
    //CALIBRATE GYRO
    hCal = hCalSum / (calCounter - 30);
    pCal = pCalSum / (calCounter - 30);
    rCal = rCalSum / (calCounter - 30);
    //tempOffset = 25.0 - rawTemp; Temperature not used

    //CALIBRATE RADIO
    ch1Cal = ch1CalSum / (calCounter - 30);
    ch2Cal = 1000 + (ch2CalSum / (calCounter - 30));
    ch3Cal = ch3CalSum / (calCounter - 30);
    ch4Cal = ch4CalSum / (calCounter - 30);

    uncalibrated = false;
    calCounter = 0;
    Serial.println(F("Calibrated"));
    printCalData();
    yaw.attach(yawPin);         //YAW SERVO ATTACHED HERE
    //Ensure MAF arrays are completely filled up before taking averages
  } else if (calCounter >= 30) {
    //GYRO
    hCalSum += hRawMAFAve();
    pCalSum += pRawMAFAve();
    rCalSum += rRawMAFAve();

    //RADIO
    ch1CalSum += ch1MAFAve();
    ch2CalSum += ch2MAFAve();
    ch3CalSum += ch3MAFAve();
    ch4CalSum += ch4MAFAve();

    calCounter += 1;
    printCalibrationTimer();
    blinkLED(500);
  } else {
    //If MAF Arrays aren't completely full then dont begin taking averages yet
    calCounter += 1;
  }
}
