/*
 * mcp_hw.c
 * Hardware Layer
 * Created on: 14-Apr-2022
 * Author: sujaj
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <mcpLibrary/mcp_hw.h>
#include <mcpLibrary/mcp.h>

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/

#define BIT 1

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

typedef struct {

    uint8_t     value;
    uint8_t     address;
    uint8_t     mask;
    uint8_t     buffer;

} reg_t;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

static reg_t reg;

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/* SPI read from registers */
static void mcp_read ( uint8_t reg, uint8_t *buffer);

/* SPI write to registers */
static void mcp_write (uint8_t reg, uint8_t *value, uint8_t count);

/* SPI modifies the registers */
static void mcp_bit_modify (uint8_t reg, uint8_t mask, uint8_t *value);

/* SPI fast TX write */
static void mcp_load_tx (uint8_t tx_reg, uint8_t *buffer);

/* SPI fast RX read */
static void mcp_read_rx (uint8_t rx_reg, uint8_t *buffer);

/******************************************************************************
* Private Function Definitions
*******************************************************************************/

/*
 * @fn      mcp_read_buff
 * @brief   SPI read command
 * @param   reg  - register address
 * @return  none
 */
void mcp_read_buff ( uint8_t reg) {
   transmitBuffer[0] = CMD_READ;              //SPI command to read
   transmitBuffer[1] = reg;                   //Register address to read
   transmitBuffer[2] = 0x00;
   spiTransaction.count = 12;

   spiTransaction.txBuf = (void *)transmitBuffer;
   spiTransaction.rxBuf = (void *)receiveBuffer;
   spiParams.dataSize   = 8; //data frame size in bits

   GPIO_write(SLAVE_SELECT, 0); //SPI slave select asserted

   transferOK = SPI_transfer(spiHandle, &spiTransaction);

   GPIO_write(SLAVE_SELECT, 1); //SPI slave select de-asserted

   if (!transferOK) {
       Display_printf(displayHandle, 0, 0, "SPI Read buff unsuccessful" );
   }
   else {
      Display_printf(displayHandle, 0, 0, "SPI Read buff successful");

      Display_printf(displayHandle, 0, 0, "Transmitted: %d %d %d %d", transmitBuffer[0], transmitBuffer[1], transmitBuffer[2], transmitBuffer[3]);

      Display_printf(displayHandle, 0, 0, "mcp_read_buffer Received: %d %d %d %d %d %d %d %d %d %d %d %d", receiveBuffer[0], receiveBuffer[1], receiveBuffer[2], receiveBuffer[3], receiveBuffer[4], receiveBuffer[5], receiveBuffer[6], receiveBuffer[7], receiveBuffer[8], receiveBuffer[9], receiveBuffer[10], receiveBuffer[11]);
   }

   //reset array
   transmitBuffer[0] = 0;
   transmitBuffer[1] = 0;
   transmitBuffer[2] = 0;
   transmitBuffer[3] = 0;
   receiveBuffer[0]  = 0;
   receiveBuffer[1]  = 0;
   receiveBuffer[2]  = 0;
   receiveBuffer[3]  = 0;
}

/*
 * @fn      mcp_read
 * @brief   SPI read one register
 * @param   reg  - register address
 * @return  none
 * for debugging
 */
static void mcp_read ( uint8_t reg, uint8_t *buffer) {
    transmitBuffer[0] = CMD_READ;              //SPI command to read
    transmitBuffer[1] = reg;                   //Register address to read
    transmitBuffer[2] = 0x00;

    spiTransaction.count = 3; //CMD+address+0x00

    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;
    spiParams.dataSize   = 8;

    GPIO_write(SLAVE_SELECT, 0);

    transferOK = SPI_transfer(spiHandle, &spiTransaction);

    GPIO_write(SLAVE_SELECT, 1);

    if (!transferOK) {
        //        Display_printf(displayHandle, 0, 0, "SPI Read unsuccessful" );
    }
    else {
        //       Display_printf(displayHandle, 0, 0, "SPI Read successful");
        //        Display_printf(displayHandle, 0, 0, "Transmitted: %d %d %d %d", transmitBuffer[0], transmitBuffer[1], transmitBuffer[2], transmitBuffer[3]);
        //        Display_printf(displayHandle, 0, 0, "Received: %d %d %d %d\n", receiveBuffer[0], receiveBuffer[1], receiveBuffer[2], receiveBuffer[3]);
    }

    *buffer = receiveBuffer[2];

    transmitBuffer[0] = 0;
    transmitBuffer[1] = 0;
    transmitBuffer[2] = 0;
    transmitBuffer[3] = 0;
    receiveBuffer[0]  = 0;
    receiveBuffer[1]  = 0;
    receiveBuffer[2]  = 0;
    receiveBuffer[3]  = 0;
}

