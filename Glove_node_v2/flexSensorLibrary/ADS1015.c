/*
 * ADS1015.c
 *
 *  Created on: 24-Aug-2022
 *      Author: sujaj
 */

#include <flexSensorLibrary/ADS1015.h>

uint16_t _mode = ADS1015_CONFIG_MODE_CONT;
uint16_t _gain = ADS1015_CONFIG_PGA_2;
uint16_t _sampleRate = ADS1015_CONFIG_RATE_1600HZ;

uint16_t calibrationValues[2][2] = {{0, 0}, {0, 0}};
float _multiplierToVolts = 1.0F;

/*
 * @fn      readRegister
 * @brief   read a register location on ADS
 * @param   location - register address
 * @return  data bytes
 */
uint16_t readRegister(uint8_t location)
{
    i2cTransaction.slaveAddress = 0x48; //device I²C address
    txBuffer[0] = location;

    /* write address to Address Pointer register*/
    i2cTransaction.readBuf    = rxBuffer;
    i2cTransaction.readCount  = 0;
    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.writeCount = 1;

    if(I2C_transfer(i2c, &i2cTransaction))
        {
//            Display_printf(displayHandle, 0,0,"I2C write success!");
        }
    else{
//            Display_printf(displayHandle, 0,0,"I2C write fail!");
            i2cErrorHandler(&i2cTransaction);
        }

    /* read data sent by ADS */
    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.writeCount = 0;
    i2cTransaction.readBuf    = rxBuffer;
    i2cTransaction.readCount  = 3;

    if(I2C_transfer(i2c, &i2cTransaction))
    {
//        Display_printf(displayHandle, 0,0,"I2C read success!");
//        Display_printf(displayHandle, 0,0,"Data %d %d", rxBuffer[0], rxBuffer[1]);
    }
    else{
//        Display_printf(displayHandle, 0,0,"I2C read fail!");
        i2cErrorHandler(&i2cTransaction);
    }
    return (rxBuffer[0] <<8 | rxBuffer[1]);
}

/*
 * @fn      writeRegister
 * @brief   write to register location on ADS
 * @param   location - register address
 * @param   val - data to be written
 * @return  none
 */
void writeRegister(uint8_t location, uint16_t val)
{
    i2cTransaction.slaveAddress = 0x48; //device I²C address

    txBuffer[0] = location;
    txBuffer[1] = (uint8_t)(val >> 8);
    txBuffer[2] = (uint8_t)(val & 0xFF);

    /* write address and data */
    i2cTransaction.readBuf    = rxBuffer;
    i2cTransaction.readCount  = 0;
    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.writeCount = 3;

    if(I2C_transfer(i2c, &i2cTransaction))
        {
//            Display_printf(displayHandle, 0,0,"I2C write success!");
//            Display_printf(displayHandle, 0, 0, "%d %d %d", txBuffer[0], txBuffer[1], txBuffer[2]);
        }
    else{
//            Display_printf(displayHandle, 0,0,"I2C write fail!");
            i2cErrorHandler(&i2cTransaction);
        }
}

/*
 * @fn      getSingleEnded
 * @brief   configure for single-ended measurement
 * @param   channel - input channel
 * @return  decimal value of sensor channel single-ended input
 */
uint16_t getSingleEnded (uint8_t channel)
{
    if (channel > 3) {
        return 0;
    }
    /* set configurations */
    uint16_t config = ADS1015_CONFIG_OS_SINGLE |
                        _mode |
                        ADS1015_CONFIG_RATE_3300HZ;

    config |= _gain;


    switch (channel)
    {
    case (0):
        config |= ADS1015_CONFIG_MUX_SINGLE_0;
        break;
    case (1):
        config |= ADS1015_CONFIG_MUX_SINGLE_1;
        break;
    case (2):
        config |= ADS1015_CONFIG_MUX_SINGLE_2;
        break;
    case (3):
        config |= ADS1015_CONFIG_MUX_SINGLE_3;
        break;
    }

    /* write configurations to register */
    writeRegister(ADS1015_POINTER_CONFIG, config);
    sleep(ADS1015_DELAY);

    /* read sensor data */
    return readRegister(ADS1015_POINTER_CONVERT) >> 4;
}

