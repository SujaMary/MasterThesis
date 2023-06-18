/*
 * mcp_hw.h
 *
 *  Created on: 14-Apr-2022
 *      Author: sujaj
 */

#ifndef MCPLIBRARY_MCP_HW_H_
#define MCPLIBRARY_MCP_HW_H_
/******************************************************************************
* Includes
*******************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
/**
 * @macro MCP25625_RTS
 * @brief TRANSMIT BUFFER
 * Transmit registers - fast read.
 *  * @param ( TXB0 / TXB1 / TXB2 )
 */
#define MCP_TX( x )            ( (x) * 2 )

/**
 * @macro MCP25625_RX
 * @brief RECEIVE BUFFER
 * Receive registers - fast read.
 * @param ( RXB1 / RXB0 )
 */
#define MCP_RX( x )            ( (x) * 4 )

/**
 * @macro MCP25625_RTS
 * @brief PIN CONTROL COMMAND
 * RTS pin control
 * @param ( TXB0 / TXB1 / TXB2 )
 */
#define MCP_RTS( x )           ( 1 << (x) )

/**
 * @macro CTL_TXB
 * @brief TRANSMIT BUFFER CONTROL
 * TX control register adresses
 * @param ( TXB0 / TXB1 / TXB2 )
 */
#define CTL_TXB( x, y )        ( (x) * 10 + (y) )
/**
 * @macro CTL_RXB
 * @brief RECEIVE BUFFER CONTROL
 * RX control register addresses
 * @param ( RXB1 / RXB0 )
 */
#define CTL_RXB( x )           ( (x) * 10 + 0x60 )

/**
 * @macro MCP25625_MASK
 * @brief RX MASKS
 * Mask register addresses
 * @param ( RXM0 - Masks RXB0 /
 *          RXM1 - Masks RXB1 )
 */
#define MCP_MASK( x )          ( (x) * 4 + 0x20 )

/**
 * @macro RXF registers
 * @brief RX FILTERS
 * Filter register addresses
 * @param ( RXF0 - Filters RXB0 /
 *          RXF1 - Filters RXB0 /
 *          RXF2 - Filters RXB1 /
 *          RXF3 - Filters RXB1 /
 *          RXF4 - Filters RXB1 /
 *          RXF5 - Filters RXB1 )
 */
#define MCP_FILTER( x )        ( (x) < 3 ? (x) * 4 : (x) * 4 + 0x10 )

/******************************************************************************
* Configuration Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
/*
 * Receive buffer
 */
typedef enum {

    RXB0,
    RXB1

} RXB_t;

/*
 * Transmit buffers
 */
typedef enum {

    TXB0,
    TXB1,
    TXB2

} TXB_t;

/*
 * Receive filters
 */
typedef enum {

    RXF_0,
    RXF_1,
    RXF_2,
    RXF_3,
    RXF_4,
    RXF_5

}RXF_t;
/*
 * SPI instruction set
 */
typedef enum {

    CMD_RESET                   = 0xC0,
    CMD_READ                    = 0x03,
    CMD_READ_RX                 = 0x90,
    CMD_WRITE                   = 0x02,
    CMD_LOAD_TX                 = 0x40,
    CMD_RTS                     = 0x80,
    CMD_READ_STAT               = 0xA0,
    CMD_RX_STAT                 = 0xB0,
    CMD_BIT_MODIFY              = 0x05

} mcp_cmd;

/* @enum : mcp_ctl
 * control registers and their address
 */
typedef enum {

    CTL_RXP     = 0x0C,     //BFPCTRL
    CTL_RTS     = 0x0D,     //TXRTSCTRL
    CTL_CAN     = 0x0F,     //CANCTRL
    CTL_CNF3    = 0x28,     //CNF3
    CTL_CNF2    = 0x29,     //CNF2
    CTL_CNF1    = 0x2A,     //CNF1
    CTL_EFLG    = 0x2D,     //EFLG
    CTL_TXB     = 0x30,     //TXBnCTRL
    CTL_RXB0    = 0x60,     //RXB0CTRL
    CTL_RXB1    = 0x70      //RXB1CTRL

} mcp_ctl;