/*
 * @fn      mcp_write
 * @brief   SPI Write command
 * @param   addr  - register address
 * @param   value - value to be written
 * @param   count - number of bytes
 * @return  none
 */
static void mcp_write (uint8_t addr, uint8_t *value, uint8_t count) {
    transmitBuffer[0] = CMD_WRITE; //SPI write command
    transmitBuffer[1] = addr;
    transmitBuffer[2] = *value;
    transmitBuffer[3] = *(value+1);
    transmitBuffer[4] = *(value+2);
    transmitBuffer[5] = *(value+3);
    spiTransaction.count = count;

    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;
    spiParams.dataSize   = 8;

    GPIO_write(SLAVE_SELECT, 0);

    transferOK = SPI_transfer(spiHandle, &spiTransaction);

    GPIO_write(SLAVE_SELECT, 1);

    if (!transferOK) {
        //        Display_printf(displayHandle, 0, 0, "SPI Write unsuccessful" );
    }
    else {
    //        Display_printf(displayHandle, 0, 0, "SPI write successful");
    //        Display_printf(displayHandle, 0, 0, "Transmitted: %d %d %d %d", transmitBuffer[0], transmitBuffer[1], transmitBuffer[2], transmitBuffer[3]);
    //        Display_printf(displayHandle, 0, 0, "Received: %d %d %d %d\n", receiveBuffer[0], receiveBuffer[1], receiveBuffer[2], receiveBuffer[3]);
    }
    transmitBuffer[0] = 0;
    transmitBuffer[1] = 0;
    transmitBuffer[2] = 0;
    transmitBuffer[3] = 0;
    transmitBuffer[4] = 0;
    transmitBuffer[5] = 0;

    receiveBuffer[0]  = 0;
    receiveBuffer[1]  = 0;
    receiveBuffer[2]  = 0;
    receiveBuffer[3]  = 0;
}

/*
 * @fn      mcp_bit_modify
 * @brief   SPI bit modify
 * @param   addr  - register address
 * @param   mask  - mask to modify bits
 * @param   value - new value
 * @return  none
 */
static void mcp_bit_modify (uint8_t addr, uint8_t mask, uint8_t *value) {
    transmitBuffer[0] = CMD_BIT_MODIFY; //SPI bit modify command
    transmitBuffer[1] = addr;
    transmitBuffer[2] = mask;
    transmitBuffer[3] = *value;
    spiTransaction.count = 4;

    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;
    spiParams.dataSize   = 8;

    GPIO_write(SLAVE_SELECT, 0);

    transferOK = SPI_transfer(spiHandle, &spiTransaction);

    GPIO_write(SLAVE_SELECT, 1);

    if (!transferOK) {
        //        Display_printf(displayHandle, 0, 0, "SPI bit modify unsuccessful" );
    }
    else {
    //        Display_printf(displayHandle, 0, 0, "SPI bit modify successful");
    //        Display_printf(displayHandle, 0, 0, "Transmitted: %d %d %d %d", transmitBuffer[0], transmitBuffer[1], transmitBuffer[2], transmitBuffer[3]);
    //        Display_printf(displayHandle, 0, 0, "Received: %d %d %d %d\n", receiveBuffer[0], receiveBuffer[1], receiveBuffer[2], receiveBuffer[3]);
    }
    transmitBuffer[0] = 0;
    transmitBuffer[1] = 0;
    transmitBuffer[2] = 0;
    transmitBuffer[3] = 0;
    receiveBuffer[0]  = 0;
    receiveBuffer[1]  = 0;
    receiveBuffer[2]  = 0;
    receiveBuffer[3]  = 0;
}

