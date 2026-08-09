#include "F28x_Project.h"


// Initialize Serial (actually SCIA)
Uint32 Config_SCI(void){

	Uint32 baudrate = 9600;
	Uint32 bitsNumber = 8;
	Uint32 parityType = SERIAL_PARITY_NONE;
	Uint32 us_frame_sci=0;  // Tempo em us do frame da SCI
	Uint32 N_bits=2; // No Mínimo Star e Stop bits

	switch(bitsNumber) {
		case 8:
			ScibRegs.SCICCR.bit.SCICHAR = 0x7;
			N_bits+=8;
			break;
		case 7:
			ScibRegs.SCICCR.bit.SCICHAR = 0x6;
			N_bits+=7;
			break;
		default:
			ScibRegs.SCICCR.bit.SCICHAR = 0x7;
			N_bits+=8;
	}

	// Parity settings
	switch(parityType){
		case SERIAL_PARITY_EVEN:
			ScibRegs.SCICCR.bit.PARITYENA = 1;
			ScibRegs.SCICCR.bit.PARITY = 1;
			N_bits+=1;
			break;
		case SERIAL_PARITY_ODD:
			ScibRegs.SCICCR.bit.PARITYENA = 1;
			ScibRegs.SCICCR.bit.PARITY = 0;
			N_bits+=1;
			break;
		case SERIAL_PARITY_NONE:
			ScibRegs.SCICCR.bit.PARITYENA = 0;
			break;
		default:
			ScibRegs.SCICCR.bit.PARITYENA = 0;
	}


	us_frame_sci = (N_bits * 1E6) / baudrate;  // Periodo de um frame da SCI (Start e stop bit, data bits e paridade se houver)

	// Baud rate settings - Automatic
	baudrate = (Uint32) (50E6 / (baudrate*8) - 1);

	// Configure the High and Low baud rate registers
	ScibRegs.SCIHBAUD.all = (baudrate & 0xFF00) >> 8;
	ScibRegs.SCILBAUD.all = (baudrate & 0x00FF);

	// Enables TX and RX Interrupts
	ScibRegs.SCICTL2.bit.TXINTENA = 0;
	ScibRegs.SCIFFTX.bit.TXFFIENA = 0;
	ScibRegs.SCICTL2.bit.RXBKINTENA = 1;
	ScibRegs.SCIFFRX.bit.RXFFIENA = 0;

	// FIFO TX configurations
	ScibRegs.SCIFFTX.bit.TXFFIL = 1;	// Interrupt level
	ScibRegs.SCIFFTX.bit.SCIFFENA = 1;	// Enables FIFO
	ScibRegs.SCIFFTX.bit.TXFFINTCLR = 1;	// Clear interrupt flag

	// FIFO: RX configurations
	ScibRegs.SCIFFRX.bit.RXFFIL = 4;	// Interrupt level
	ScibRegs.SCIFFRX.bit.RXFFIENA = 1;
	ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;	// Clear interrupt flag
	ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;	// Clear overflow flag

	// FIFO: Control configurations
	ScibRegs.SCIFFCT.all=0x00;

	// Enable RX and TX and reset the serial
	ScibRegs.SCICTL1.bit.RXENA	 = 1;
	ScibRegs.SCICTL1.bit.TXENA	 = 1;
	ScibRegs.SCICTL1.bit.SWRESET = 1;

	// FIFO: Reset
	ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;
	ScibRegs.SCIFFTX.bit.TXFIFORESET = 1;
	ScibRegs.SCIFFTX.bit.SCIRST = 0;
	ScibRegs.SCIFFTX.bit.SCIRST = 1;

	return us_frame_sci;

}

// Transmit variable data based on passed size
void serial_transmitData(Uint16 * data, Uint16 size){
	static Uint16 i = 0;

	ScibRegs.SCIFFTX.bit.TXFIFORESET=0;  // Limpa a FIFO Tx para o novo envio;
	ScibRegs.SCIFFTX.bit.TXFIFORESET=1;

	for (i = 0; i < size; i++){
		ScibRegs.SCITXBUF.all= 0x00FF & data[i];
	}

	// If you want to wait until the TX buffer is empty, uncomment line below
//	while (ScibRegs.SCICTL2.bit.TXEMPTY != true) ;
}

