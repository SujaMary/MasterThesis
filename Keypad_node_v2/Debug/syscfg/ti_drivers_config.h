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

/* DIO5 */
extern const uint_least8_t              SLAVE_SELECT_CONST;
#define SLAVE_SELECT                    0
/* DIO11 */
extern const uint_least8_t              RESET_CONST;
#define RESET                           1
/* DIO12 */
extern const uint_least8_t              STBY_CONST;
#define STBY                            2
/* DIO25 */
extern const uint_least8_t              GPIO_COL_1_CONST;
#define GPIO_COL_1                      3
/* DIO26 */
extern const uint_least8_t              GPIO_COL_2_CONST;
#define GPIO_COL_2                      4
/* DIO27 */
extern const uint_least8_t              GPIO_COL_3_CONST;
#define GPIO_COL_3                      5
/* DIO28 */
extern const uint_least8_t              GPIO_COL_4_CONST;
#define GPIO_COL_4                      6
/* DIO29 */
extern const uint_least8_t              GPIO_ROW_1_CONST;
#define GPIO_ROW_1                      7
/* DIO30 */
extern const uint_least8_t              GPIO_ROW_2_CONST;
#define GPIO_ROW_2                      8
/* DIO0 */
extern const uint_least8_t              GPIO_ROW_3_CONST;
#define GPIO_ROW_3                      9
/* DIO1 */
extern const uint_least8_t              GPIO_ROW_4_CONST;
#define GPIO_ROW_4                      10
#define CONFIG_TI_DRIVERS_GPIO_COUNT    11

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== PIN ========
 */
#include <ti/drivers/PIN.h>

extern const PIN_Config BoardGpioInitTable[];

/* Parent Signal: SLAVE_SELECT GPIO Pin, (DIO5) */
#define CONFIG_PIN_2                   0x00000005
/* Parent Signal: RESET GPIO Pin, (DIO11) */
#define CONFIG_PIN_3                   0x0000000b
/* Parent Signal: STBY GPIO Pin, (DIO12) */
#define CONFIG_PIN_4                   0x0000000c
/* Parent Signal: GPIO_COL_1 GPIO Pin, (DIO25) */
#define CONFIG_PIN_5                   0x00000019
/* Parent Signal: GPIO_COL_2 GPIO Pin, (DIO26) */
#define CONFIG_PIN_9                   0x0000001a
/* Parent Signal: GPIO_COL_3 GPIO Pin, (DIO27) */
#define CONFIG_PIN_10                   0x0000001b
/* Parent Signal: GPIO_COL_4 GPIO Pin, (DIO28) */
#define CONFIG_PIN_11                   0x0000001c
/* Parent Signal: GPIO_ROW_1 GPIO Pin, (DIO29) */
#define CONFIG_PIN_12                   0x0000001d
/* Parent Signal: GPIO_ROW_2 GPIO Pin, (DIO30) */
#define CONFIG_PIN_13                   0x0000001e
/* Parent Signal: GPIO_ROW_3 GPIO Pin, (DIO0) */
#define CONFIG_PIN_14                   0x00000000
/* Parent Signal: GPIO_ROW_4 GPIO Pin, (DIO1) */
#define CONFIG_PIN_15                   0x00000001
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
#define CONFIG_TI_DRIVERS_PIN_COUNT    16


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