/*
 * @fn      mcp_read_rx
 * @brief   SPI READ RX Buffer
 * @param   rx_reg  - receive buffer
 * @param   buffer  - buffer for received data
 * @return  none
 */
static void mcp_read_rx (uint8_t rx_reg, uint8_t *buffer)
{
    transmitBuffer[0] = rx_reg;
    spiTransaction.count = 14; //CMD+ID(4)+DLC+data(8)

    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;
    spiParams.dataSize   = 8;

    GPIO_write(SLAVE_SELECT, 0);

    transferOK = SPI_transfer(spiHandle, &spiTransaction);

    GPIO_write(SLAVE_SELECT, 1);

    if (!transferOK) {
        //        Display_printf(displayHandle, 0, 0, "SPI Read RX unsuccessful" );
    }
    else {
    //        Display_printf(displayHandle, 0, 0, "SPI Read RX successful");
    //        Display_printf(displayHandle, 0, 0, "Transmitted: %d", transmitBuffer[0]);
    //        Display_printf(displayHandle, 0, 0, "mcp_read_rx Received: %d %d %d %d %d %d %c %c %c %c %c %c %c %c %c %c", receiveBuffer[0], receiveBuffer[1], receiveBuffer[2], receiveBuffer[3], receiveBuffer[4], receiveBuffer[5], receiveBuffer[6], receiveBuffer[7], receiveBuffer[8], receiveBuffer[9], receiveBuffer[10], receiveBuffer[11], receiveBuffer[12], receiveBuffer[13], receiveBuffer[14], receiveBuffer[15]);
    }

    *buffer = receiveBuffer[1];
    *(buffer+1) = receiveBuffer[2];
    *(buffer+2) = receiveBuffer[3];
    *(buffer+3) = receiveBuffer[4];
    *(buffer+4) = receiveBuffer[5];
    *(buffer+5) = receiveBuffer[6];
    *(buffer+6) = receiveBuffer[7];
    *(buffer+7) = receiveBuffer[8];
    *(buffer+8) = receiveBuffer[9];
    *(buffer+9) = receiveBuffer[10];
    *(buffer+10) = receiveBuffer[11];
    *(buffer+11) = receiveBuffer[12];
    *(buffer+12) = receiveBuffer[13];

    // reset
    transmitBuffer[0] = 0;
    transmitBuffer[1] = 0;
    transmitBuffer[2] = 0;
    transmitBuffer[3] = 0;
    receiveBuffer[0]  = 0;
    receiveBuffer[1]  = 0;
    receiveBuffer[2]  = 0;
    receiveBuffer[3]  = 0;
    receiveBuffer[4]  = 0;
    receiveBuffer[5]  = 0;
    receiveBuffer[6]  = 0;
    receiveBuffer[7]  = 0;
    receiveBuffer[8]  = 0;
    receiveBuffer[9]  = 0;
    receiveBuffer[10]  = 0;
    receiveBuffer[11]  = 0;
    receiveBuffer[12]  = 0;
    receiveBuffer[13]  = 0;
}

/*
 * @fn      mcp_load_tx
 * @brief   SPI LOAD TX Buffer
 * @param   tx_reg  - transmit buffer
 * @param   buffer  - buffer for data
 * @return  none
 */
