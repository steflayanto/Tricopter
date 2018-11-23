// These bit flags are set in bUpdateFlagsShared to indicate which channels have new signals
#define CH_ONE_FLAG 1
#define CH_TWO_FLAG 2
#define CH_THREE_FLAG 4
#define CH_FOUR_FLAG 8
#define AUX5_FLAG 16
#define AUX6_FLAG 32

//Radio Receiver Variables
volatile uint8_t bUpdateFlagsShared;
volatile uint16_t unCh1InShared;
volatile uint16_t unCh2InShared;
volatile uint16_t unCh3InShared;
volatile uint16_t unCh4InShared;
volatile uint16_t unAux5InShared;
volatile uint16_t unAux6InShared;
uint32_t ulCh1Start;
uint32_t ulCh2Start;
uint32_t ulCh3Start;
uint32_t ulCh4Start;
uint32_t ulAux5Start;
uint32_t ulAux6Start;


//Radio Channel MAF
int radio1MAF[RADIO_MAF_LENGTH];
int radio2MAF[RADIO_MAF_LENGTH];
int radio3MAF[RADIO_MAF_LENGTH];
int radio4MAF[RADIO_MAF_LENGTH];


void readChannels() {
  updateRadioMAF();
  static uint16_t unCh1In;
  static uint16_t unCh2In;
  static uint16_t unCh3In;
  static uint16_t unCh4In;
  static uint16_t unAux5In;
  static uint16_t unAux6In;
  // local copy of update flags
  static uint8_t bUpdateFlags;

  // check shared update flags to see if any channels have a new signal
  if (bUpdateFlagsShared)
  {
    noInterrupts(); // turn interrupts off quickly while we take local copies of the shared variables
    // take a local copy of which channels were updated in case we need to use this in the rest of loop
    bUpdateFlags = bUpdateFlagsShared;

    // in the current code, the shared values are always populated
    // so we could copy them without testing the flags
    // however in the future this could change, so lets
    // only copy when the flags tell us we can.

    if (bUpdateFlags & CH_ONE_FLAG)
    {
      unCh1In = unCh1InShared;
    }

    if (bUpdateFlags & CH_TWO_FLAG)
    {
      unCh2In = unCh2InShared;
    }

    if (bUpdateFlags & CH_THREE_FLAG)
    {
      unCh3In = unCh3InShared;
    }

    if (bUpdateFlags & CH_FOUR_FLAG)
    {
      unCh4In = unCh4InShared;
    }

    if (bUpdateFlags & AUX5_FLAG)
    {
      unAux5In = unAux5InShared;
    }

    if (bUpdateFlags & AUX6_FLAG)
    {
      unAux6In = unAux6InShared;
    }

    // clear shared copy of updated flags as we have already taken the updates
    // we still have a local copy if we need to use it in bUpdateFlags
    bUpdateFlagsShared = 0;

    interrupts(); // we have local copies of the inputs, so now we can turn interrupts back on
    // as soon as interrupts are back on, we can no longer use the shared copies, the interrupt
    // service routines own these and could update them at any time. During the update, the
    // shared copies may contain junk. Luckily we have our local copies to work with :-)
  }

  if (bUpdateFlags & CH_ONE_FLAG)
  {
    if (ch1 != unCh1In)
    {
      ch1 = unCh1In;
    }
  }

  if (bUpdateFlags & CH_TWO_FLAG)
  {
    if (ch2 != unCh2In)
    {
      ch2 = unCh2In;
    }
  }

  if (bUpdateFlags & CH_THREE_FLAG)
  {
    if (ch3 != unCh3In)
    {
      ch3 = unCh3In;
    }
  }

  if (bUpdateFlags & CH_FOUR_FLAG)
  {
    if (ch4 != unCh4In)
    {
      ch4 = unCh4In;
    }
  }


  if (bUpdateFlags & AUX5_FLAG)
  {
    if (aux5 != unAux5In)
    {
      aux5 = unAux5In;
    }
  }

  if (bUpdateFlags & AUX6_FLAG)
  {
    if (aux6 != unAux6In)
    {
      aux6 = unAux6In;
    }
  }

  bUpdateFlags = 0;

}



