/*
 * keypad.c
 *
 *  Created on: 13-Oct-2022
 *      Author: sujaj
 */
#include <keypadLibrary/keypad.h>

/* set all columns HIGH */
void setColsHigh() {
    GPIO_write(GPIO_COL_1, GPIO_CFG_OUT_HIGH);
    GPIO_write(GPIO_COL_2, GPIO_CFG_OUT_HIGH);
    GPIO_write(GPIO_COL_3, GPIO_CFG_OUT_HIGH);
    GPIO_write(GPIO_COL_4, GPIO_CFG_OUT_HIGH);
}

/* set Column 1 LOW */
void setCol1() {
    GPIO_write(GPIO_COL_1, GPIO_CFG_OUT_LOW);
}

/* set Column 2 LOW */
void setCol2() {
    GPIO_write(GPIO_COL_1, GPIO_CFG_OUT_HIGH);
    GPIO_write(GPIO_COL_2, GPIO_CFG_OUT_LOW);
}

/* set Column 3 LOW */
void setCol3() {
    GPIO_write(GPIO_COL_1, GPIO_CFG_OUT_HIGH);
    GPIO_write(GPIO_COL_2, GPIO_CFG_OUT_HIGH);
    GPIO_write(GPIO_COL_3, GPIO_CFG_OUT_LOW);
}

/* set Column 4 LOW */
void setCol4() {
    GPIO_write(GPIO_COL_1, GPIO_CFG_OUT_HIGH);
    GPIO_write(GPIO_COL_2, GPIO_CFG_OUT_HIGH);
    GPIO_write(GPIO_COL_3, GPIO_CFG_OUT_HIGH);
    GPIO_write(GPIO_COL_4, GPIO_CFG_OUT_LOW);
}

/* ===== Poll keypad for key pressed =====
 * columns configured as output and rows configured as input
 * each column is set LOW one by one
 * each row is read
 * row would also read LOW if a is key pressed */
uint16_t search() {
        setColsHigh();

        for (col = 1; col<=4; col++) {
            switch (col) {
                case 1:
                    setCol1(); //set Column 1 LOW
                    for (row = 1; row<= 4; row++ ) {
                        switch (row) { //read each row; if key pressed, then row would also read LOW
                            case 1:
                                if (GPIO_read(GPIO_ROW_1) == 0)
                                {
                                return keys[3][3];
                                }
                                break;
                            case 2:
                                if (GPIO_read(GPIO_ROW_2) == 0)
                                {
                                return keys[2][3];
                                }
                                break;
                            case 3:
                                if (GPIO_read(GPIO_ROW_3) == 0)
                                {
                                return keys[1][3];
                                }
                                break;
                            case 4:
                                if (GPIO_read(GPIO_ROW_4) == 0)
                                {
                                return keys[0][3];
                                }
                                break;
                            default:

                         }
                    }
                    break;

                case 2:
                setCol2(); //set Column 2 LOW
                for (row = 1; row<= 4; row++ ) { //read each row; if key pressed, then row would also read LOW
                    switch (row) {
                        case 1:
                            if (GPIO_read(GPIO_ROW_1) == 0)
                            {
                            return keys[3][2];
                            }
                            break;
                        case 2:
                            if (GPIO_read(GPIO_ROW_2) == 0)
                            {
                            return keys[2][2];
                            }
                            break;
                        case 3:
                            if (GPIO_read(GPIO_ROW_3) == 0)
                            {
                            return keys[1][2];
                            }
                            break;
                        case 4:
                            if (GPIO_read(GPIO_ROW_4) == 0)
                            {
                            return keys[0][2];
                            }
                            break;
                        default:

                     }
                }
                break;
                case 3:
                setCol3(); //set Column 3 LOW
                for (row = 1; row<= 4; row++ ) { //read each row; if key pressed, then row would also read LOW
                    switch (row) {
                        case 1:
                            if (GPIO_read(GPIO_ROW_1) == 0)
                            {
                            return keys[3][1];
                            }
                            break;
                        case 2:
                            if (GPIO_read(GPIO_ROW_2) == 0)
                            {
                            return keys[2][1];
                            }
                            break;
                        case 3:
                            if (GPIO_read(GPIO_ROW_3) == 0)
                            {
                            return keys[1][1];
                            }
                           break;
                        case 4:
                            if (GPIO_read(GPIO_ROW_4) == 0)
                            {
                            return keys[0][1];
                            }
                            break;
                        default:
                     }
                }
                break;

                case 4:
                setCol4(); //set Column 4 LOW
                for (row = 1; row<= 4; row++ ) { //read each row; if key pressed, then row would also read LOW
                    switch (row) {
                        case 1:
                            if (GPIO_read(GPIO_ROW_1) == 0)
                            {
                            return keys[3][0];
                            }
                            break;
                        case 2:
                            if (GPIO_read(GPIO_ROW_2) == 0)
                            {
                            return keys[2][0];
                            }
                            break;
                        case 3:
                            if (GPIO_read(GPIO_ROW_3) == 0)
                            {
                            return keys[1][0];
                            }
                            break;
                        case 4:
                            if (GPIO_read(GPIO_ROW_4) == 0)
                            {
                            return keys[0][0];
                            }
                            break;
                        default:

                     }
                }
                break;
                default:
            }
    }
        return 126; // returns ASCII character '~' when no key is pressed
}
