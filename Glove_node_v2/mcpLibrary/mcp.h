/*
 * mcp.h
 *
 *  Created on: 15-Apr-2022
 *      Author: sujaj
 */

#ifndef MCPLIBRARY_MCP_H_
#define MCPLIBRARY_MCP_H_

/******************************************************************************
* Includes
*******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/display/Display.h>

#include "mcpLibrary/mcp_hw.h"

/* Driver configuration */
#include "ti_drivers_config.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
#define MSG_SIZE            (8)
#define DATA_SIZE           (2)
#define PAYLOAD_SIZE        (14)

#define MCP_OK             0
#define MCP_MODE_FAULT     1
#define MCP_HW_ERR         2
#define MCP_TX_ERR         3
#define MCP_RX_ERR         4

/******************************************************************************
* Configuration Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Variables
*******************************************************************************/

Display_Handle displayHandle;

SPI_Handle      spiHandle;
SPI_Params      spiParams;
SPI_Transaction spiTransaction;

bool            transferOK;

uint8_t transmitBuffer  [PAYLOAD_SIZE+1];
uint8_t receiveBuffer   [PAYLOAD_SIZE+1];

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/*
 * MCP initialization
 */
int mcp_init (can_opmode mode);

/*
 * change MCP operation mode
 */
int mcp_mode (can_opmode mode);

/*
 * check for errors
 */
int mcp_com_error ( bool clear);

/*
 * configure bit timing logic
 */
int mcp_btl_config ( uint8_t BRP, uint8_t SJW, uint8_t PRSEG, uint8_t PHSEG1, uint8_t PHSEG2, bool SAM, bool BTLMODE, bool WAKFIL, bool SOFR);

/*
 * Configure transmit buffer
 */
int mcp_tx_config ( TXB_t tx_buff, uint8_t TXP, bool use_RTS_pin );

/*
 * Configure receive buffer
 */
int mcp_rx_config ( RXB_t rx_buff, rx_mode_t mode, bool BUKT, bool RX_PIN);

/*
 * Configure receive buffer filter
 */
int mcp_filter_config ( RXF_t rx_filt, uint16_t filter_SID, uint32_t filter_EID, bool filter_EXIDE );

/*
 * Configure receive buffer mask
 */
int mcp_mask_config ( RXB_t rx_buff, uint16_t mask_SID, uint32_t mask_EID);

/*
 * Load message to transmit buffer
 */
//int mcp_msg_load ( TXB_t tx_buffer, uint8_t *msg, uint8_t count, uint32_t ID, bool EXIDE, bool RTR );
//int mcp_msg_load ( TXB_t tx_buffer, char *msg, uint8_t count, uint32_t ID, bool EXIDE, bool RTR );
int mcp_msg_load ( TXB_t tx_buffer, uint16_t *msg, uint8_t count, uint32_t ID, bool EXIDE, bool RTR );

/*
 * Send message from transmit buffer
 */
int mcp_msg_send ( TXB_t tx_buff);

/*
 * check if message ready in receive buffers
 */
bool mcp_msg_ready ( RXB_t rx_buffer);

/*
 * Read message from receive buffer
 */
int mcp_msg_read ( RXB_t rx_buffer, uint8_t *msg, uint8_t *count, uint32_t *ID, bool *IDE, bool *RTR );

#endif /* MCPLIBRARY_MCP_H_ */
