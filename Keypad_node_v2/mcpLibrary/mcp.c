/*
 *  mcp.c
 *
 *  Created on: 15-Apr-2022
 *  Author: sujaj
 */

/*
 * Includes
 */

#include <mcpLibrary/mcp.h>

/*
 * Module Preprocessor Constants
 */
#define MCP_RTS_ERR    8
#define MCP_RXP_ERR    9
#define MCP_CTL_ERR    7
#define MCP_COM_ERR    6
/*
 * Module Preprocessor Macros
 */

/*
 * Module Typedefs
 */

/*
 * Module Variable Definitions
 */

/* CONTROL TYPES */

static mcp_can_ctl      can_ctl;
static mcp_rts_ctl      rts_pins;
static mcp_rxp_ctl      rx_pins;
static mcp_int_ctl      ie_ctl;
static mcp_txb_ctl      tx_ctl;
static mcp_rxb0_ctl     rx0_ctl;
static mcp_rxb1_ctl     rx1_ctl;
static mcp_cnf1_ctl     cnf_1;
static mcp_cnf2_ctl     cnf_2;
static mcp_cnf3_ctl     cnf_3;
static can_opmode       default_mode;

/* TRANSFER TYPE */
static mcp_transfer    transfer;

/* STATUS TYPES */
static mcp_can_stat         can_stat;
static rx_fstatus           rx_status;
static can_fstatus          status;

/*
 * Function Prototypes
 */
static void unpack_sid ( mcp_id *id, uint32_t *SID);
static void unpack_eid ( mcp_id *id, uint32_t *EID);
static void pack_sid ( uint32_t SID, mcp_id *id );
static void pack_eid ( uint32_t EID, mcp_id *id );

/*
 * Private Function Definitions
 */

/*
 * @fn      unpack_sid
 * @brief   get standard identifier
 * @param   id  - id variable
 * @param   SID - ID
 * @return  0 - successful
 */
static void unpack_sid ( mcp_id *id, uint32_t *SID )
{
    *SID = 0;
    *SID |= id->sidh;
    *SID <<= 3;
    *SID |= id->sidl.sidl;
}

/*
 * @fn      unpack_eid
 * @brief   get Extended identifier
 * @param   id  - id variable
 * @param   EID - ID
 * @return  0 - successful
 */
static void unpack_eid ( mcp_id *id, uint32_t *EID )
{
    *EID = 0;
    *EID |= id->sidh;
    *EID <<= 3;
    *EID |= id->sidl.sidl;
    *EID <<= 2;
    *EID |= id->sidl.eidt;
    *EID <<= 8;
    *EID |= id->eidh;
    *EID <<= 8;
    *EID |= id->eidl;
}

/*
 * @fn      pack_sid
 * @brief   form standard identifier
 * @param   SID - ID
 * @param   id  - id variable
 * @return  0 - successful
 */
static void pack_sid ( uint32_t SID, mcp_id *id)
{
    id->sidh = ( SID & 0x07F8 ) >> 3;
    id->sidl.sidl = ( SID & 0x0007 );
}

/*
 * @fn      pack_eid
 * @brief   form Extended identifier
 * @param   EID - ID
 * @param   id  - id variable
 * @return  0 - successful
 */
static void pack_eid ( uint32_t EID, mcp_id *id)
{
    id->sidh  = ( EID & 0x1FE00000 ) >> 21;
    id->sidl.sidl  = ( EID & 0x001C0000 ) >> 18;
    id->sidl.eidt  = ( EID & 0x00030000 ) >> 16;
    id->eidh  = ( EID & 0x0000FF00 ) >> 8;
    id->eidl  = ( EID & 0x000000FF );
}
/*
 * Public Function Definitions
 */

/*
 * @fn      mcp_init
 * @brief   initialize mcp registers
 * @param   mode  - operational mode
 * @return  0 - successful
 */
