/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  The macros defines herein are intended for use by applications which
 *  directly include this header. These macros should NOT be hard coded or
 *  copied into library source code.
 *
 *  Symbols declared as const are intended for use with libraries.
 *  Library source code must extern the correct symbol--which is resolved
 *  when the application is linked.
 *
 *  DO NOT EDIT - This file is generated for the CC26X2R1_LAUNCHXL
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_CC26X2R1_LAUNCHXL
#ifndef DeviceFamily_CC26X2
#define DeviceFamily_CC26X2
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== CCFG ========
 */


/*
 *  ======== GPIO ========
 */

/* DIO29 */
extern const uint_least8_t              SLAVE_SELECT_CONST;
#define SLAVE_SELECT                    0
/* DIO13, LaunchPad Button BTN-1 (Left) */
extern const uint_least8_t              Btn_1_CONST;
#define Btn_1                           1
/* DIO0 */
extern const uint_least8_t              RESET_CONST;
#define RESET                           2
/* DIO1 */
extern const uint_least8_t              STBY_CONST;
#define STBY                            3
/* DIO11 */
extern const uint_least8_t              RTS_TX0_CONST;
#define RTS_TX0                         4
#define CONFIG_TI_DRIVERS_GPIO_COUNT    5

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== I2C ========
 */

/*
 *  SCL: DIO4
 *  SDA: DIO5
 */
extern const uint_least8_t              CONFIG_I2C_ADS_0_CONST;
#define CONFIG_I2C_ADS_0                0
#define CONFIG_TI_DRIVERS_I2C_COUNT     1

/* ======== I2C Addresses and Speeds ======== */
#include <ti/drivers/I2C.h>

/* ---- CONFIG_I2C_ADS_0 I2C bus components ---- */

/* no components connected to CONFIG_I2C_ADS_0 */

/* max speed unspecified, defaulting to 100 Kbps */
#define CONFIG_I2C_ADS_0_MAXSPEED   (100U) /* Kbps */
#define CONFIG_I2C_ADS_0_MAXBITRATE ((I2C_BitRate)I2C_100kHz)


/*
 *  ======== PIN ========
 */
#include <ti/drivers/PIN.h>

extern const PIN_Config BoardGpioInitTable[];

/* Parent Signal: SLAVE_SELECT GPIO Pin, (DIO29) */
#define CONFIG_PIN_2                   0x0000001d
/* LaunchPad Button BTN-1 (Left), Parent Signal: Btn_1 GPIO Pin, (DIO13) */
#define CONFIG_PIN_3                   0x0000000d
/* Parent Signal: RESET GPIO Pin, (DIO0) */
#define CONFIG_PIN_9                   0x00000000
/* Parent Signal: STBY GPIO Pin, (DIO1) */
#define CONFIG_PIN_10                   0x00000001
/* Parent Signal: RTS_TX0 GPIO Pin, (DIO11) */
#define CONFIG_PIN_4                   0x0000000b
/* Parent Signal: CONFIG_I2C_ADS_0 SDA, (DIO5) */
#define CONFIG_PIN_5                   0x00000005
/* Parent Signal: CONFIG_I2C_ADS_0 SCL, (DIO4) */
#define CONFIG_PIN_11                   0x00000004
/* LaunchPad SPI Bus, Parent Signal: CONFIG_SPI_MASTER SCLK, (DIO10) */
#define CONFIG_PIN_6                   0x0000000a
/* LaunchPad SPI Bus, Parent Signal: CONFIG_SPI_MASTER MISO, (DIO8) */
#define CONFIG_PIN_7                   0x00000008
/* LaunchPad SPI Bus, Parent Signal: CONFIG_SPI_MASTER MOSI, (DIO9) */
#define CONFIG_PIN_8                   0x00000009
/* XDS110 UART, Parent Signal: CONFIG_UART_0 TX, (DIO3) */
#define CONFIG_PIN_0                   0x00000003
/* XDS110 UART, Parent Signal: CONFIG_UART_0 RX, (DIO2) */
#define CONFIG_PIN_1                   0x00000002
#define CONFIG_TI_DRIVERS_PIN_COUNT    12


/*
 *  ======== SPI ========
 */

/*
 *  MOSI: DIO9
 *  MISO: DIO8
 *  SCLK: DIO10
 *  LaunchPad SPI Bus
 */
extern const uint_least8_t              CONFIG_SPI_MASTER_CONST;
#define CONFIG_SPI_MASTER               0
#define CONFIG_TI_DRIVERS_SPI_COUNT     1


/*
 *  ======== UART ========
 */

/*
 *  TX: DIO3
 *  RX: DIO2
 *  XDS110 UART
 */
extern const uint_least8_t              CONFIG_UART_0_CONST;
#define CONFIG_UART_0                   0
#define CONFIG_TI_DRIVERS_UART_COUNT    1


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