/*
 * @fn      getDifferential
 * @brief   configure for differential measurement
 * @param   CONFIG_MUX_DIFF - differential pin setups
 * @return  *signed* decimal value of sensor differential input
 */
uint16_t getDifferential(uint16_t CONFIG_MUX_DIFF)
{
    // check for valid argument input
    if (
    (CONFIG_MUX_DIFF == ADS1015_CONFIG_MUX_DIFF_P0_N1) ||
    (CONFIG_MUX_DIFF == ADS1015_CONFIG_MUX_DIFF_P0_N3) ||
    (CONFIG_MUX_DIFF == ADS1015_CONFIG_MUX_DIFF_P1_N3) ||
    (CONFIG_MUX_DIFF == ADS1015_CONFIG_MUX_DIFF_P2_N3)
    )
    {
        // valid argument; do nothing and then carry on below
    }
    else
    {
        return 0; // received invalid argument
    }

    uint16_t config = ADS1015_CONFIG_OS_SINGLE   |
                        _mode |
                        _sampleRate;

    config |= _gain;

    config |= CONFIG_MUX_DIFF; // default is ADS1015_CONFIG_MUX_DIFF_P0_N1

    /* write configurations to register */
    writeRegister(ADS1015_POINTER_CONFIG, config);
    sleep(ADS1015_DELAY);

    /* read sensor data */
    uint16_t result = readRegister(ADS1015_POINTER_CONVERT) >> 4;

    /* making sure we keep the sign bit intact */
    if (result > 0x07FF)
    {
    // negative number - extend the sign to 16th bit
      result |= 0xF000;
    }
    return (int16_t)result; // cast as a *signed* 16 bit int
}

/*
 * @fn      getAnalogData
 * @brief   calls getSingleEnded() - function for backward compatibility
 * @param   channel - input channel
 * @return  decimal value of sensor channel single-ended input
 */
uint16_t getAnalogData(uint8_t channel)
{
    return getSingleEnded(channel);
}

/*
 * @fn      getScaledAnalogData
 * @brief   Scale analog data
 * @param   channel - input channel
 * @return  Returns a value between 0 and 1 based on how bent the finger is.
 *
 * === This function will not work with an uncalibrated sensor ===
 */
float getScaledAnalogData (uint8_t channel)
{
    float data = mapf(getAnalogData(channel), calibrationValues[channel][0], calibrationValues[channel][1], 0, 1);
    if (data > 1)
    {
        return 1;
    }
    else if (data < 0)
    {
        return 0;
    }
    else
    {
        return data;
    }
}

/*
 * @fn      calibrate
 * @brief   calibrate the sensor and map the flexible range to values given by the user
 * @param   none
 * @return  none
 */
void calibrate ()
{
    for (finger = 0; finger < 2; finger++)
    {
        uint16_t value = getAnalogData(finger);
        if ((value > calibrationValues[finger][1] || calibrationValues[finger][1] == 0) && value < 1085)
        {
            calibrationValues[finger][1] = value;
        }
        else if (value < calibrationValues[finger][0] || calibrationValues[finger][0] == 0)
        {
            calibrationValues[finger][0] = value;
        }
    }
}

/*
 * @fn      getCalibration
 * @brief   Get the high or low calibration value for a certain channel
 * @param   channel - channel
 * @param   hiLo - 1, high
 *               - 0, low
 * @return  high calibration if hiLo is 1
 *          low calibration if hiLo is 0
 */
uint16_t getCalibration(uint8_t channel, bool hiLo)
{
    return calibrationValues[channel][hiLo];
}

/*
 * @fn      setCalibration
 * @brief   Sets the high or low calibration value of a channel without using the automatic calibration function
 * @param   channel - channel
 * @param   hiLo - 1, high
 *               - 0, low
 * @param   value - calibration value
 * @return  none
 */
void setCalibration(uint8_t channel, bool hiLo, uint16_t value)
{
    calibrationValues[channel][hiLo] = value;
}

/*
 * @fn      resetCalibration
 * @brief   Resets the calibration to 0
 * @param   none
 * @return  none
 */
void resetCalibration()
{
    for (channel = 0; channel < 2; channel++)
    {
        for (hiLo = 0; hiLo < 2; hiLo++)
        {
            calibrationValues[channel][hiLo] = 0;
        }
    }
}

