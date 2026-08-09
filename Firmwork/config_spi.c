//###########################################################################
//
// FILE:   F2837xD_Spi.c
//
// TITLE:  F2837xD SPI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2837xD Support Library v210 $
// $Release Date: Tue Nov  1 14:46:15 CDT 2016 $
// $Copyright: Copyright (C) 2013-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

//
// Included Files
//
#include "F28x_Project.h"

//
// Calculate BRR: 7-bit baud rate register value
// SPI CLK freq = 500 kHz
// LSPCLK freq  = CPU freq / 4  (by default)
// BRR          = (LSPCLK freq / SPI CLK freq) - 1
//
#if CPU_FRQ_200MHZ
#define SPI_BRR        ((200E6 / 4) / 500E3) - 1
#endif

/*
#if CPU_FRQ_150MHZ
#define SPI_BRR        ((150E6 / 4) / 500E3) - 1
#endif

#if CPU_FRQ_120MHZ
#define SPI_BRR        ((120E6 / 4) / 500E3) - 1
#endif
*/

//
// InitSPI - This function initializes the SPI to a known state
//
void Config_SPI(void){

    //
    // Initialize SPI FIFO registers
    //
    //SpicRegs.SPIFFTX.all = 0xC022;    // Enable FIFOs, set TX FIFO level to 4
    SpicRegs.SPIFFTX.bit.TXFFIL = 0;                         // 4:0    Interrupt level
    SpicRegs.SPIFFTX.bit.TXFFIENA = 0;                       // 5      Interrupt enable0
    SpicRegs.SPIFFTX.bit.TXFFINTCLR = 1;                     // 6      Clear INT flag
    SpicRegs.SPIFFTX.bit.TXFFINT = 0;                        // 7      INT flag
    SpicRegs.SPIFFTX.bit.TXFFST = 0;                         // 12:8   FIFO status
    SpicRegs.SPIFFTX.bit.TXFIFO = 1;                         // 13     FIFO reset
    SpicRegs.SPIFFTX.bit.SPIFFENA = 1;                       // 14     SPI FIFO Enhancements Enable
    SpicRegs.SPIFFTX.bit.SPIRST = 0;                         // 15     SPI Reset


    //SpicRegs.SPIFFRX.all = 0x0022;    // Set RX FIFO level to 4
    SpicRegs.SPIFFRX.bit.RXFFIL = 16;                      // 4:0    Interrupt level
    SpicRegs.SPIFFRX.bit.RXFFIENA = 1;                        // 5      Interrupt enable0
    SpicRegs.SPIFFRX.bit.RXFFINTCLR = 0;                     // 6      Clear INT flag
    SpicRegs.SPIFFRX.bit.RXFFINT = 0;                        // 7      INT flag
    SpicRegs.SPIFFRX.bit.RXFFST = 0;                         // 12:8   FIFO status
    SpicRegs.SPIFFRX.bit.RXFIFORESET = 0;                    // 13     FIFO reset
    SpicRegs.SPIFFRX.bit.RXFFOVFCLR = 0;                     // 14     Clear overflow
    SpicRegs.SPIFFRX.bit.RXFFOVF = 0;                        // 15     FIFO overflow

    SpicRegs.SPIFFCT.all = 0x00;

    SpicRegs.SPIFFTX.bit.SPIRST = 1;

    SpicRegs.SPIFFRX.bit.RXFIFORESET=1;

    //
    // Initialize core SPI registers
    //

    // Initialize SPI-C

    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 16-bit character
    // Enable loop-back
    SpicRegs.SPICCR.bit.SPISWRESET = 0;
    SpicRegs.SPICCR.bit.CLKPOLARITY = 1;
    SpicRegs.SPICCR.bit.SPICHAR = (16-1);  // Troquei aqui

    SpicRegs.SPICCR.bit.SPILBK = 0;  // <-----Troquei aqui o Loopback mode

    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpicRegs.SPICTL.bit.MASTER_SLAVE = 0; // <-----Troquei aqui o MASTER_SLAVE
    SpicRegs.SPICTL.bit.TALK = 1;
    SpicRegs.SPICTL.bit.CLK_PHASE = 0;
    SpicRegs.SPICTL.bit.SPIINTENA = 1;

    // Set the baud rate
    SpicRegs.SPIBRR.bit.SPI_BIT_RATE = SPI_BRR;

    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    SpicRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset
    SpicRegs.SPICCR.bit.SPISWRESET = 1;
}


//
// End of file
//
