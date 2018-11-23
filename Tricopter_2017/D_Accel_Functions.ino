//BMA220 Accelerometer DFR Robot

//Accelerometer Variables
float pAccelMAF[ACCEL_MAF_LENGTH];
float rAccelMAF[ACCEL_MAF_LENGTH];
byte accelData[3];
int8_t x_data;
int8_t y_data;
int8_t z_data;

//float prevAccelX, prevAccelY, prevAccelZ;

float accelPitch, accelRoll;

void setupAccel() {
  setLowPassFilter();
  setRange();
}

void setLowPassFilter() {
  Wire.beginTransmission(0x0A); // address of the accelerometer
  Wire.write(0x20); //Low Pass Filter Register Address
  Wire.write(ACCEL_FILTER);   //[0x5,0x4,0x3,0x2,0x1,0x0] is [32Hz,64Hz,125Hz,250Hz,500Hz,1000Hz] sample rate respectively.
  Wire.endTransmission();
}

void setRange() {
  Wire.beginTransmission(0x0A); // address of the accelerometer
  Wire.write(0x22); //Range Register Address
  Wire.write(0x00);   //+-2g Range
  Wire.endTransmission();
}

void readAccel()
{
  Wire.beginTransmission(0x0A); // address of the accelerometer
  // reset the accelerometer
  Wire.write(0x04); // X data
  Wire.endTransmission();
  Wire.requestFrom(0x0A, 1);   // request 6 bytes from slave device #2
  while (Wire.available())   // slave may send less than requested
  {
    accelData[0] = Wire.read(); // receive a byte as character
  }
  x_data = (int8_t)accelData[0] >> 2;

  Wire.beginTransmission(0x0A); // address of the accelerometer
  // reset the accelerometer
  Wire.write(0x06); // Y data
  Wire.endTransmission();
  Wire.requestFrom(0x0A, 1);   // request 6 bytes from slave device #2
  while (Wire.available())   // slave may send less than requested
  {
    accelData[1] = Wire.read(); // receive a byte as characte
  }
  y_data = (int8_t)accelData[1] >> 2;

  Wire.beginTransmission(0x0A); // address of the accelerometer
  // reset the accelerometer
  Wire.write(0x08); // Z data
  Wire.endTransmission();
  Wire.requestFrom(0x0A, 1);   // request 6 bytes from slave device #2
  while (Wire.available())   // slave may send less than requested
  {
    accelData[2] = Wire.read(); // receive a byte as characte
  }
  z_data = (int8_t)accelData[2] >> 2;
}

void dataUnitConversion() {
  int tempAccelX = map((int)x_data, -32, 31, -200, 194 ) ;
  int tempAccelY = map((int)y_data, -32, 31, -200, 194 ) ;
  int tempAccelZ = map((int)z_data, -32, 31, -200, 194 ) ;

  accelX = (float)tempAccelX / 100;
  accelY = (float)tempAccelY / 100;
  accelZ = (float)tempAccelZ / 100;
}

void deducePitchRoll() {
  accelPitch = deg(atan2(accelX, sqrt((accelY * accelY) + (accelZ * accelZ))));
  accelRoll = deg(atan2(-accelY, -accelZ));
}
void updateAccelMAF() {
  int i = 0;
  for (i = 0; i < (ACCEL_MAF_LENGTH - 1); i++) { //Move MAF forwards without last term
    pAccelMAF[i] = pAccelMAF[i + 1];
    rAccelMAF[i] = rAccelMAF[i + 1];
  }
  pAccelMAF[ACCEL_MAF_LENGTH - 1] = accelPitch; //Plug in latest term for MAF
  rAccelMAF[ACCEL_MAF_LENGTH - 1] = accelRoll;
}

//FLOAT RETURNING FUNCTIONS
float pitchAccelMAF() {
  int i = 0;
  float total = 0;
  for (i = 0; i <= (ACCEL_MAF_LENGTH - 1); i++) {
    total = total + pAccelMAF[i];
  }
  return (total / ACCEL_MAF_LENGTH);
}

float rollAccelMAF() {
  int i = 0;
  float total = 0;
  for (i = 0; i <= (ACCEL_MAF_LENGTH - 1); i++) {
    total = total + rAccelMAF[i];
  }
  return (total / ACCEL_MAF_LENGTH);
}

float totalForce() {
  float _total;
  _total = abs(accelX) + abs(accelY) + abs(accelZ);
  _total = sqrt(_total);
  return _total;
}
