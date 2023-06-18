/*
 * glove_node_v2_3.c
 *
 *  Created on: 21-Feb-2023
 *      Author: sujaj
 */
#include <mcpLibrary/mcp.h>
#include <flexSensorLibrary/ADS1015.h>

/*======= masterThread =======*/
void *mainThread( void *arg0) {

    struct flexSensor {
        char  tx_test[MSG_SIZE];
        uint16_t adcReading_finger0;
        uint16_t adcReading_finger1;
    };

    uint8_t count;
    bool RX_IDE, RX_RTR;
    uint8_t rx_test[MSG_SIZE] = {0};

    uint32_t ID = 0;
    uint32_t EID_finger0 = 0x7E8;
    uint32_t SID_finger0 = 0x0B;
    uint32_t SID_finger1 = 134;

    uint16_t glove_buffer;
    uint16_t combined[3];

    struct flexSensor sensorData;
    uint16_t finger0, finger1;

    Display_init();
    GPIO_init();
    SPI_init();
    SPI_Params_init(&spiParams);
    I2C_init();
    I2C_Params_init(&i2cParams);

    /* Open display handle */
    displayHandle = Display_open(Display_Type_UART, NULL);
    if (displayHandle == NULL) {
        while (1);
    }

    /* Open SPI handle */
    spiParams.frameFormat = SPI_POL0_PHA0;
    spiParams.bitRate = 1000000;
    spiHandle = SPI_open(CONFIG_SPI_MASTER, &spiParams);

    if (spiHandle == NULL) {
        while (1);
    }

    /* Open I²C handle */
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(CONFIG_I2C_ADS_0, &i2cParams);
    if (i2c == NULL) {
        while (1);
    }

    /* General call to flex sensor*/
    generalCall();
    sleep(0.5);

    /* Set range of PGA */
    setGain(ADS1015_CONFIG_PGA_1);

    Display_printf(displayHandle, 0, 0, "----------------------------");
    Display_printf(displayHandle, 0, 0, "Node 1");
    Display_printf(displayHandle, 0, 0, "Starting glove controller node");

    /* Reset MCP*/
    GPIO_write(RESET, 0);
    usleep(2);
    GPIO_write(RESET, 1);

    /* Set MCP to normal mode*/
    GPIO_write(STBY, 0);

    /* Initialize MCP */
    if( mcp_init(OPMODE_NORMAL)) {
        Display_printf(displayHandle, 0, 0, "\n--Initialization Error--");
    }
    else {
        Display_printf(displayHandle, 0, 0, "\nMCP25625 initialized");
    }

    /* Configure receive buffer filters */
    if( mcp_filter_config( RXF_0, 0x7E6, 0x7E6, true ) || \
       mcp_filter_config( RXF_1, 0x7E6, 0x7E6, true ) || \
       mcp_filter_config( RXF_2, 0x7E6, 0x7E6, true ) || \
       mcp_filter_config( RXF_3, 0x7E6, 0x7E6, true ) || \
       mcp_filter_config( RXF_4, 0x7E6, 0x7E6, true ) || \
       mcp_filter_config( RXF_5, 0x7E6, 0x7E6, true ) )

        Display_printf(displayHandle, 0, 0," - Filter Config Error\r" );
    else
        Display_printf(displayHandle, 0, 0, " - Filter Config Done\r" );

    /* Configure receive buffer masks */
    if( mcp_mask_config( RXB0, 0x07FF, 0x1FFFFFFF ) || \
       mcp_mask_config( RXB1, 0x07FF, 0x1FFFFFFF ) )
        Display_printf(displayHandle, 0, 0, " - Mask Config Error\r" );
    else
        Display_printf(displayHandle, 0, 0," - Mask Config Done\r" );

    /* Configure bit timing logic register */
    mcp_btl_config(0, 4, 6, 6, 6, true, true, false, false);

    /* Configure receive buffers */
    if( mcp_rx_config( RXB0, RX_MODE_EID, true, false ) || \
        mcp_rx_config( RXB1, RX_MODE_EID, false, false ) )
        Display_printf(displayHandle, 0, 0," - RX Configuration Error\r\n" );
    else
        Display_printf(displayHandle, 0, 0," - RX Config Done\r\n" );

    while(1)
    {
        sleep(1);

        /* Check RXB0 for message */
        if (mcp_msg_ready (RXB0))
        {
            /* Read message from RXB0 */
            if(mcp_msg_read(RXB0, rx_test, &count, &ID, &RX_IDE, &RX_RTR))
            {
                Display_printf(displayHandle, 0, 0, "RX Error");
            }
            Display_printf(displayHandle, 0, 0, "--- Message received ---");
            Display_printf(displayHandle, 0, 0, "Receive buffer: RX0");
            Display_printf(displayHandle, 0, 0, "Received ID: %i", ID);
            Display_printf(displayHandle, 0, 0, "Tag: %d\n", *rx_test);
        }

        /* Check RXB1 for message */
        if (mcp_msg_ready (RXB1))
        {
            /* Read message from RXB1 */
            if(mcp_msg_read(RXB1, rx_test, &count, &ID, &RX_IDE, &RX_RTR))
            {
                Display_printf(displayHandle, 0, 0, "RX Error");
            }
            Display_printf(displayHandle, 0, 0, "--- Message received ---");
            Display_printf(displayHandle, 0, 0, "Receive buffer: RX1");
            Display_printf(displayHandle, 0, 0, "Received ID: %i", ID);
            Display_printf(displayHandle, 0, 0, "Tag: %d\n", *rx_test);
         }

        /* Read sensor data */
        finger0 = getSingleEnded(0);
        finger1 = getSingleEnded(1);

        sensorData.adcReading_finger0 = finger0;
        sensorData.adcReading_finger1 = finger1;

        Display_printf(displayHandle, 0, 0, "Finger 1: %d Finger 2: %d\n", finger0, finger1);

        /* Check received message ID */
        if (ID == 0x7E6)
        {
            /* Combine node data and received message to transmit */
            combined[0] = sensorData.adcReading_finger0;
            combined[1] = sensorData.adcReading_finger1;
            combined[2] = *rx_test;
            Display_printf (displayHandle, 0, 0, "--- Message transmitted ---");
            Display_printf(displayHandle, 0, 0, "Data - Finger 1: %d Finger 2: %d \r\nTag: %d", combined[0], combined[1], combined[2]);

            /* Transmit message */
            if( !mcp_msg_load(TXB0, combined, 6, EID_finger0, true, false) && \
                             !mcp_msg_send(TXB0))
            {
                Display_printf(displayHandle, 0, 0, "MESSAGE SENT\n");
                ID = 0;
            }
            else
            {
                Display_printf(displayHandle, 0, 0, "ERROR\n");
            }
        }
    }

    Display_close(displayHandle);
    SPI_close(spiHandle);

    return (NULL);
    }