int mcp_init (can_opmode mode) {

    can_ctl.reg     = CTL_CAN;  // CANCTRL register
    rts_pins.reg    = CTL_RTS;  // TXRTSCTRL register
    rx_pins.reg     = CTL_RXP;  // BFPCTRL register
    tx_ctl.reg      = CTL_TXB;  // TXBnCTRL register
    rx0_ctl.reg     = CTL_RXB0; // RXB0CTRL register
    rx1_ctl.reg     = CTL_RXB1; // RXB1CTRL register
    cnf_1.reg       = CTL_CNF1; // CNF1 register
    cnf_2.reg       = CTL_CNF2; // CNF2 register
    cnf_3.reg       = CTL_CNF3; // CNF3 register
    default_mode = mode;        // operational mode

    /* CANCTRL setting*/
    can_ctl.clkpre  = 0;
    can_ctl.clken   = false;
    can_ctl.osm     = false;
//    can_ctl.abat    = true;
    can_ctl.abat    = false;
    can_ctl.reqop   = OPMODE_CONFIG;

    if (mcp_hw_ctl_set((void*) &can_ctl)) // write to CANCTRL register
        return MCP_CTL_ERR;

    /* TXRTSCTRL setting */
    rts_pins.p0_mode = true;
    rts_pins.p1_mode = true;
    rts_pins.p2_mode = false;

    if( mcp_hw_ctl_set((void*) &rts_pins)) // write to TXRTSCTRL register
        return MCP_RTS_ERR;

    /* BFPCTRL setting */
    rx_pins.p0_mode = true;
    rx_pins.p1_mode = true;
    rx_pins.p0_enab = true;
    rx_pins.p1_enab = true;

    if ( mcp_hw_ctl_set ((void*)&rx_pins)) // write to BFPCTRL register
        return MCP_RXP_ERR;

    /* TXB1CTRL setting */
    tx_ctl.txp      = 2;
    tx_ctl.txreq    = false;
    tx_ctl.buffer   = TXB0;

    if (mcp_hw_ctl_set (&tx_ctl)) // write to TXB0CTRL register
        return MCP_CTL_ERR;

    tx_ctl.buffer   = TXB1;

    if (mcp_hw_ctl_set (&tx_ctl)) // write to TXB1CTRL register
        return MCP_CTL_ERR;

    tx_ctl.buffer   = TXB2;

    if (mcp_hw_ctl_set (&tx_ctl)) // write to TXB2CTRL register
        return MCP_CTL_ERR;

    /* RXB0CTRL setting */
    rx0_ctl.bukt    = false;
    rx0_ctl.rxm     = RX_MODE_SID;

    /* RXB1CTRL setting */
    rx1_ctl.rxm     = RX_MODE_SID;

    // write to RXB0CTRL and RXB1CTRL register
    if ( mcp_hw_ctl_set ((void*) &rx0_ctl) ||
         mcp_hw_ctl_set ((void*) &rx1_ctl)   )
        return MCP_CTL_ERR;

    /* CANINTF setting */
    ie_ctl.err     = false;
    ie_ctl.merre   = false;
    ie_ctl.rx0     = false;
    ie_ctl.rx1     = false;
    ie_ctl.tx0     = false;
    ie_ctl.tx1     = false;
    ie_ctl.tx2     = false;
    ie_ctl.wak     = false;
    ie_ctl.reg     = INT_FLG;

    if( mcp_hw_ctl_set( ( void* )&ie_ctl ) ) // write to CANINTF register
            return MCP_CTL_ERR;

    /* CANINTE setting */
    ie_ctl.err     = true;
    ie_ctl.mask    = true;
    ie_ctl.merre   = true;
    ie_ctl.rx0     = true;
    ie_ctl.rx1     = true;
    ie_ctl.tx0     = true;
    ie_ctl.tx1     = true;
    ie_ctl.tx2     = true;
    ie_ctl.wak     = false;
    ie_ctl.reg     = INT_CTL;
    if( mcp_hw_ctl_set( ( void* )&ie_ctl ) ) // write to CANINTE register
        return MCP_CTL_ERR;

    if( mcp_mode( default_mode ) ) // change MCP back to operational mode
            return MCP_MODE_FAULT;

    return MCP_OK;
}

/*
 * @fn      mcp_mode
 * @brief   change mcp operation mode
 * @param   mode  - operational mode
 * @return  0 - successful
 */
