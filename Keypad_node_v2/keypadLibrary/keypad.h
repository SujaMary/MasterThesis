/*
 * Keypad.h
 *
 *  Created on: 13-Oct-2022
 *      Author: sujaj
 */

#ifndef KEYPADLIBRARY_KEYPAD_H_
#define KEYPADLIBRARY_KEYPAD_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

/* Drivers Header files */
#include <ti/drivers/GPIO.h>
#include <ti/display/Display.h>

/* Driver Configuration */
#include "ti_drivers_config.h"

int row, col;

uint16_t keys [4][4] =
    {
     {'1', '2', '3', 'A'},
     {'4', '5', '6', 'B'},
     {'7', '8', '9', 'C'},
     {'*', '0', '#', 'D'}
    };

/* set all columns HIGH */
void setColsHigh ();

/* set Column 1 LOW */
void setCol1 ();

/* set Column 2 LOW */
void setCol2 ();

/* set Column 3 LOW */
void setCol3 ();

/* set Column 4 LOW */
void setCol4 ();

/* Poll keypad for key pressed */
uint16_t search();

#endif /* KEYPADLIBRARY_KEYPAD_H_ */