/*
 * * address of interrupt registers
 */
typedef enum {

    INT_CTL                     = 0x2B,
    INT_FLG                     = 0x2C

} mcp_int;

/*
 * Status registers and addresses
 */

typedef enum {

    STAT_CAN                    = 0x0E,
    STAT_TEC                    = 0x1C,
    STAT_REC                    = 0x1D

} mcp_stat;

/* @enum : can_opmode
 * operation modes of MCP
 */

typedef enum {

    OPMODE_NORMAL,
    OPMODE_SLEEP,
    OPMODE_LOOP,
    OPMODE_LISTEN,
    OPMODE_CONFIG

} can_opmode;

/*
 * Receive modes
 */
typedef enum {

    RX_MODE_ALL,
    RX_MODE_SID,
    RX_MODE_EID,
    RX_MODE_OFF

} rx_mode_t;
/*
 * Interrupt status
 */
typedef enum {

    INT_NO,
    INT_ERR,
    INT_WAKE,
    INT_TXB0,
    INT_TXB1,
    INT_TXB2,
    INT_RXB0,
    INT_RXB1

} int_status_t;
/*
 * Fast status
 */
typedef enum {

    FSTAT_RX0IF,
    FSTAT_RX1IF,
    FSTAT_TX0REQ,
    FSTAT_TX0IF,
    FSTAT_TX1REQ,
    FSTAT_TX1IF,
    FSTAT_TX2REQ,
    FSTAT_TX2IF

} can_fstatus;

/*
 * RX_STATUS instruction return
 */
typedef struct {

    RXF_t   filterhit       : 3;
    bool    exide           : 1;
    bool    rtr             : 1;
    uint8_t                 : 1;
    bool    rxb_0           : 1;
    bool    rxb_1           : 1;

} rx_fstatus;
/*
 * BFPCTRL register
 * RX pin control and status
 */
typedef enum {

    RXP_P0_MODE     = 0x01,
    RXP_P1_MODE     = 0x02,
    RXP_P0_ENAB     = 0x04,
    RXP_P1_ENAB     = 0x08,
    RXP_P0_STAT     = 0x10,
    RXP_P1_STAT     = 0x20

} rxp_ctl_mask;

typedef struct {

    bool    p0_mode : 1;
    bool    p1_mode : 1;
    bool    p0_enab : 1;
    bool    p1_enab : 1;
    bool    p0_stat : 1;
    bool    p1_stat : 1;
    uint8_t         : 2;

    mcp_ctl         reg;
    rxp_ctl_mask    mask;

} mcp_rxp_ctl;

/*
 * enum for TXRTSCTRL
 */
typedef enum {

    RTS_P0_MODE     = 0x01, //TX0RTS pin mode bit
    RTS_P1_MODE     = 0x02, //TX1RTS pin mode bit
    RTS_P2_MODE     = 0x04, //TX2RTS pin mode bit
    RTS_P0_STAT     = 0x08, //TX0RTS pin state bit
    RTS_P1_STAT     = 0x10, //TX1RTS pin state bit
    RTS_P2_STAT     = 0x20  //TX2RTS pin state bit

} rts_ctl_mask;

typedef struct {

    bool        p0_mode : 1;
    bool        p1_mode : 1;
    bool        p2_mode : 1;
    bool        p0_stat : 1;
    bool        p1_stat : 1;
    bool        p2_stat : 1;
    uint8_t             : 2;

    mcp_ctl         reg;
    rts_ctl_mask    mask;

} mcp_rts_ctl;

/* @enum : can_ctl_mask
 * value for CANCTRL reg
 */
typedef enum {

    CAN_CLKPRE                  = 0x03,//0000 0011
    CAN_CLKOUT                  = 0x04,//0000 0100
    CAN_OSM                     = 0x08,//0000 1000
    CAN_ABAT                    = 0x10,//0001 0000
    CAN_REQOP                   = 0xE0 //1110 0000

} can_ctl_mask;