int mcp_mode(can_opmode mode) {
    can_ctl.reqop = mode;
    can_ctl.mask = CAN_REQOP;

    mcp_hw_ctl_update ((void*) &can_ctl); // write to CANCTRL

    if( mcp_hw_ctl_get( ( void* )&can_stat ) ) // read CANSTAT
        if( can_stat.opmod != mode ) // check if changed mode is updated in register
            return MCP_CTL_ERR;

    return MCP_OK;

}

/*
 * @fn      mcp_com_error
 * @brief   check for errors- checks interrupt register
 * @param   clear  - 1, clear
 * @return  0 - successful
 */
int mcp_com_error ( bool clear)
{
    ie_ctl.reg = INT_FLG; //CANINTF

    mcp_hw_ctl_get( &ie_ctl ); // Read CANINTF

    if( ie_ctl.merre ) // if Message Error Interrupt Flag bit has interrupt pending
    {
        ie_ctl.merre = 0;
        ie_ctl.mask = CAN_MERRE;

        if( clear )
            mcp_hw_ctl_update( &ie_ctl ); // write to CANINTF

        return MCP_COM_ERR;
    }

    return MCP_OK;
}

/*
 * @fn      mcp_btl_config
 * @brief   configure bit time
 * @param   BRP      - Baud Rate Prescaler bits
 * @param   SJW      - Synchronization Jump Width Length bits
 * @param   PRSEG    - Propagation Segment Length bits
 * @param   PHSEG1   - PS1 Length bits
 * @param   PHSEG2   - PS2 Length bits
 * @param   SAM      - Sample Point Configuration bit
 * @param   BTLMODE  - PS2 Bit Time Length bit
 * @param   WAKFIL   - Wake-up Filter bit
 * @param   SOFR     - Start-of-Frame Signal bit
 * @return  0 - successful
 */
int mcp_btl_config ( uint8_t BRP, uint8_t SJW, uint8_t PRSEG, uint8_t PHSEG1, uint8_t PHSEG2, bool SAM, bool BTLMODE, bool WAKFIL, bool SOFR)
{
    if( mcp_mode( OPMODE_CONFIG ) ) // set MCP to configuration mode
        return MCP_MODE_FAULT;

    cnf_1.brp       = BRP;
    cnf_1.sjw       = SJW;
    cnf_2.btlmode   = BTLMODE;
    cnf_2.sam       = SAM;
    cnf_2.prseg     = PRSEG;
    cnf_2.phseg1    = PHSEG1;
    cnf_3.phseg2    = PHSEG2;
    cnf_3.sof       = SOFR;
    cnf_3.wakfil    = WAKFIL;

    // update CNF1, CNF2, CNF3 registers
    if( mcp_hw_ctl_set( ( void* )&cnf_1 ) || \
        mcp_hw_ctl_set( ( void* )&cnf_2 ) || \
        mcp_hw_ctl_set( ( void* )&cnf_3 ) )
       return MCP_CTL_ERR;

    if( mcp_mode( default_mode ) ) // change MCP back to operational mode
        return MCP_MODE_FAULT;

    return MCP_OK;
}

/*
 * @fn      mcp_tx_config
 * @brief   configure transmit buffers
 * @param   tx_buff        - transmit buffer
 * @param   TXP            - Transmit Buffer Priority bits
 * @param   use_RTS_pin    - use pin to request message transmission
 * @return  0 - successful
 */
int mcp_tx_config ( TXB_t tx_buff, uint8_t TXP, bool use_RTS_pin ) {
    tx_ctl.buffer = tx_buff;
    tx_ctl.txp = TXP;

    if( mcp_mode( OPMODE_CONFIG ) ) // set MCP to configuration mode
        return MCP_MODE_FAULT;

    if( mcp_hw_ctl_set( ( void* )&tx_ctl ) ) // write to TXBnCTRL register
        return MCP_CTL_ERR;

    switch ( tx_buff )
    {
        case TXB0:

            rts_pins.p0_mode = use_RTS_pin;
            rts_pins.mask = RTS_P0_MODE;

            if( mcp_hw_ctl_update( ( void* )&rts_pins ) ) // write to TXnRTS register
                return MCP_CTL_ERR;
        break;
        case TXB1:

            rts_pins.p1_mode = use_RTS_pin;
            rts_pins.mask = RTS_P1_MODE;

            if( mcp_hw_ctl_update( ( void* )&rts_pins ) ) // write to TXnRTS register
                return MCP_CTL_ERR;
        break;
        case TXB2:

            rts_pins.p2_mode = use_RTS_pin;
            rts_pins.mask = RTS_P2_MODE;

            if( mcp_hw_ctl_update( ( void* )&rts_pins ) ) // write to TXnRTS register
                return MCP_CTL_ERR;
        break;
    }

    if( mcp_mode( default_mode ) ) // change MCP back to operational mode
        return MCP_MODE_FAULT;

    return MCP_OK;
}

