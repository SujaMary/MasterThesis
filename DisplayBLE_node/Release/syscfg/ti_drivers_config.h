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
 *  ======== AESCCM ========
 */

extern const uint_least8_t                  CONFIG_AESCCM0_CONST;
#define CONFIG_AESCCM0                      0
#define CONFIG_TI_DRIVERS_AESCCM_COUNT      1


/*
 *  ======== AESCTR ========
 */

extern const uint_least8_t                  CONFIG_AESCTR_0_CONST;
#define CONFIG_AESCTR_0                     0
#define CONFIG_TI_DRIVERS_AESCTR_COUNT      1


/*
 *  ======== AESCTRDRBG ========
 */

extern const uint_least8_t                      CONFIG_AESCTRDRBG_0_CONST;
#define CONFIG_AESCTRDRBG_0                     0
#define CONFIG_TI_DRIVERS_AESCTRDRBG_COUNT      1


/*
 *  ======== AESECB ========
 */

extern const uint_least8_t                  CONFIG_AESECB0_CONST;
#define CONFIG_AESECB0                      0
#define CONFIG_TI_DRIVERS_AESECB_COUNT      1


/*
 *  ======== ECDH ========
 */

extern const uint_least8_t              CONFIG_ECDH0_CONST;
#define CONFIG_ECDH0                    0
#define CONFIG_TI_DRIVERS_ECDH_COUNT    1


/*
 *  ======== GPIO ========
 */

/* DIO13, LaunchPad Button BTN-1 (Left) */
extern const uint_least8_t              CONFIG_GPIO_BTN1_CONST;
#define CONFIG_GPIO_BTN1                0
/* DIO14, LaunchPad Button BTN-2 (Right) */
extern const uint_least8_t              CONFIG_GPIO_BTN2_CONST;
#define CONFIG_GPIO_BTN2                1
/* DIO6, LaunchPad LED Red */
extern const uint_least8_t              CONFIG_GPIO_LED_0_CONST;
#define CONFIG_GPIO_LED_0               2
/* DIO29 */
extern const uint_least8_t              SLAVE_SELECT_CONST;
#define SLAVE_SELECT                    3
/* DIO0 */
extern const uint_least8_t              RESET_CONST;
#define RESET                           4
/* DIO1 */
extern const uint_least8_t              STBY_CONST;
#define STBY                            5
/* DIO22 */
extern const uint_least8_t              CONFIG_GPIO_0_CONST;
#define CONFIG_GPIO_0                   6
/* DIO23 */
extern const uint_least8_t              CONFIG_GPIO_1_CONST;
#define CONFIG_GPIO_1                   7
/* DIO24 */
extern const uint_least8_t              CONFIG_GPIO_2_CONST;
#define CONFIG_GPIO_2                   8
#define CONFIG_TI_DRIVERS_GPIO_COUNT    9

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== NVS ========
 */

extern const uint_least8_t              CONFIG_NVSINTERNAL_CONST;
#define CONFIG_NVSINTERNAL              0
#define CONFIG_TI_DRIVERS_NVS_COUNT     1


/*
 *  ======== PIN ========
 */
#include <ti/drivers/PIN.h>

extern const PIN_Config BoardGpioInitTable[];

/* LaunchPad Button BTN-1 (Left), Parent Signal: CONFIG_GPIO_BTN1 GPIO Pin, (DIO13) */
#define CONFIG_PIN_BTN1                   0x0000000d
/* LaunchPad Button BTN-2 (Right), Parent Signal: CONFIG_GPIO_BTN2 GPIO Pin, (DIO14) */
#define CONFIG_PIN_BTN2                   0x0000000e
/* LaunchPad LED Red, Parent Signal: CONFIG_GPIO_LED_0 GPIO Pin, (DIO6) */
#define CONFIG_PIN_6                   0x00000006
/* Parent Signal: SLAVE_SELECT GPIO Pin, (DIO29) */
#define CONFIG_PIN_7                   0x0000001d
/* Parent Signal: RESET GPIO Pin, (DIO0) */
#define CONFIG_PIN_8                   0x00000000
/* Parent Signal: STBY GPIO Pin, (DIO1) */
#define CONFIG_PIN_9                   0x00000001
/* Parent Signal: CONFIG_SPI_0 SCLK, (DIO5) */
#define CONFIG_PIN_3                   0x00000005
/* Parent Signal: CONFIG_SPI_0 MISO, (DIO4) */
#define CONFIG_PIN_4                   0x00000004
/* Parent Signal: CONFIG_SPI_0 MOSI, (DIO30) */
#define CONFIG_PIN_5                   0x0000001e
/* LaunchPad SPI Bus, Parent Signal: CONFIG_SPI_MASTER SCLK, (DIO10) */
#define CONFIG_PIN_10                   0x0000000a
/* LaunchPad SPI Bus, Parent Signal: CONFIG_SPI_MASTER MISO, (DIO8) */
#define CONFIG_PIN_11                   0x00000008
/* LaunchPad SPI Bus, Parent Signal: CONFIG_SPI_MASTER MOSI, (DIO9) */
#define CONFIG_PIN_12                   0x00000009
/* XDS110 UART, Parent Signal: CONFIG_UART_0 TX, (DIO3) */
#define CONFIG_PIN_UART_TX                   0x00000003
/* XDS110 UART, Parent Signal: CONFIG_UART_0 RX, (DIO2) */
#define CONFIG_PIN_UART_RX                   0x00000002
/* Parent Signal: CONFIG_GPIO_0 GPIO Pin, (DIO22) */
#define CONFIG_PIN_0                   0x00000016
/* Parent Signal: CONFIG_GPIO_1 GPIO Pin, (DIO23) */
#define CONFIG_PIN_1                   0x00000017
/* Parent Signal: CONFIG_GPIO_2 GPIO Pin, (DIO24) */
#define CONFIG_PIN_2                   0x00000018
#define CONFIG_TI_DRIVERS_PIN_COUNT    17




/*
 *  ======== SPI ========
 */

/*
 *  MOSI: DIO30
 *  MISO: DIO4
 *  SCLK: DIO5
 */
extern const uint_least8_t              CONFIG_SPI_0_CONST;
#define CONFIG_SPI_0                    0
/*
 *  MOSI: DIO9
 *  MISO: DIO8
 *  SCLK: DIO10
 *  LaunchPad SPI Bus
 */
extern const uint_least8_t              CONFIG_SPI_MASTER_CONST;
#define CONFIG_SPI_MASTER               1
#define CONFIG_TI_DRIVERS_SPI_COUNT     2


/*
 *  ======== TRNG ========
 */

extern const uint_least8_t              CONFIG_TRNG_0_CONST;
#define CONFIG_TRNG_0                   0
#define CONFIG_TI_DRIVERS_TRNG_COUNT    1


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