static void mcp_load_tx (uint8_t tx_reg, uint8_t *buffer)
{
    transmitBuffer[0] = tx_reg;         //address
    transmitBuffer[1] = *buffer;        //sidh
    transmitBuffer[2] = *(buffer+1);    //sidl
    transmitBuffer[3] = *(buffer+2);    //eidh
    transmitBuffer[4] = *(buffer+3);    //eidl
    transmitBuffer[5] = *(buffer+4);    //dlc
    transmitBuffer[6] = *(buffer+5);    //data [0]
    transmitBuffer[7] = *(buffer+6);    //data [1]
    transmitBuffer[8] = *(buffer+7);
    transmitBuffer[9] = *(buffer+8);

    spiTransaction.count = 10; //CMD+ID(4)+DLC+data(4)--data+tag

    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;
    spiParams.dataSize   = 8;

    GPIO_write(SLAVE_SELECT, 0);

    transferOK = SPI_transfer(spiHandle, &spiTransaction);

    GPIO_write(SLAVE_SELECT, 1);

    if (!transferOK) {
        //        Display_printf(displayHandle, 0, 0, "SPI load TX unsuccessful" );
    }
    else {
        //        Display_printf(displayHandle, 0, 0, "SPI load TX successful");
        //        Display_printf(displayHandle, 0, 0, "Transmitted: %d %d %d %d %d %d %d %d %d %d", transmitBuffer[0], transmitBuffer[1], transmitBuffer[2], transmitBuffer[3], transmitBuffer[4], transmitBuffer[5], transmitBuffer[6], transmitBuffer[7], transmitBuffer[8], transmitBuffer[9]);
        //        Display_printf(displayHandle, 0, 0, "Received: %d\n", receiveBuffer[0]);
    }
    transmitBuffer[0] = 0;
    transmitBuffer[1] = 0;
    transmitBuffer[2] = 0;
    transmitBuffer[3] = 0;
    transmitBuffer[4] = 0;
    transmitBuffer[5] = 0;
    transmitBuffer[6] = 0;
    transmitBuffer[7] = 0;
    transmitBuffer[8] = 0;
    transmitBuffer[9] = 0;
    transmitBuffer[10] = 0;
    transmitBuffer[11] = 0;
    transmitBuffer[12] = 0;
    transmitBuffer[13] = 0;

    receiveBuffer[0]  = 0;
    receiveBuffer[1]  = 0;
    receiveBuffer[2]  = 0;
    receiveBuffer[3]  = 0;
}

/*
 * @fn      mcp_reset
 * @brief   SPI Reset command
 * @param   none
 * @return  0 - success
 */
int mcp_reset () {
    transmitBuffer[0] = CMD_RESET; //SPI reset command
    spiTransaction.count = 1;

    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;
    spiParams.dataSize   = 8;

    GPIO_write(SLAVE_SELECT, 0);

    transferOK = SPI_transfer(spiHandle, &spiTransaction);

    GPIO_write(SLAVE_SELECT, 1);

    if (!transferOK) {
        //        Display_printf(displayHandle, 0, 0, "SPI RESET unsuccessful" );
    }
    else {
    //        Display_printf(displayHandle, 0, 0, "SPI RESET successful");
    //        Display_printf(displayHandle, 0, 0, "Transmitted: %d %d %d %d", transmitBuffer[0], transmitBuffer[1], transmitBuffer[2], transmitBuffer[3]);
    //        Display_printf(displayHandle, 0, 0, "Received: %d %d %d %d\n", receiveBuffer[0], receiveBuffer[1], receiveBuffer[2], receiveBuffer[3]);
    }
    transmitBuffer[0] = 0;
    transmitBuffer[1] = 0;
    transmitBuffer[2] = 0;
    transmitBuffer[3] = 0;

    receiveBuffer[0]  = 0;
    receiveBuffer[1]  = 0;
    receiveBuffer[2]  = 0;
    receiveBuffer[3]  = 0;

    return 0;
}

/*
 * @fn      mcp_hw_rts_ctl
 * @brief   use RTS pin to initiate transmission
 * @param   none
 * @return  0 - success
 */