/*
 * @fn      mcp_rx_config
 * @brief   configure receive buffers
 * @param   rx_buff   - receive buffer
 * @param   mode      - Receive Buffer Operating mode (to filter on EID or SID or accept all)
 * @param   BUKT      - Rollover Enable bit
 * @param   RX_PIN    - Receive buffer full pin Enable bit
 * @return  0 - successful
 */
int mcp_rx_config ( RXB_t rx_buff, rx_mode_t mode, bool BUKT, bool RX_PIN) {
    switch( rx_buff )
        {
            case RXB0 :
                rx0_ctl.bukt = BUKT;
                rx0_ctl.rxm = mode;

                if( mcp_hw_ctl_set( ( void* )&rx0_ctl ) ) // write to RXB0CTRL register
                    return MCP_CTL_ERR;

                rx_pins.p0_enab = RX_PIN;
                rx_pins.p0_mode = true;

                if( mcp_hw_ctl_set( ( void* )&rx_pins ) ) // write to BFPCTRL register
                    return MCP_CTL_ERR;
                break;

            case RXB1 :

                rx1_ctl.rxm = mode;

                if( mcp_hw_ctl_set( ( void* )&rx1_ctl ) ) // write to RXB1CTRL register
                    return MCP_CTL_ERR;

                rx_pins.p1_enab = RX_PIN;
                rx_pins.p1_mode = true;

                if( mcp_hw_ctl_set( ( void* )&rx_pins ) ) // write to BFPCTRL register
                    return MCP_CTL_ERR;
            break;
        }
    return MCP_OK;
}

/*
 * @fn      mcp_filter_config
 * @brief   configure receive buffer filter
 * @param   rx_filter - Filter
 * @param   SID       - Standard identifier
 * @param   EID       - Extended identifier
 * @param   EXIDE     - Extended Identifier Enable bit
 * @return  0 - successful
 */
int mcp_filter_config ( RXF_t rx_filter, uint16_t SID, uint32_t EID, bool EXIDE )
{
    if( mcp_mode( OPMODE_CONFIG ) ) // set MCP to configuration mode
        return MCP_MODE_FAULT;

    if( ( transfer.id.sidl.ide = EXIDE ) ) //This bit indicates whether the received message was a Standard or an Extended Frame
        pack_eid( EID, &transfer.id );  // if Extended identifer, make EID
    else
        pack_sid( SID, &transfer.id );  // if standard identifer, make SID

    if( mcp_hw_filter_set( rx_filter, &transfer.id ) ) // write to filter register
        return MCP_CTL_ERR;

    if( mcp_mode( default_mode ) ) // change MCP back to operational mode
        return MCP_MODE_FAULT;

    return MCP_OK;
}

/*
 * @fn      mcp_mask_config
 * @brief   configure receive buffer mask
 * @param   rx_mask  - Mask
 * @param   SID      - Standard identifier
 * @param   EID      - Extended identifier
 * @return  0 - successful
 */
int mcp_mask_config ( RXB_t rx_mask, uint16_t SID, uint32_t EID)
{
   pack_eid( EID, &transfer.id );
   pack_sid( SID, &transfer.id );

    if( mcp_hw_mask_set( rx_mask, &transfer.id ) ) //writes data to mask register
        return MCP_CTL_ERR;

    return MCP_OK;
}

