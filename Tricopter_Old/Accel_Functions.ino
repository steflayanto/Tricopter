//BMA220 Accelerometer DFR Robot
float pAccelMAF[accelMAFLength];
float rAccelMAF[accelMAFLength];
byte accelData[3];
int8_t x_data;
int8_t y_data;
int8_t z_data;
float accelX, accelY, accelZ;
float prevAccelX, prevAccelY, prevAccelZ;
float accelPitch, accelRoll;


void setupAccel() {
  setLowPassFilter(); 
  setRange(); 
}

void setLowPassFilter() {
  Wire.beginTransmission(0x0A); // address of the accelerometer 
  Wire.write(0x20); //Low Pass Filter Register Address
  Wire.write(0x01);   //500kHz sample rate
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
  Wire.requestFrom(0x0A,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    accelData[0] = Wire.read(); // receive a byte as character
  }  
  x_data=(int8_t)accelData[0]>>2;

  Wire.beginTransmission(0x0A); // address of the accelerometer 
  // reset the accelerometer 
  Wire.write(0x06); // Y data
  Wire.endTransmission(); 
  Wire.requestFrom(0x0A,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    accelData[1] = Wire.read(); // receive a byte as characte
  }  
  y_data=(int8_t)accelData[1]>>2;

  Wire.beginTransmission(0x0A); // address of the accelerometer 
  // reset the accelerometer 
  Wire.write(0x08); // Z data
  Wire.endTransmission(); 
  Wire.requestFrom(0x0A,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    accelData[2] = Wire.read(); // receive a byte as characte
  }  
  z_data=(int8_t)accelData[2]>>2; 
} 

void dataUnitConversion(){
  int tempAccelX=map((int)x_data, -32, 31, -200, 194 ) ;
  int tempAccelY=map((int)y_data, -32, 31, -200, 194 ) ;
  int tempAccelZ=map((int)z_data, -32, 31, -200, 194 ) ;

  accelX=(float)tempAccelX/100;
  accelY=(float)tempAccelY/100;
  accelZ=(float)tempAccelZ/100;
}

void deducePitchRoll() {
  accelPitch=deg(atan2(accelX,sqrt((accelY*accelY)+(accelZ*accelZ))));
  accelRoll=deg(atan2(-accelY,-accelZ));
}
void updateAccelMAF() {
  int _i_=0;
  for (_i_=0; _i_<(accelMAFLength-1);_i_++) { //Move MAF forwards without last term
    pAccelMAF[_i_]=pAccelMAF[_i_+1];
    rAccelMAF[_i_]=rAccelMAF[_i_+1];
  } 
  pAccelMAF[accelMAFLength-1]= accelPitch; //Plug in latest term for MAF
  rAccelMAF[accelMAFLength-1]= accelRoll;
}

//FLOAT RETURNING FUNCTIONS
float pitchMAF() {
  int i__=0;
  float total=0;
  for(i__=0;i__<=(accelMAFLength-1);i__++) {
    total = total + pAccelMAF[i__];
  }
  return (total/accelMAFLength);
}

float rollMAF() {
  int i__=0;
  float total=0;
  for(i__=0;i__<=(accelMAFLength-1);i__++) {
    total = total + rAccelMAF[i__];
  }
  return (total/accelMAFLength);
}

float totalForce() {
 float _total;
_total = abs(accelX)+abs(accelY)+abs(accelZ);
_total = sqrt(_total);
return _total;
}
