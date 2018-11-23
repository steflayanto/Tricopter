void setupRadio(){
  PCintPort::attachInterrupt(CH_ONE_PIN, calcCh1,CHANGE); 
  PCintPort::attachInterrupt(CH_TWO_PIN, calcCh2,CHANGE); 
  PCintPort::attachInterrupt(CH_THREE_PIN, calcCh3,CHANGE); 
  PCintPort::attachInterrupt(CH_FOUR_PIN, calcCh4,CHANGE); 
  PCintPort::attachInterrupt(AUX_IN_PIN, calcAux,CHANGE);  
}

void calibrateRadioInput() {
  readChannels();
  ch1Cal=ch1-1450;
  ch3Cal=ch3-1450;
  ch4Cal=ch4-1450;
}
void readChannels() {
  static uint16_t unCh1In;
  static uint16_t unCh2In;
  static uint16_t unCh3In;
  static uint16_t unCh4In;
  static uint16_t unAuxIn;
  // local copy of update flags
  static uint8_t bUpdateFlags;

  // check shared update flags to see if any channels have a new signal
  if(bUpdateFlagsShared)
  {
    noInterrupts(); // turn interrupts off quickly while we take local copies of the shared variables

      // take a local copy of which channels were updated in case we need to use this in the rest of loop
    bUpdateFlags = bUpdateFlagsShared;

    // in the current code, the shared values are always populated
    // so we could copy them without testing the flags
    // however in the future this could change, so lets
    // only copy when the flags tell us we can.

    if(bUpdateFlags & CH_ONE_FLAG)
    {
      unCh1In = unCh1InShared;
    }

    if(bUpdateFlags & CH_TWO_FLAG)
    {
      unCh2In = unCh2InShared;
    }

    if(bUpdateFlags & CH_THREE_FLAG)
    {
      unCh3In = unCh3InShared;
    }

    if(bUpdateFlags & CH_FOUR_FLAG)
    {
      unCh4In = unCh4InShared;
    }

    if(bUpdateFlags & AUX_FLAG)
    {
      unAuxIn = unAuxInShared;
    }

    // clear shared copy of updated flags as we have already taken the updates
    // we still have a local copy if we need to use it in bUpdateFlags
    bUpdateFlagsShared = 0;

    interrupts(); // we have local copies of the inputs, so now we can turn interrupts back on
    // as soon as interrupts are back on, we can no longer use the shared copies, the interrupt
    // service routines own these and could update them at any time. During the update, the 
    // shared copies may contain junk. Luckily we have our local copies to work with :-)
  }

  if(bUpdateFlags & CH_ONE_FLAG)
  {
    if(ch1 != unCh1In)
    {
      ch1=unCh1In;
    }
  }

  if(bUpdateFlags & CH_TWO_FLAG)
  {
    if(ch2 != unCh2In)
    {
      ch2=unCh2In;
    }
  }

  if(bUpdateFlags & CH_THREE_FLAG)
  {
    if(ch3 != unCh3In)
    {
      ch3=unCh3In;
    }
  }

  if(bUpdateFlags & CH_FOUR_FLAG)
  {
    if(ch4 != unCh4In)
    {
      ch4=unCh4In;
    }
  }


  if(bUpdateFlags & AUX_FLAG)
  {
    if(aux != unAuxIn)
    {
      aux = unAuxIn;
    }
  }

  bUpdateFlags = 0;

}

// simple interrupt service routine
void calcCh1()
{
  // if the pin is high, its a rising edge of the signal pulse, so lets record its value
  if(digitalRead(CH_ONE_PIN) == HIGH)
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
  if(digitalRead(CH_TWO_PIN) == HIGH)
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
  if(digitalRead(CH_THREE_PIN) == HIGH)
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
  if(digitalRead(CH_FOUR_PIN) == HIGH)
  { 
    ulCh4Start = micros();
  }
  else
  {
    unCh4InShared = (uint16_t)(micros() - ulCh4Start);
    bUpdateFlagsShared |= CH_FOUR_FLAG;
  }
}

void calcAux()
{
  if(digitalRead(AUX_IN_PIN) == HIGH)
  { 
    ulAuxStart = micros();
  }
  else
  {
    unAuxInShared = (uint16_t)(micros() - ulAuxStart);
    bUpdateFlagsShared |= AUX_FLAG;
  }
}