/* can control setting
 * CANCTRL register
 */

typedef struct {
    uint8_t     clkpre        : 2;
    bool        clken         : 1;
    bool        osm           : 1;
    bool        abat          : 1;
    can_opmode  reqop         : 3;

    mcp_ctl             reg;
    can_ctl_mask        mask;

} mcp_can_ctl;

/*
 * Configuration 3 Settings
 */
typedef enum {

    CNF3_PHSEG2                 = 0x07,
    CNF3_WAKFIL                 = 0x40,
    CNF3_SOFR                   = 0x80

} cnf3_ctl_mask;

typedef struct {

    uint8_t     phseg2          : 3;
    uint8_t                     : 3;
    bool        wakfil          : 1;
    bool        sof             : 1;

    mcp_ctl             reg;
    cnf3_ctl_mask       mask;

} mcp_cnf3_ctl;

/*
 * Configuration 2 Settings
 */
typedef enum {

    CNF2_PRSEG                   = 0x07,
    CNF2_PHSEG1                  = 0x38,
    CNF2_SAM                     = 0x40,
    CNF2_BTLMODE                 = 0x80

} cnf2_ctl_mask;

typedef struct {

    uint8_t     prseg           : 3;
    uint8_t     phseg1          : 3;
    bool        sam             : 1;
    bool        btlmode         : 1;

    mcp_ctl        reg;
    cnf2_ctl_mask       mask;

} mcp_cnf2_ctl;

/*
 * Configuration 1 Settings
 */
typedef enum {

    CNF1_BRP                    = 0x3F,
    CNF1_SJW                    = 0xC0

} cnf1_ctl_mask;

typedef struct {

    uint8_t     brp             : 6;
    uint8_t     sjw             : 2;

    mcp_ctl             reg;
    cnf1_ctl_mask       mask;

} mcp_cnf1_ctl;

/*
 * CANINTE and CANINTF
 */
typedef enum {

    CAN_RX0IE                   = 0x01,
    CAN_RX1IE                   = 0x02,
    CAN_TX0IE                   = 0x04,
    CAN_TX1IE                   = 0x08,
    CAN_TX2IE                   = 0x10,
    CAN_ERRIE                   = 0x20,
    CAN_WAKIE                   = 0x40,
    CAN_MERRE                   = 0x80

} int_ctl_mask;

typedef struct {

    uint8_t     rx0             : 1;
    uint8_t     rx1             : 1;
    uint8_t     tx0             : 1;
    uint8_t     tx1             : 1;
    uint8_t     tx2             : 1;
    uint8_t     err             : 1;
    uint8_t     wak             : 1;
    uint8_t     merre           : 1;

    mcp_int                 reg;
    int_ctl_mask            mask;

} mcp_int_ctl;

/*
 * Error Flags
 */
typedef enum {

    FLG_EWARN                   = 0x01,
    FLG_RXWAR                   = 0x02,
    FLG_TXWAR                   = 0x04,
    FLG_RXEP                    = 0x08,
    FLG_TXEP                    = 0x10,
    FLG_TXBO                    = 0x20,
    FLG_RX0OVR                  = 0x40,
    FLG_RX1OVR                  = 0x80

} eflg_ctl_mask;

typedef struct {

    bool        ewarn           : 1;
    bool        rxwar           : 1;
    bool        txwar           : 1;
    bool        rxerr           : 1;
    bool        txerr           : 1;
    bool        txbo            : 1;
    bool        rx0ovr          : 1;
    bool        rx1ovr          : 1;

    mcp_ctl             reg;
    eflg_ctl_mask       mask;

} mcp_eflg_ctl;

/*
 * TXBnCTRL
 */
typedef enum {
    TXB_TXP     = 0x03,
    TXB_TXREQ   = 0x08,
    TXB_TXERR   = 0x10,
    TXB_MLOA    = 0x20,
    TXB_ABTF    = 0x40

} txb_ctl_mask;