/*
 * @fn      mcp_msg_load
 * @brief   load message to transmit buffer
 * @param   tx_buff  - transmit buffer
 * @param   msg        - Message
 * @param   count      - Number of data bytes
 * @param   ID         - ID of message
 * @param   IDE        - Extended identifier bit
 * @param   RTR        - Remote Transmission Request bit
 * @return  0 - successful
 */

//int mcp_msg_load ( TXB_t tx_buff, char *msg, uint8_t count, uint32_t ID, bool IDE, bool RTR )
//int mcp_msg_load ( TXB_t tx_buff, uint8_t *msg, uint8_t count, uint32_t ID, bool IDE, bool RTR )

int mcp_msg_load ( TXB_t tx_buff, uint16_t *msg, uint8_t count, uint32_t ID, bool IDE, bool RTR )
{
    tx_ctl.buffer = tx_buff;
    tx_ctl.txreq = false;
    tx_ctl.mask = TXB_TXREQ; // TXBnCTRL

    if( mcp_mode( OPMODE_CONFIG ) ) // set MCP to configuration mode
        return MCP_MODE_FAULT;

    if( mcp_hw_ctl_update( &tx_ctl ) )
        return MCP_CTL_ERR;

    if( mcp_mode( default_mode ) ) // set MCP back to operational mode
        return MCP_MODE_FAULT;

    if( ( transfer.id.sidl.ide = IDE ) ) // if extended identifier
    {
        pack_eid( ID, &transfer.id ); // pack ID
        Display_printf(displayHandle, 0, 0, "Transmitted ID: %i", ID);

    //        Display_printf(displayHandle, 0, 0, "identifies as Extended identifier");
    //        Display_printf( displayHandle, 0, 0, "EID: %i, %i, %i, %i, %i ", transfer.id.sidh, transfer.id.sidl.sidl, transfer.id.sidl.eidt, transfer.id.eidh, transfer.id.eidl  );
      }
    else // if standard identifier
    {
        pack_sid( ID, &transfer.id );
        Display_printf(displayHandle, 0, 0, "Transmitted ID: %i", ID);

    //        Display_printf(displayHandle, 0, 0, "identifies as standard identifier");
    //        Display_printf( displayHandle, 0, 0, "SID: %i, %i, %i, %i, %i ", transfer.id.sidh, transfer.id.sidl.sidl, transfer.id.sidl.eidt, transfer.id.eidh, transfer.id.eidl  );
    }

    transfer.payload.dlc.rtr = RTR; // Remote Transmission Request bit
    transfer.payload.dlc.len = count; //update count

    /* for debugging */
    //    Display_printf (displayHandle, 0, 0, "Data in msg: %d %d", *msg, *(msg+1));
    //    Display_printf (displayHandle, 0, 0, "Data into msg_load from main: %c", *msg);
    //    Display_printf (displayHandle, 0, 0, "Data into msg_load from main: %d", *msg);

    memcpy( ( void* )&transfer.payload.buff, ( void* )msg, count ); // copy message

    /* for debugging */
    //    Display_printf(displayHandle, 0, 0, "Data in payload: %d %d %d %d", transfer.payload.buff[0], transfer.payload.buff[1], transfer.payload.buff[2], transfer.payload.buff[3]);

    //    Display_printf (displayHandle, 0, 0, "Payload: %c", *transfer.payload.buff);
    //    Display_printf (displayHandle, 0, 0, "Payload: %d %d", *transfer.payload.buff, *(transfer.payload.buff+1));
    //    Display_printf(displayHandle, 0, 0, "Data: %d", *msg);

    if( mcp_hw_data_set( tx_buff, &transfer ) ) // write to data register
        return MCP_TX_ERR;

    return MCP_OK;
}

/*
 * @fn      mcp_msg_send
 * @brief   send message from transmit buffer
 * @param   tx_buff  - transmit buffer
 * @return  0 - successful
 */