/*
 * @fn      mapf
 * @brief   map value between 0 and 1 based on how bent the finger is
 * @param   none
 * @return  none
 */
float mapf(float val, float in_min, float in_max, float out_min, float out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*
 * @fn      setMode
 * @brief   sets conversion mode
 * @param   mode - 0 for continuous
 *                 1 for single shot
 * @return none
 */
void setMode(uint16_t mode)
{
  _mode = mode;
}

/*
 * @fn      getMode
 * @brief   reads conversion mode
 * @param   none
 * @return  return 0 for continuous and 1 for single shot
 */
uint16_t getMode ()
{
  return _mode;
}

/*
 * @fn      setGain
 * @brief   sets gain to PGA
 * @param   gain - gain
 * @return  none
 */
void setGain (uint16_t gain)
{
    _gain = gain;
    updateMultiplierToVolts(_gain); // each new gain setting changes how we convert to volts
}

/*
 * @fn      getGain
 * @brief   reads set gain to PGA
 * @param   none
 * @return  gain
 *
 * === Will return a different hex value for each gain ===
 *  0x0E00: +/- 0.256V
 *  0X0000: +/- 6.144V
 *  0X0200: +/- 4.096V
 *  0X0400: +/- 2.048V
 *  0X0600: +/- 1.024V
 *  0X0800: +/- 0.512V
 *  0X0A00: +/- 0.256V
 */
uint16_t getGain ()
{
    return _gain;
}

/*
 * @fn      updateMultiplierToVolts
 * @brief   sets multiplier to PGA
 * @param   _gain - multiplier
 * @return  none
 */
void updateMultiplierToVolts(uint16_t _gain)
{
    switch (_gain)
    {
    case (ADS1015_CONFIG_PGA_TWOTHIRDS):
        _multiplierToVolts = 3.0F;
        break;
    case (ADS1015_CONFIG_PGA_1):
        _multiplierToVolts = 2.0F;
        break;
    case (ADS1015_CONFIG_PGA_2):
        _multiplierToVolts = 1.0F;
        break;
    case (ADS1015_CONFIG_PGA_4):
        _multiplierToVolts = 0.5F;
        break;
    case (ADS1015_CONFIG_PGA_8):
        _multiplierToVolts = 0.25F;
        break;
    case (ADS1015_CONFIG_PGA_16):
        _multiplierToVolts = 0.125F;
        break;
    default:
        _multiplierToVolts = 1.0F;
    }
}

/*
 * @fn      getMultiplier
 * @brief   reads set multiplier
 * @param   none
 * @return  multiplier value
 */
float getMultiplier()
{
  return _multiplierToVolts;
}

/*
 * @fn      setSampleRate
 * @brief   sets sample rate
 * @param   sampleRate - sample rate
 * @return  none
 */
void setSampleRate (uint16_t sampleRate)
{
    _sampleRate = sampleRate;
}

/*
 * @fn      getSampleRate
 * @brief   reads set sample rate
 * @param   none
 * @return  sample rate
 *
 * === Will return a different hex value for each sample rate ===
 *  0x0000: 128 Hz
 *  0X0020: 250 Hz
 *  0X0040: 490 Hz
 *  0X0060: 920 Hz
 *  0X0080: 1600 Hz
 *  0X00A0: 2400 Hz
 *  0X00C0: 3300 Hz
 */
uint16_t getSampleRate ()
{
    return _sampleRate;
}

/*
 * @fn      available
 * @brief   Checks to see if DRDY flag is set in the status register
 * @param   none
 * @return  HIGH or LOW
 */
bool available()
{
  uint16_t value = readRegister(ADS1015_POINTER_CONFIG);
  return (value & (1 << 0)); //Bit 0 is DRDY
}

/*
 * @fn      setComparatorSingleEnded
 * @brief   Sets up the comparator to operate in basic mode, causing the
            ALERT/RDY pin to assert (go from high to low) when the ADC
            value exceeds the specified threshold.
 * @param   channel - channel
 * @param   threshold - threashold value
 * @return  none
 */
void setComparatorSingleEnded(uint8_t channel, int16_t threshold)
{
    if (channel > 3) {
        return;
    }

    uint16_t config =
                    ADS1015_CONFIG_MODE_CONT |
                    _sampleRate |
                    ADS1015_CONFIG_CQUE_1CONV   |   // Comparator enabled and asserts on 1 match
                    ADS1015_CONFIG_CLAT_LATCH   |   // Latching mode
                    ADS1015_CONFIG_CPOL_ACTVLOW |   // Alert/Rdy active low   (default val)
                    ADS1015_CONFIG_CMODE_TRAD;      // Traditional comparator (default val)

    config |= _gain;

    switch (channel)
    {
    case (0):
        config |= ADS1015_CONFIG_MUX_SINGLE_0;
        break;
    case (1):
        config |= ADS1015_CONFIG_MUX_SINGLE_1;
        break;
    case (2):
        config |= ADS1015_CONFIG_MUX_SINGLE_2;
        break;
    case (3):
        config |= ADS1015_CONFIG_MUX_SINGLE_3;
        break;
    }

    // Set the high threshold register
    // Shift 12-bit results left 4 bits for the ADS1015
    writeRegister(ADS1015_POINTER_HITHRESH, threshold << 4);

    // Write config register to the ADC
    writeRegister(ADS1015_POINTER_CONFIG, config);
}

/*
 * @fn      getLastConversionResults
 * @brief   reads the last conversion results without changing the config value
 * @param   none
 * @return  conversion result
 */
int16_t getLastConversionResults()
{
    // Wait for the conversion to complete
    sleep(ADS1015_DELAY);

    // Read the conversion results
    uint16_t result = readRegister(ADS1015_POINTER_CONVERT) >> 4;

    // Shift 12-bit results right 4 bits for the ADS1015, making sure we keep the sign bit intact
    if (result > 0x07FF)
    {
      // negative number - extend the sign to 16th bit
      result |= 0xF000;
    }
    return (int16_t)result;
}

/*
 * @fn      generalCall
 * @brief   general call to I²C devices
 * @param   none
 * @return  None
 */
void generalCall()
{
    i2cTransaction.slaveAddress = 0x00;

    txBuffer[0] = 0x06;

    i2cTransaction.readBuf    = rxBuffer;
    i2cTransaction.readCount  = 0;
    i2cTransaction.writeBuf   = txBuffer;
    i2cTransaction.writeCount = 1;

    if(I2C_transfer(i2c, &i2cTransaction))
        {
            Display_printf(displayHandle, 0,0,"I2C reset success!");
        }
    else{
            Display_printf(displayHandle, 0,0,"I2C reset fail!");
            i2cErrorHandler(&i2cTransaction);
        }
}

/*
 * @fn      i2cErrorHandler
 * @brief   Handle I²C error
 * @param   *transaction - current transaction
 * @return  None
 */
void i2cErrorHandler(I2C_Transaction *transaction)
{
    switch (transaction->status) {
    case I2C_STATUS_TIMEOUT:
        Display_printf(displayHandle, 0, 0,"I2C transaction timed out!");
        break;
    case I2C_STATUS_CLOCK_TIMEOUT:
        Display_printf(displayHandle, 0, 0,"I2C serial clock line timed out!");
        break;
    case I2C_STATUS_ADDR_NACK:
        Display_printf(displayHandle, 0, 0,"I2C slave address not acknowledged!");
        break;
    case I2C_STATUS_DATA_NACK:
        Display_printf(displayHandle, 0, 0,"I2C data byte not acknowledged!");
        break;
    case I2C_STATUS_ARB_LOST:
        Display_printf(displayHandle, 0, 0,"I2C arbitration to another master!");
        break;
    case I2C_STATUS_INCOMPLETE:
        Display_printf(displayHandle, 0, 0,"I2C transaction returned before completion!");
        break;
    case I2C_STATUS_BUS_BUSY:
        Display_printf(displayHandle, 0, 0,"I2C bus is already in use!");
        break;
    case I2C_STATUS_CANCEL:
        Display_printf(displayHandle, 0, 0,"I2C transaction cancelled!");
        break;
    case I2C_STATUS_INVALID_TRANS:
        Display_printf(displayHandle, 0, 0,"I2C transaction invalid!");
        break;
    case I2C_STATUS_ERROR:
        Display_printf(displayHandle, 0, 0,"I2C generic error!");
        break;
    default:
        Display_printf(displayHandle, 0, 0,"I2C undefined error case!");
        break;
    }
}