typedef struct {
    uint8_t     txp     : 2;
    uint8_t             : 1;
    bool        txreq   : 1;
    bool        txerr   : 1;
    bool        mloa    : 1;
    uint8_t     abtf    : 1;
    uint8_t             : 1;

    mcp_ctl         reg;
    txb_ctl_mask    mask;
    TXB_t           buffer;

} mcp_txb_ctl;

/*
 * RXB0CTRL
 */
typedef enum {

    RX0_FILHIT0 = 0x01,
    RX0_BUKT1   = 0x02,
    RX0_BUKT    = 0x04,
    RX0_RTR     = 0x08,
    RX0_RXM     = 0x60

} rxb0_ctl_mask;

typedef struct {
    bool        filhit0    : 1;
    bool        bukt_1     : 1;
    bool        bukt       : 1;
    bool        rxrtr      : 1;
    uint8_t                : 1;
    rx_mode_t   rxm        : 3;

    mcp_ctl         reg;
    rxb0_ctl_mask   mask;

} mcp_rxb0_ctl;

/*
 * RXB1CTRL
 */
typedef enum {

    RX1_FILHIT                  = 0x07,
    RX1_RTR                     = 0x08,
    RX1_RXM                     = 0x60

} rxb1_ctl_mask;

typedef struct {

    uint8_t     filhit         : 3;
    bool        rxrtr          : 1;
    uint8_t                    : 1;
    rx_mode_t   rxm            : 3;

    mcp_ctl             reg;
    rxb1_ctl_mask       mask;

} mcp_rxb1_ctl;

/*
 * CAN status
 */
typedef struct {

    uint8_t                     : 1;
    int_status_t    icod        : 3;
    uint8_t                     : 1;
    can_opmode      opmod       : 3;

} mcp_can_stat;

/*
 * MCP id
 */
typedef struct {

    uint8_t     eidt            : 2;
    uint8_t                     : 1;
    uint8_t     ide             : 1;
    uint8_t     srr             : 1;
    uint8_t     sidl            : 3;

} mcp_sidl;

typedef struct {

    uint8_t             sidh;
    mcp_sidl            sidl;
    uint8_t             eidh;
    uint8_t             eidl;

} mcp_id;

//RXBnDLC
typedef struct {

    uint8_t     len             : 4;
    uint8_t                     : 2;
    uint8_t     rtr             : 1;
    uint8_t                     : 1;

} mcp_dlc;

typedef struct {

    mcp_dlc        dlc;
    uint8_t        buff[ 6 ];
} mcp_data;

typedef struct {

    mcp_id         id;
    mcp_data       payload;

} mcp_transfer;

/******************************************************************************
* Variables
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/*
 * Software Reset
 */
int mcp_reset ();

/*
 * RTS Pin Control
 */
int mcp_hw_rts_ctl ( TXB_t line);

/*
 * Status
 */
can_fstatus mcp_hw_status ( void);

/*
 * Read RX Status
 */
void mcp_hw_rx_status (rx_fstatus *status);

/*
 * Write Register
 */
int mcp_hw_ctl_set (void *value);

/*
 * Update Register
 */
int mcp_hw_ctl_update (void *value);

/*
 * Reads control, configuration, interrupt and status registers.
 */
int mcp_hw_ctl_get ( void *result);

/*
 * Writes group of data registers for specified TX buffer.
 */
int mcp_hw_data_set ( TXB_t num, mcp_transfer *tx_data);

/*
 * Reads group of data registers for specified RX buffer
 */
int mcp_hw_data_get ( RXB_t num, mcp_transfer *rx_data);

/*
 * Writes data to filter registers.
 */
int mcp_hw_filter_set ( RXF_t reg, mcp_id *filter);

/*
 * Setup Mask - Writes data to mask registers.
 */
int mcp_hw_mask_set ( RXB_t reg, mcp_id *mask );


#endif /* MCPLIBRARY_MCP_HW_H_ */