int mcp_msg_send ( TXB_t tx_buff)
{
    tx_ctl.buffer = tx_buff;
    tx_ctl.txreq = true;     // set this bit to request message be transmitted
    tx_ctl.mask = TXB_TXREQ; //TXBnCTRL register

    if( mcp_hw_ctl_update( ( void* )&tx_ctl ) ) // write to register
        return MCP_CTL_ERR;

    while( tx_ctl.txreq ) { // if message transmission pending, then txreq is 1
        if( mcp_hw_ctl_get( ( void* )&tx_ctl ) ) {
             return MCP_HW_ERR;
        }

    /* for debugging */
    //    Display_printf(displayHandle, 0, 0, "TXB0CTRL values abtf:%d mloa:%d txerr:%d txreq:%d txp:%d", tx_ctl.abtf, tx_ctl.mloa, tx_ctl.txerr,tx_ctl.txreq, tx_ctl.txp);
    //    Display_printf(displayHandle, 0, 0, "TEC");
    //    mcp_read_buff(0x1C);
    //    Display_printf(displayHandle, 0, 0, "EFLG");
    //    mcp_read_buff(0x2D);
    }
    return MCP_OK;
}

/*
 * @fn      mcp_msg_ready
 * @brief   check if message ready in receive buffers
 * @param   rx_buffer  - Receive buffer
 * @return  1 - interrupt pending
 *          0 - no interrupt pending
 */
bool mcp_msg_ready ( RXB_t rx_buffer)
{
    rx_fstatus rx_status;

    mcp_hw_rx_status( &rx_status ); // SPI read status function call

    switch ( rx_buffer )
    {
        case RXB0:
            return rx_status.rxb_0;
        break;
        case RXB1 :
            return rx_status.rxb_1;
        break;
        default:
            return false;
        break;
    }
}

/*
 * @fn      mcp_msg_read
 * @brief   read message from receive buffer
 * @param   rx_buffer  - Receive buffer
 * @param   msg        - Message
 * @param   count      - Number of data bytes
 * @param   ID         - ID of message
 * @param   IDE        - Extended identifier bit
 * @param   RTR        - Remote Transmission Request bit
 * @return  0 if successful
 */

//int mcp_msg_read ( RXB_t rx_buffer, uint16_t *msg, uint8_t *count, uint32_t *ID, bool *IDE, bool *RTR )
//int mcp_msg_read ( RXB_t rx_buffer, char *msg, uint8_t *count, uint32_t *ID, bool *IDE, bool *RTR )

int mcp_msg_read ( RXB_t rx_buffer, uint8_t *msg, uint8_t *count, uint32_t *ID, bool *IDE, bool *RTR )//changed uint8_t *msg to char *msg
{
    if( mcp_hw_data_get( rx_buffer, &transfer ) ) // get data from receive buffer
        return MCP_RX_ERR;

    if( ( *IDE = transfer.id.sidl.ide ) ) // check if extended identifier
    {
        *RTR = transfer.payload.dlc.rtr; // check if remote transmission request
        unpack_eid( &transfer.id, ID ); // unpack message ID

    } else {                            // check if standard identifier

        *RTR = transfer.id.sidl.srr;
        unpack_sid( &transfer.id, ID ); // unpack message ID
    }

    *count = transfer.payload.dlc.len; // copy count

    /* for debugging */
    //    Display_printf(displayHandle, 0, 0, "Count %d", *count );
    //    Display_printf(displayHandle, 0, 0, "Message %s", transfer.payload.buff );

    //    Display_printf(displayHandle, 0, 0, "Payload data: %i %s %s", *transfer.payload.buff, *transfer.payload.buff, transfer.payload.buff);
    //    Display_printf(displayHandle, 0, 0, "Payload data: %s", transfer.payload.buff);

    memcpy( ( void* )msg, ( void* )transfer.payload.buff, *count); // copy received message

    /* for debugging*/
    //    Display_printf(displayHandle, 0, 0, "Message copied %s", msg );
    //    Display_printf (displayHandle, 0, 0, "Payload: %s", transfer.payload.buff);
    //    Display_printf(displayHandle, 0, 0, "count: %d, data: %s ", *count, *msg);

    /* reset CANINTF register */
    ie_ctl.rx0 = false;
    ie_ctl.rx1 = false;
    ie_ctl.mask = CAN_RX0IE | CAN_RX1IE;
    ie_ctl.reg = INT_FLG;

    if( mcp_hw_ctl_update( ( void* )&ie_ctl ) ) // write to CANINTF register
        return MCP_CTL_ERR;

    return MCP_OK;
}
