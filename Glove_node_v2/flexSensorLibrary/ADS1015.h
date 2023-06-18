/*
 * ADS1015.h
 *
 * Author: Suja Joseph
 */

/*
 * Includes
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <ti/display/Display.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

#include "ti_drivers_config.h"

/*
 *  Constants and macros
 */
#ifndef FLEXSENSORLIBRARY_ADS1015_H_
#define FLEXSENSORLIBRARY_ADS1015_H_

/* I²C address */
#define ADS1015_ADDRESS_GND 0x48
#define ADS1015_ADDRESS_VDD 0x49
#define ADS1015_ADDRESS_SDA 0x4A
#define ADS1015_ADDRESS_SCL 0x4B

#define ADS1015_DELAY                (1)

/* Register address */
#define ADS1015_POINTER_CONVERT      (0x00)
#define ADS1015_POINTER_CONFIG       (0x01)
#define ADS1015_POINTER_LOWTHRESH    (0x02)
#define ADS1015_POINTER_HITHRESH     (0x03)

/* Config register setting */
#define ADS1015_CONFIG_OS_NO         (0x8000)
#define ADS1015_CONFIG_OS_SINGLE     (0x8000)
#define ADS1015_CONFIG_OS_READY      (0x0000)
#define ADS1015_CONFIG_OS_NOTREADY   (0x8000)

/* Mode setting */
#define ADS1015_CONFIG_MODE_CONT     (0x0000)
#define ADS1015_CONFIG_MODE_SINGLE   (0x0100)

/* MUX setting */
#define ADS1015_CONFIG_MUX_SINGLE_0    (0x4000)
#define ADS1015_CONFIG_MUX_SINGLE_1    (0x5000)
#define ADS1015_CONFIG_MUX_SINGLE_2    (0x6000)
#define ADS1015_CONFIG_MUX_SINGLE_3    (0x7000)
#define ADS1015_CONFIG_MUX_DIFF_P0_N1  (0x0000)
#define ADS1015_CONFIG_MUX_DIFF_P0_N3  (0x1000)
#define ADS1015_CONFIG_MUX_DIFF_P1_N3  (0x2000)
#define ADS1015_CONFIG_MUX_DIFF_P2_N3  (0x3000)

/* sample rate setting */
#define ADS1015_CONFIG_RATE_128HZ    (0x0000)
#define ADS1015_CONFIG_RATE_250HZ    (0x0020)
#define ADS1015_CONFIG_RATE_490HZ    (0x0040)
#define ADS1015_CONFIG_RATE_920HZ    (0x0060)
#define ADS1015_CONFIG_RATE_1600HZ   (0x0080)
#define ADS1015_CONFIG_RATE_2400HZ   (0x00A0)
#define ADS1015_CONFIG_RATE_3300HZ   (0x00C0)

/* PGA setting */
#define ADS1015_CONFIG_PGA_MASK      (0X0E00)
#define ADS1015_CONFIG_PGA_TWOTHIRDS (0X0000)  // +/- 6.144v
#define ADS1015_CONFIG_PGA_1         (0X0200)  // +/- 4.096v
#define ADS1015_CONFIG_PGA_2         (0X0400)  // +/- 2.048v
#define ADS1015_CONFIG_PGA_4         (0X0600)  // +/- 1.024v
#define ADS1015_CONFIG_PGA_8         (0X0800)  // +/- 0.512v
#define ADS1015_CONFIG_PGA_16        (0X0A00)  // +/- 0.256v

/* Comparator setting */
#define ADS1015_CONFIG_CMODE_TRAD   (0x0000)  // Traditional comparator with hysteresis (default)
#define ADS1015_CONFIG_CMODE_WINDOW (0x0010)  // Window comparator
#define ADS1015_CONFIG_CPOL_ACTVLOW (0x0000)  // ALERT/RDY pin is low when active (default)
#define ADS1015_CONFIG_CPOL_ACTVHI  (0x0008)  // ALERT/RDY pin is high when active
#define ADS1015_CONFIG_CLAT_NONLAT  (0x0000)  // Non-latching comparator (default)
#define ADS1015_CONFIG_CLAT_LATCH   (0x0004)  // Latching comparator
#define ADS1015_CONFIG_CQUE_1CONV   (0x0000)  // Assert ALERT/RDY after one conversions
#define ADS1015_CONFIG_CQUE_2CONV   (0x0001)  // Assert ALERT/RDY after two conversions
#define ADS1015_CONFIG_CQUE_4CONV   (0x0002)  // Assert ALERT/RDY after four conversions
#define ADS1015_CONFIG_CQUE_NONE    (0x0003)  // Disable the comparator and put ALERT/RDY in high state (default)


Display_Handle  displayHandle;
I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;

uint8_t         txBuffer[5];
uint8_t         rxBuffer[5];

int channel, hiLo;
int finger;

uint16_t readRegister(uint8_t location);
void writeRegister(uint8_t location, uint16_t val);

uint16_t getSingleEnded (uint8_t channel); ////Returns the decimal value of sensor channel single-ended input
uint16_t getDifferential(uint16_t CONFIG_MUX_DIFF);
uint16_t getAnalogData(uint8_t channel);
float getScaledAnalogData (uint8_t channel);

void calibrate ();
uint16_t getCalibration(uint8_t channel, bool hiLo);
void setCalibration(uint8_t channel, bool hiLo, uint16_t value);
void resetCalibration();
float mapf(float val, float in_min, float in_max, float out_min, float out_max);

bool available();

void setMode(uint16_t mode);
uint16_t getMode ();

void setGain (uint16_t gain);
uint16_t getGain ();

void setSampleRate (uint16_t sampleRate);
uint16_t getSampleRate ();

void updateMultiplierToVolts(uint16_t _gain);
float getMultiplier();

void setComparatorSingleEnded(uint8_t channel, int16_t threshold);

int16_t getLastConversionResults();

void generalCall();

void i2cErrorHandler(I2C_Transaction *transaction);

#endif /* FLEXSENSORLIBRARY_ADS1015_H_ */