int mcp_hw_rts_ctl ( TXB_t line)
{
    transmitBuffer[0] = CMD_RTS | MCP_RTS( line );
    spiTransaction.count = 1;

    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;
    spiParams.dataSize   = 8;

    GPIO_write(SLAVE_SELECT, 0);

    transferOK = SPI_transfer(spiHandle, &spiTransaction);

    GPIO_write(SLAVE_SELECT, 1);

    if (!transferOK) {
        //        Display_printf(displayHandle, 0, 0, "SPI RTS unsuccessful" );
    }
    else {
    //        Display_printf(displayHandle, 0, 0, "SPI RTS successful");
    //        Display_printf(displayHandle, 0, 0, "Transmitted: %d", transmitBuffer[0]);
    //        Display_printf(displayHandle, 0, 0, "Received: %d\n", receiveBuffer[0]);
    }

    transmitBuffer[0] = 0;
    transmitBuffer[1] = 0;
    transmitBuffer[2] = 0;
    transmitBuffer[3] = 0;

    receiveBuffer[0]  = 0;
    receiveBuffer[1]  = 0;
    receiveBuffer[2]  = 0;
    receiveBuffer[3]  = 0;

    return 0;
}

/*
 * @fn      mcp_hw_status
 * @brief   SPI read status
 * quick poll of several status bits
 * @param   none
 * @return  status bits
 */
can_fstatus mcp_hw_status ( void) {

    can_fstatus res;

    transmitBuffer[0] = CMD_READ_STAT;

    spiTransaction.count = 1;
    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;
    spiParams.dataSize   = 8;

    GPIO_write(SLAVE_SELECT, 0);

    transferOK = SPI_transfer(spiHandle, &spiTransaction);

    GPIO_write(SLAVE_SELECT, 1);

    if (!transferOK) {
        //        Display_printf(displayHandle, 0, 0, "SPI Read hw Status unsuccessful" );
    }
    else {
        //        Display_printf(displayHandle, 0, 0, "SPI Read hw Status successful");
        //        Display_printf(displayHandle, 0, 0, "Transmitted: %d %d %d %d", transmitBuffer[0], transmitBuffer[1], transmitBuffer[2], transmitBuffer[3]);
        //        Display_printf(displayHandle, 0, 0, "Received: %d %d %d %d\n", receiveBuffer[0], receiveBuffer[1], receiveBuffer[2], receiveBuffer[3]);
    }

    res = (can_fstatus) receiveBuffer[1];

    transmitBuffer[0] = 0;
    transmitBuffer[1] = 0;
    transmitBuffer[2] = 0;
    transmitBuffer[3] = 0;

    receiveBuffer[0]  = 0;
    receiveBuffer[1]  = 0;
    receiveBuffer[2]  = 0;
    receiveBuffer[3]  = 0;

    return res;
}

/*
 * @fn      mcp_hw_rx_status
 * @brief   SPI read RX status
 * quick poll that indicates filter match and message type of received message
 * @param   none
 * @return  status bits
 */
void mcp_hw_rx_status (rx_fstatus *status) {

    transmitBuffer[0] = CMD_RX_STAT;
    transmitBuffer[1] = 0x00;
    spiTransaction.count = 2;

    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)receiveBuffer;
    spiParams.dataSize   = 8;

    GPIO_write(SLAVE_SELECT, 0);

    transferOK = SPI_transfer(spiHandle, &spiTransaction);

    GPIO_write(SLAVE_SELECT, 1);

    if (!transferOK) {
        //        Display_printf(displayHandle, 0, 0, "SPI read RX status unsuccessful" );
    }
    else {
        //        Display_printf(displayHandle, 0, 0, "SPI read RX status successful");
        //        Display_printf(displayHandle, 0, 0, "Transmitted: %d %d %d %d", transmitBuffer[0], transmitBuffer[1], transmitBuffer[2], transmitBuffer[3]);
        //        Display_printf(displayHandle, 0, 0, "Received: %d %d %d %d\n", receiveBuffer[0], receiveBuffer[1], receiveBuffer[2], receiveBuffer[3]);
    }

    uint8_t *temp_status;
    temp_status = status;
    *temp_status = receiveBuffer[1];

    transmitBuffer[0] = 0;
    transmitBuffer[1] = 0;
    transmitBuffer[2] = 0;
    transmitBuffer[3] = 0;

    receiveBuffer[0]  = 0;
    receiveBuffer[1]  = 0;
    receiveBuffer[2]  = 0;
    receiveBuffer[3]  = 0;
}