// simple interrupt service routine
void calcCh1()
{
  // if the pin is high, its a rising edge of the signal pulse, so lets record its value
  if (digitalRead(CH_ONE_PIN) == HIGH)
  {
    ulCh1Start = micros();
  }
  else
  {
    // else it must be a falling edge, so lets get the time and subtract the time of the rising edge
    // this gives use the time between the rising and falling edges i.e. the pulse duration.
    unCh1InShared = (uint16_t)(micros() - ulCh1Start);
    // use set the throttle flag to indicate that a new throttle signal has been received
    bUpdateFlagsShared |= CH_ONE_FLAG;
  }
}

void calcCh2()
{
  if (digitalRead(CH_TWO_PIN) == HIGH)
  {
    ulCh2Start = micros();
  }
  else
  {
    unCh2InShared = (uint16_t)(micros() - ulCh2Start);
    bUpdateFlagsShared |= CH_TWO_FLAG;
  }
}

void calcCh3()
{
  if (digitalRead(CH_THREE_PIN) == HIGH)
  {
    ulCh3Start = micros();
  }
  else
  {
    unCh3InShared = (uint16_t)(micros() - ulCh3Start);
    bUpdateFlagsShared |= CH_THREE_FLAG;
  }
}

void calcCh4()
{
  if (digitalRead(CH_FOUR_PIN) == HIGH)
  {
    ulCh4Start = micros();
  }
  else
  {
    unCh4InShared = (uint16_t)(micros() - ulCh4Start);
    bUpdateFlagsShared |= CH_FOUR_FLAG;
  }
}

void calcAux5()
{
  if (digitalRead(AUX5_IN_PIN) == HIGH)
  {
    ulAux5Start = micros();
  }
  else
  {
    unAux5InShared = (uint16_t)(micros() - ulAux5Start);
    bUpdateFlagsShared |= AUX5_FLAG;
  }
}

void calcAux6()
{
  if (digitalRead(AUX6_IN_PIN) == HIGH)
  {
    ulAux6Start = micros();
  }
  else
  {
    unAux6InShared = (uint16_t)(micros() - ulAux6Start);
    bUpdateFlagsShared |= AUX6_FLAG;
  }
}


//Functions for the moving average filter (MAF)
void updateRadioMAF() {
  int __i = 0;
  for (__i = 0; __i < (RADIO_MAF_LENGTH - 1); __i++) { //Move MAF forwards without last term
    radio1MAF[__i] = radio1MAF[__i + 1];
    radio2MAF[__i] = radio2MAF[__i + 1];
    radio3MAF[__i] = radio3MAF[__i + 1];
    radio4MAF[__i] = radio4MAF[__i + 1];
  }
  //Plug in latest term for MAF. Map Ranges are all empirically devised
  radio1MAF[RADIO_MAF_LENGTH - 1] = map(constrain(ch1, 1050, 1870), 1050, 1870, 1000, -1000); //Yaw (inverted)
  radio2MAF[RADIO_MAF_LENGTH - 1] = map(constrain(ch2, 1055, 1880), 1055, 1880, -1000, 1000); //Throttle
  radio3MAF[RADIO_MAF_LENGTH - 1] = map(constrain(ch3, 1060, 1875), 1060, 1875, 1000, -1000); //Pitch (inverted)
  radio4MAF[RADIO_MAF_LENGTH - 1] = map(constrain(ch4, 1045, 1865), 1045, 1865, -1000, 1000); //Roll
}

int ch1MAFAve() {
  int i__ = 0;
  float total = 0.0;
  for (i__ = 0; i__ <= (RADIO_MAF_LENGTH - 1); i__++) {
    total = total + radio1MAF[i__];
  }
  return int(total / RADIO_MAF_LENGTH);
}

int ch2MAFAve() {
  int i__ = 0;
  float total = 0.0;
  for (i__ = 0; i__ <= (RADIO_MAF_LENGTH - 1); i__++) {
    total = total + radio2MAF[i__];
  }
  return int(total / RADIO_MAF_LENGTH);
}

int ch3MAFAve() {
  int i__ = 0;
  float total = 0.0;
  for (i__ = 0; i__ <= (RADIO_MAF_LENGTH - 1); i__++) {
    total = total + radio3MAF[i__];
  }
  return int(total / RADIO_MAF_LENGTH);
}

int ch4MAFAve() {
  int i__ = 0;
  float total = 0.0;
  for (i__ = 0; i__ <= (RADIO_MAF_LENGTH - 1); i__++) {
    total = total + radio4MAF[i__];
  }
  return int(total / RADIO_MAF_LENGTH);
}
