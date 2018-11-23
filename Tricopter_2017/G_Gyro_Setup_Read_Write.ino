//Registers
char WHO_AM_I = 0x00;
char SMPLRT_DIV = 0x15;
char DLPF_FS = 0x16;
char TEMP_OUT_H = 0x1B;
char TEMP_OUT_L = 0x1C;
char GYRO_XOUT_H = 0x1D;
char GYRO_XOUT_L = 0x1E;
char GYRO_YOUT_H = 0x1F;
char GYRO_YOUT_L = 0x20;
char GYRO_ZOUT_H = 0x21;
char GYRO_ZOUT_L = 0x22;

//This is a list of settings that can be loaded into the registers.
//DLPF, Full Scale Register Bits
//FS_SEL must be set to 3 for proper operation
//Set DLPF_CFG to 3 for 1kHz Fint and 42 Hz Low Pass Filter
char DLPF_CFG_0 = 1 << 0;
char DLPF_CFG_1 = 1 << 1;
char DLPF_CFG_2 = 1 << 2;
char DLPF_FS_SEL_0 = 1 << 3;
char DLPF_FS_SEL_1 = 1 << 4;

//Gyro I2C Address
char itgAddress = 0x69;

void setupGyro() {
  //Read the WHO_AM_I register and print the result
  //Serial.print("ID: ");
  //Serial.println(id, HEX);
  char id = 0;
  id = gyroRead(itgAddress, 0x00);

  //Configure the gyroscope
  //Set the gyroscope scale for the outputs to +/-2000 degrees per second
  gyroWrite(itgAddress, DLPF_FS, (DLPF_FS_SEL_0 | DLPF_FS_SEL_1 | DLPF_CFG_0));
  //Set the sample rate to 100 hz
  gyroWrite(itgAddress, SMPLRT_DIV, 9);
}
// char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
// char registerAddress: The address of the register on the sensor that should be written to.
// char data: The value to be written to the specified register.
void gyroWrite(char address, char registerAddress, char data)
{
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// char address: The I2C address of the sensor. For the ITG-3200 breakout the address is 0x69.
// char registerAddress: The address of the register on the sensor that should be read
//Return:
// unsigned char: The value currently residing in the specified register
unsigned char gyroRead(char address, char registerAddress)
{
  unsigned char data = 0;
  //Contact Slave
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.endTransmission();

  //Ask for data
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 1);
  if (Wire.available()) {
    data = Wire.read();
  }
  Wire.endTransmission();

  return data;
}

//This function is used to read the X-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
int readX(void)
{
  int data = 0;
  data = gyroRead(itgAddress, GYRO_XOUT_H) << 8;
  data |= gyroRead(itgAddress, GYRO_XOUT_L);

  return data;
}

//This function is used to read the Y-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
int readY(void)
{
  int data = 0;
  data = gyroRead(itgAddress, GYRO_YOUT_H) << 8;
  data |= gyroRead(itgAddress, GYRO_YOUT_L);

  return data;
}

//This function is used to read the Z-Axis rate of the gyroscope. The function returns the ADC value from the Gyroscope
//NOTE: This value is NOT in degrees per second.
int readZ(void)
{
  int data = 0;
  data = gyroRead(itgAddress, GYRO_ZOUT_H) << 8;
  data |= gyroRead(itgAddress, GYRO_ZOUT_L);

  return data;
}

int readTemp(void)
{
  int data = 0;
  data = gyroRead(itgAddress, TEMP_OUT_H) << 8;
  data |= gyroRead(itgAddress, TEMP_OUT_L);

  return data;
}

void readRawValues() {
  xRawRate = MOUNTING * (readX() / 14.375);
  yRawRate = (-MOUNTING) * (readY() / 14.375);
  zRawRate = (-MOUNTING) * (readZ() / 14.375);
  rawTemp = readTemp() / 280.0;
}