/*
 * @fn      mcp_hw_ctl_set
 * @brief   writes values to control registers
 * @param   value - value to be written
 * @return  0 success
 */
int mcp_hw_ctl_set (void *value) {
    if( *( ( uint8_t* )value + 1 ) != 0x30 ) // check if address at (*value+1) is TXB0CTRL
    {
        memcpy( ( void* )&reg, value, sizeof( reg_t ) );
        mcp_write ( reg.address, &reg.value, 3 );
    }
    else {
        memcpy( ( void* )&reg, value, sizeof( reg_t ) );
        mcp_write (CTL_TXB( reg.buffer, reg.address ), &reg.value, 3);
    }

    return 0;
}

/*
 * @fn      mcp_hw_ctl_update
 * @brief   Updates control, configuration and interrupt registers
 * @param   value - value to be written
 * @return  0 success
 */

int mcp_hw_ctl_update (void *value) {
    if( *( ( uint8_t* )value + 1 ) != 0x30 ) // check if address at (*value+1) is TXB0CTRL
    {
        memcpy( ( void* )&reg, value, sizeof( reg_t ) );
        mcp_bit_modify( reg.address, reg.mask, &reg.value);
    }
    else {
        memcpy( ( void* )&reg, value, sizeof( reg_t ) );
        mcp_bit_modify( CTL_TXB( reg.buffer, reg.address ), reg.mask, &reg.value );
    }

    return 0;
}

/*
 * @fn      mcp_hw_ctl_get
 * @brief   Reads control, configuration, interrupt and status registers
 * @param   result - value read
 * @return  0 success
 */
int mcp_hw_ctl_get ( void *result) {
    if (*( ( uint8_t* )result + 1 ) == 0x2C ) // check if CANINTF
    {
            memcpy(( void* )&reg, result, sizeof( reg_t ) );
            mcp_read( reg.address, &reg.value);
    }
    else if(*( ( uint8_t* )result + 1 ) == 0x30 ) //check if TXB0CTRL
    {
            memcpy( ( void* )&reg, result, sizeof( reg_t ) );
            mcp_read( CTL_TXB( reg.buffer, reg.address ), &reg.value);
    }
    else //stat_can
    {
        memcpy(( void* )&reg, result, sizeof( reg_t ) );
        reg.address = STAT_CAN;
        mcp_read( reg.address, &reg.value);
    }

    memcpy( result, &reg.value, 1 );

    return 0;
}

/*
 * @fn      mcp_hw_data_set
 * @brief   writes data to transmit buffer
 * @param   num - transmit buffer number
 * @param   tx_data - data to be transmitted
 * @return  0 success
 */
int mcp_hw_data_set ( TXB_t num, mcp_transfer *tx_data)
{
    mcp_load_tx ( CMD_LOAD_TX + MCP_TX( num ), ( uint8_t* )tx_data);
    return 0;
}

/*
 * @fn      mcp_hw_data_get
 * @brief   reads data from receive buffer
 * @param   num - receive buffer number
 * @param   rx_data - data received
 * @return  0 success
 */
int mcp_hw_data_get ( RXB_t num, mcp_transfer *rx_data)
{
   mcp_read_rx( CMD_READ_RX + MCP_RX( num ), (uint8_t*) rx_data);
   return 0;
}

/*
 * @fn      mcp_hw_filter_set
 * @brief   writes data to filter registers
 * @param   reg - receive buffer filter number
 * @param   filter - ID to filter by
 * @return  0 success
 */
int mcp_hw_filter_set ( RXF_t reg, mcp_id *filter)
{
    mcp_write( MCP_FILTER( reg ), (uint8_t*) filter, 6);

    return 0;
}

/*
 * @fn      mcp_hw_mask_set
 * @brief   Writes data to mask registers
 * @param   reg - receive buffer mask number
 * @param   mask - mask to set filter bits with
 * @return  0 success
 */
int mcp_hw_mask_set ( RXB_t reg, mcp_id *mask )
{
    mcp_write( MCP_MASK( reg ), ( uint8_t* ) mask, 6 );

    return 0;
}
