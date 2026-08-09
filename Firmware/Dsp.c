#include "F28x_Project.h"
#include "F2837xD_Ipc_drivers.h"
#include "math.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_can.h"
#include "driverlib/can.h"

#define NBYTES_INVFV 4

#define ID_ON_OFF_INV_FV 1
#define ID_POT_INV_FV_REF 2
#define ID_POT_INV_FV_MED 3
//#define ID_TENSAO_PAINEIS 4


//################ DEFINIÇÕES E VARIAVEIS DO GMG ###############################

#define IRMS_MAX (200.0f) // (+/-)
#define VRMS_MAX (400.0f) // (+/-)
#define PQ_MAX (65535.0f) // (+/-)

#define GANHOI (327.0f)  // floor(65535/(IRMS_MAX))
#define GANHOI_inv (1.0/GANHOI)

#define GANHOV (163.0f)  // floor(65535/(VRMS_MAX))
#define GANHOV_inv (1.0/GANHOV)

#define GANHOPQ (0.468f)  // (65535/(2*PQ_MAX))  // Arredondado para 3 casas decimais
#define GANHOPQ_inv (1.0/GANHOPQ)

#define GANHOF (100.0f) // Multiplicador para a frequência (para enviar como inteiro)
#define GANHOF_inv (1.0/GANHOF)

#define OFFSETPQ (int32)(32766)


volatile Variaveis_Externas var_Ext={0};

//volatile Variaveis_CAN_GMG GMG = {0};
Variaveis_CAN_GMG_int GMG_int = {0};

Variaveis_Contatora Contatora_CargaR = {0}, Contatora_CargaR_ant = {0};

Variaveis_Controle_Pot_GMG var_GMG = Default_INIT_pot_GMG;

// Variáveis da CAN -> GMG
volatile uint32_t status;
volatile uint32_t errorFlag = 0;

extern unsigned char Rx_Mensagem1[6];
extern unsigned char Rx_Mensagem2[6];
extern unsigned char Rx_Mensagem3[8];
extern unsigned char Tx_Mensagem_CargaR;

extern tCANMsgObject RxObj_Mensagem1;
extern tCANMsgObject RxObj_Mensagem2;
extern tCANMsgObject RxObj_Mensagem3;
extern tCANMsgObject TxObj_CargaR;

//*************** FIM DAS DEFINIÇÕES E VARIAVEIS DO GMG ********************************


//################ DEFINIÇÕES E VARIAVEIS DO SIC ###############################
float Va_RMS_sic = 0, Vb_RMS_sic = 0, Vc_RMS_sic = 0;
Uint16 Va_RMS_sic_int = 0, Vb_RMS_sic_int = 0, Vc_RMS_sic_int = 0;

float Ia_RMS_sic = 0, Ib_RMS_sic = 0, Ic_RMS_sic = 0;
Uint16 Ia_RMS_sic_int = 0, Ib_RMS_sic_int = 0, Ic_RMS_sic_int = 0;

float Freq_sic = 60;
Uint16 Freq_sic_int = 0;

float P_sic = 0, Q_sic = 0, S_sic = 0;
Uint16 P_sic_int = 0, Q_sic_int = 0, S_sic_int = 0;

float Vae_sic = 0, Vcc_sic = 0;
Uint16 Vae_sic_int = 0, Vcc_sic_int = 0;

float P_boost = 0, P_crowbar = 0;
Uint16 P_boost_int = 0, P_crowbar_int = 0;

//*************** FIM DEFINIÇÕES E VARIAVEIS DO SIC ********************************


//################ DEFINIÇÕES E VARIAVEIS DO INV_FV ###############################
volatile Uint16 Liga_Inversor_FV = 0;
volatile Uint16 kid = 0;
const Uint16 ID_vec[4]={1,2,3,2};

//*************** FIM DAS DEFINIÇÕES E VARIAVEIS DO INV_FV ********************************


//##### INICIO Variáveis SPI ##########################################
Uint16 rdata_spi[N_VAR_PACOTE_spi]={0};  // Receive data buffer da SPI - Primeira Word define o pacote, o resto é zero
Uint16 Pacotes_spi[N_PACOTES_spi][N_VAR_PACOTE_spi]={0};     // Pacote de Dados a serem enviados
Uint16 seleciona_pacote_spi=0;
Uint16 ir_spi=0, is_spi=0;
//***** FIM Variáveis SPI *********************************************


//##### INICIO Variáveis SCI ##########################################
Uint16 Pacote_send_InvFV[NBYTES_INVFV]={0,0,0,'\n'};     // Pacote de Dados a serem enviados
Uint16 Pacote_recebido_InvFV[NBYTES_INVFV]={0,0,0,'\n'}; // Pacote de Dados a serem recebidos
Uint16 DataS=1000;
Uint16 bytesRetardatarios_sci=0, bytesRetardatarios_sci_ant=0;
Uint16 Tipo_OP_sci=0, ID_sci=0;

unsigned char Enable_FV_transmit=1;
Uint32 us_frame_sci=0;

//***** FIM Variáveis SCI *********************************************

Uint16 toggle_LED=0;

volatile float RecBuffer_CPU1_to_CPU2[NfloatsCPU1toCPU2];
volatile float SendBuffer_CPU2_to_CPU1[NfloatsCPU2toCPU1];

float pot_FV_ref_CPU1 = 0.0;
Uint32 Enable_Control_PotMinGMG_CPU1=0;

__interrupt void cpu_timer1_isr(void);
__interrupt void cpu_timer2_isr(void);

// Function Prototypes of SPI
__interrupt void spiRxFifoIsr(void);   // Comunicação Raspberry
__interrupt void scibRxFifoIsr(void);  // Comunicação com o Inversor FV - Growatt
__interrupt void canbISR(void);        // Comunicação com o Wattimetro

void Controle_PotMin_GMG(Variaveis_Controle_Pot_GMG *GMG);

// Main
void main(void)
{
    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2837xD_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Initialize GPIO:
    // This example function is found in the F2837xD_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    // Initialize GPIO
    //EALLOW;
    //GPIO_WritePin(PIN_ENABLE_INV   , Enable_INV   );
    //EDIS;

    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the F2837xD_PieCtrl.c file.
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xD_DefaultIsr.c.
    // This function is found in F2837xD_PieVect.c.
    //
    InitPieVectTable();

    InitIpc();

    WaitForIpcFlag(IPC_FLAG10);
    AckIpcFlag(IPC_FLAG10);

    Config_CAN();     // Configura a CAN
    Config_SPI();     // Configura a SPI

    us_frame_sci = Config_SCI(); // Configura a SCI

    InitCpuTimers();   // For this example, only initialize the Cpu Timers
    ConfigCpuTimer(&CpuTimer1, 200, 100E3); // em microssegundos  usado para a comunicação com a RPI - Atualiza os buffers de envio;
    ConfigCpuTimer(&CpuTimer2, 200, (float)(2*NBYTES_INVFV*us_frame_sci)); // em microssegundos  usado para a comunicação SCI;

    //
    EALLOW;
    // Interrupções da Serial
    PieVectTable.SCIB_RX_INT = &scibRxFifoIsr;
    //PieVectTable.SCIB_TX_INT = &scibTxFifoIsr;ç

    // Interrupções da SPI
    PieVectTable.SPIC_RX_INT = &spiRxFifoIsr;
    //PieVectTable.SPIC_TX_INT = &spiTxFifoIsr;

    // Interrupções da SCI
    PieVectTable.CANB0_INT = &canbISR;

    // Interrupções do Timer1
    PieVectTable.TIMER1_INT = &cpu_timer1_isr;

    // Interrupções do Timer2
    PieVectTable.TIMER2_INT = &cpu_timer2_isr;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block

    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;   // PIE Group 9, INT1 // SCIB_RX
    //PieCtrlRegs.PIEIER9.bit.INTx4 = 1;   // PIE Group 9, INT2 // SCIB_TX

    PieCtrlRegs.PIEIER6.bit.INTx9 = 1;     // Enable PIE Group 6,  // SPIC_RX
    //PieCtrlRegs.PIEIER6.bit.INTx10 = 1;     // Enable PIE Group 6, // SPIC_TX

    PieCtrlRegs.PIEIER9.bit.INTx7 = 1;   // Interrupção da CAN

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;  //Enable TINT0 in the PIE: Group 1 interrupt 7

    IER|= M_INT6;                              // Enable SPIC_RX
    IER|= M_INT9;                              // Enable SCIB_RX e CAN_B
    IER|= M_INT13;                            // Enable CPU Timer1
    IER|= M_INT14;                            // Enable CPU Timer2

    EDIS;

    //InitIpc();  /// Faz mais sentido ser antes de WaitForIpcFlag(IPC_FLAG15);

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

    while(1){

        WaitForIpcFlag(IPC_FLAG10);

        RecvIpcData(&RecBuffer_CPU1_to_CPU2, NwordsCPU1toCPU2); // Segundo argumento = Nº de variáveis words = Nº de variáveis float x 2

        Contatora_CargaR.all = (unsigned char)(RecBuffer_CPU1_to_CPU2[0]);
        Liga_Inversor_FV = (Uint16)(RecBuffer_CPU1_to_CPU2[1]);
        var_GMG.pot_ref  = RecBuffer_CPU1_to_CPU2[2];
        pot_FV_ref_CPU1  = RecBuffer_CPU1_to_CPU2[3];
        var_GMG.Enable   = RecBuffer_CPU1_to_CPU2[4];

        AckIpcFlag(IPC_FLAG10);

        var_Ext.Pot_ref_GMG = var_GMG.pot_ref;

        SendIpcData((float*)&var_Ext, NwordsCPU2toCPU1 , IPC_FLAG11);

    }
}

//
// scibRxFifoIsr - SCIB Receive FIFO ISR
//
__interrupt void scibRxFifoIsr(void){

    Uint16 i;

    for (i = 0; i < NBYTES_INVFV; i++) {
        Pacote_recebido_InvFV[i] = 0x00FF & ScibRegs.SCIRXBUF.all;
    }

    Uint16 tipo_op_rec = Pacote_recebido_InvFV[0] & 0x0F;
    Uint16 id_rec = (Pacote_recebido_InvFV[0] & 0xF0) >> 4;
    Uint16 dado_rec = (Pacote_recebido_InvFV[1] << 8) | Pacote_recebido_InvFV[2];

    if (tipo_op_rec == 3) { // RETORNO
        if (id_rec == ID_ON_OFF_INV_FV) {
            var_Ext.InvFV.Liga_Inversor_FV_rec = dado_rec;  // Confirmação ON/OFF
        }
        else if (id_rec == ID_POT_INV_FV_MED) {
            var_Ext.InvFV.Potencia_Med_FV = ((float)dado_rec);
        }
    }

    CpuTimer2Regs.TCR.bit.TRB = 1;  // reload Timer 0
    CpuTimer2Regs.TCR.bit.TSS = 0;  // start/restart Timer 0

    ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
    ScibRegs.SCIFFRX.bit.RXFFINTCLR = 1;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
}

//
// cpu_timer2_isr CPU Timer2 ISR
//
__interrupt void cpu_timer2_isr(void){
   CpuTimer2.InterruptCount++;

   bytesRetardatarios_sci_ant = bytesRetardatarios_sci;
   bytesRetardatarios_sci = ScibRegs.SCIFFRX.bit.RXFFST;

   // Caso bytesRetardatarios_sci==0, não precisa resetar, a FIFO já está vazia
   // Caso bytesRetardatarios_sci > 0, só reseta a FIFO caso a FIFO fique dois períodos consecutivos com o mesmo nível da FIFO (indicando pacote parcial)

   if(bytesRetardatarios_sci>0 && (bytesRetardatarios_sci == bytesRetardatarios_sci_ant)){
       // Reseta a FIFO da SCI depois de um tempo para não perder sincronia dos pacotes
       ScibRegs.SCIFFRX.bit.RXFIFORESET=0;
       ScibRegs.SCIFFRX.bit.RXFIFORESET=1;
   }
}

//
// spiRxFifoIsr - ISR for SPI receive FIFO
//
__interrupt void spiRxFifoIsr(void){

    //Como chegou um pacote de dados o Timer 2 resetou;
    //CpuTimer0Regs.TCR.bit.TRB = 1;  // reload Timer 0
    //CpuTimer0Regs.TCR.bit.TSS = 0;  // start/restart Timer 0

    SpicRegs.SPIFFTX.bit.TXFIFO=0;
    SpicRegs.SPIFFTX.bit.TXFIFO=1;

    for(ir_spi=0;ir_spi<N_VAR_PACOTE_spi;ir_spi++){
        rdata_spi[ir_spi] = (0x00FF & SpicRegs.SPIRXBUF);     // Read data
    }

    seleciona_pacote_spi = rdata_spi[0];

    for(is_spi=0;is_spi<N_VAR_PACOTE_spi;is_spi++){
        SpicRegs.SPITXBUF = (0xFFFF & Pacotes_spi[seleciona_pacote_spi][is_spi]);      // Send data
    }

    SpicRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
    SpicRegs.SPIFFRX.bit.RXFFINTCLR=1;  // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all|=PIEACK_GROUP6;       // Issue PIE ack
}


//
// cpu_timer1_isr CPU Timer2 ISR
//
__interrupt void cpu_timer1_isr(void){

    CpuTimer1.InterruptCount++;

    var_GMG.pot_med = var_Ext.GMG.P_Ativa;
	
	Controle_PotMin_GMG(&var_GMG);

	var_Ext.InvFV.Potencia_Ref_FV = var_GMG.pot_FV_ref;

    //  ID_vec[4]={1,2,3,2};

    if(ID_vec[kid] == ID_ON_OFF_INV_FV){ // ID 1 - ON/OFF inversor (escrita)

        if(Liga_Inversor_FV == 1 && var_Ext.InvFV.Liga_Inversor_FV_rec == 0){
            ID_sci = ID_ON_OFF_INV_FV;
            Tipo_OP_sci = 2; // 2 = Escrita
            DataS = Liga_Inversor_FV;
        }else if(Liga_Inversor_FV == 0 && var_Ext.InvFV.Liga_Inversor_FV_rec == 1){
                ID_sci = ID_ON_OFF_INV_FV;
                Tipo_OP_sci = 2; // 2 = Escrita
                DataS = Liga_Inversor_FV;
        }else{
            kid++;
        }
    }

    if(ID_vec[kid] == ID_POT_INV_FV_REF){ // Potência de referência (escrita)
        ID_sci = ID_POT_INV_FV_REF;
        Tipo_OP_sci = 2; // 2 = Escrita
        DataS = (Uint16)var_Ext.InvFV.Potencia_Ref_FV;
    }

    if(ID_vec[kid] == ID_POT_INV_FV_MED){
        ID_sci = ID_POT_INV_FV_MED;
        Tipo_OP_sci = 1; // 1 = Leitura
        DataS = 0;
    }


    if (Enable_FV_transmit) {
        Pacote_send_InvFV[0] = ((ID_sci & 0x0F) << 4) | (Tipo_OP_sci & 0x0F);
        Pacote_send_InvFV[1] = (0xFF00 & DataS) >> 8;
        Pacote_send_InvFV[2] = (0x00FF & DataS);
        Pacote_send_InvFV[3] = '\n';

        serial_transmitData(Pacote_send_InvFV, NBYTES_INVFV);
    }

    kid++;
    if(kid >= 4){
        kid = 0;
    }

    // Pisca LED
    toggle_LED = ~toggle_LED;
    GPIO_WritePin(PIN_LED2, toggle_LED ? 0 : 1);

    // Geração de valores simulados para para o SIC
    /*Va_RMS_sic = 230.0 + (rand() % 2000 - 1000) * 0.001;
    Vb_RMS_sic = 228.0 + (rand() % 2000 - 1000) * 0.001;
    Vc_RMS_sic = 232.0 + (rand() % 2000 - 1000) * 0.001;
    Ia_RMS_sic = 45.0 + (rand() % 2000 - 1000) * 0.001;
    Ib_RMS_sic = 47.0 + (rand() % 2000 - 1000) * 0.001;
    Ic_RMS_sic = 46.0 + (rand() % 2000 - 1000) * 0.001;
    Freq_sic = 60.0 + (rand() % 100 - 50) * 0.01;

    P_sic = 12000.0 + (rand() % 4000 - 2000);
    Q_sic= -3500.0 + (rand() % 1000 - 500);
    S_sic = 13000.0 + (rand() % 2000 - 1000);
    Vae_sic = 500.0 + (rand() % 200 - 100) * 0.01;
    Vcc_sic = 800.0 + (rand() % 200 - 100) * 0.01;
    P_boost = 800.0 + (rand() % 200 - 100);
    P_crowbar = 500.0 + (rand() % 200 - 100);
	*/

    Va_RMS_sic_int = (Uint16)(Va_RMS_sic * GANHOV);
    Vb_RMS_sic_int = (Uint16)(Vb_RMS_sic * GANHOV);
    Vc_RMS_sic_int = (Uint16)(Vc_RMS_sic * GANHOV);
    Ia_RMS_sic_int = (Uint16)(Ia_RMS_sic * GANHOI);
    Ib_RMS_sic_int = (Uint16)(Ib_RMS_sic * GANHOI);
    Ic_RMS_sic_int = (Uint16)(Ic_RMS_sic * GANHOI);
    Freq_sic_int = (Uint16)(Freq_sic * GANHOF);

    P_sic_int = (int16_t)(P_sic - 32768);
    Q_sic_int = (int16_t)(Q_sic - 32768);
    S_sic_int = (int16_t)(S_sic - 32768);
    Vae_sic_int = (Uint16)(Vae_sic * 64);
    Vcc_sic_int = (Uint16)(Vcc_sic * 64);
    P_boost_int = (Uint16)(P_boost);
    P_crowbar_int = (Uint16)(P_crowbar);

    // Geração de valores simulados para para o GMG
    /*Va_RMS_GMG = 220.0 + (rand() - 16384) * 0.001;
    Vb_RMS_GMG = 220.0 + (rand() - 16384) * 0.001;
    Vc_RMS_GMG = 220.0 + (rand() - 16384) * 0.001;
    Ia_RMS_GMG = 50.0  + (rand() - 16384) * 0.001;
    Ib_RMS_GMG = 50.0  + (rand() - 16384) * 0.001;
    Ic_RMS_GMG = 50.0  + (rand() - 16384) * 0.001;
    Freq_GMG = 60.0;
	*/

    Va_RMS_GMG_int = (Uint16)(Va_RMS_GMG * GANHOV);
    Vb_RMS_GMG_int = (Uint16)(Vb_RMS_GMG * GANHOV);
    Vc_RMS_GMG_int = (Uint16)(Vc_RMS_GMG * GANHOV);
    Ia_RMS_GMG_int = (Uint16)(Ia_RMS_GMG * GANHOI);
    Ib_RMS_GMG_int = (Uint16)(Ib_RMS_GMG * GANHOI);
    Ic_RMS_GMG_int = (Uint16)(Ic_RMS_GMG * GANHOI);
    Freq_GMG_int = (Uint16)(Freq_GMG * GANHOF);

    P_Ativa_GMG = Va_RMS_GMG * Ia_RMS_GMG;
    P_Reativa_GMG = 2000.0;
    P_Aparente_GMG = 5000.0;

    P_Ativa_GMG_int = (Uint16)(P_Ativa_GMG * GANHOPQ);
    P_Reativa_GMG_int = (Uint16)(P_Reativa_GMG * GANHOPQ);
    P_Aparente_GMG_int = (Uint16)(P_Aparente_GMG * GANHOPQ);*/

    // Pacote 1 - SIC RMS e Frequência
    Pacotes_spi[1][0] = 1;
    Pacotes_spi[1][1] = Va_RMS_sic_int;
    Pacotes_spi[1][2] = Vb_RMS_sic_int;
    Pacotes_spi[1][3] = Vc_RMS_sic_int;
    Pacotes_spi[1][4] = Ia_RMS_sic_int;
    Pacotes_spi[1][5] = Ib_RMS_sic_int;
    Pacotes_spi[1][6] = Ic_RMS_sic_int;
    Pacotes_spi[1][7] = Freq_sic_int;

    // Pacote 2 - SIC Potência e Supercaps
    Pacotes_spi[2][0] = 2;
    Pacotes_spi[2][1] = P_sic_int;
    Pacotes_spi[2][2] = Q_sic_int;
    Pacotes_spi[2][3] = S_sic_int;
    Pacotes_spi[2][4] = Vae_sic_int;
    Pacotes_spi[2][5] = Vcc_sic_int;
    Pacotes_spi[2][6] = P_boost_int;
    Pacotes_spi[2][7] = P_crowbar_int;

    // Pacote 3 - FV
    Pacotes_spi[3][0] = 3;
    Pacotes_spi[3][1] = Liga_Inversor_FV;
    Pacotes_spi[3][2] = var_Ext.InvFV.Potencia_Ref_FV;
    Pacotes_spi[3][3] = var_Ext.InvFV.Potencia_Med_FV;
    Pacotes_spi[3][4] = 0;
    Pacotes_spi[3][5] = 0;
    Pacotes_spi[3][6] = 0;
    Pacotes_spi[3][7] = 0;

    // Pacote 4 - GMG
    Pacotes_spi[4][0] = 4;
    Pacotes_spi[4][1] = GMG_int.P_Ativa;
    Pacotes_spi[4][2] = GMG_int.P_Reativa;
    Pacotes_spi[4][3] = GMG_int.P_Aparente;
    Pacotes_spi[4][4] = GMG_int.Va_RMS;
    Pacotes_spi[4][5] = GMG_int.Vb_RMS;
    Pacotes_spi[4][6] = GMG_int.Vc_RMS;
    Pacotes_spi[4][7] = GMG_int.Freq;

    // Pacote 5 - Correntes GMG
    Pacotes_spi[5][0] = 5;
    Pacotes_spi[5][1] = GMG_int.Ia_RMS;
    Pacotes_spi[5][2] = GMG_int.Ib_RMS;
    Pacotes_spi[5][3] = GMG_int.Ic_RMS;
    Pacotes_spi[5][4] = 0;
    Pacotes_spi[5][5] = 0;
    Pacotes_spi[5][6] = 0;
    Pacotes_spi[5][7] = 0;

    // Aqui envia o sinal para acionar a contactora pela CAN
    if(Contatora_CargaR_ant.all != Contatora_CargaR.all){
        TxObj_CargaR.pucMsgData = &Contatora_CargaR.all;
        CANMessageSet(CANB_BASE, Tx_ID_CargaR, &TxObj_CargaR, MSG_OBJ_TYPE_TX);
    }
    Contatora_CargaR_ant.all = Contatora_CargaR.all;

}



// CAN B ISR - The interrupt service routine called when a CAN interrupt is
//             triggered on CAN module B.
__interrupt void canbISR(void){

    status = CANIntStatus(CANB_BASE, CAN_INT_STS_CAUSE);

    if(status == CAN_INT_INT0ID_STATUS){

        status = CANStatusGet(CANB_BASE, CAN_STS_CONTROL);
        if(((status  & ~(CAN_ES_RXOK)) != 7) && ((status  & ~(CAN_ES_RXOK)) != 0)) errorFlag++;

    }else if(status == Rx_ID_Mensagem1){

        CANMessageGet(CANB_BASE, Rx_ID_Mensagem1, &RxObj_Mensagem1, true);
        CANIntClear(CANB_BASE, Rx_ID_Mensagem1);

        GMG_int.P_Ativa = Rx_Mensagem1[1]<<8 | Rx_Mensagem1[0];
        var_Ext.GMG.P_Ativa     = (float)(GMG_int.P_Ativa - OFFSETPQ) * GANHOPQ_inv;

        GMG_int.P_Reativa  = Rx_Mensagem1[3]<<8 | Rx_Mensagem1[2];
        var_Ext.GMG.P_Reativa      = (float)(GMG_int.P_Reativa - OFFSETPQ) * GANHOPQ_inv;

        GMG_int.P_Aparente = Rx_Mensagem1[5]<<8 | Rx_Mensagem1[4];
        var_Ext.GMG.P_Aparente     = (float)(GMG_int.P_Aparente - OFFSETPQ) * GANHOPQ_inv;

    }else if(status == Rx_ID_Mensagem2){

        CANMessageGet(CANB_BASE, Rx_ID_Mensagem2, &RxObj_Mensagem2, true);
        CANIntClear(CANB_BASE, Rx_ID_Mensagem2);

        GMG_int.Ia_RMS = Rx_Mensagem2[1]<<8 | Rx_Mensagem2[0];
        var_Ext.GMG.Ia_RMS     = (float)(GMG_int.Ia_RMS) * GANHOI_inv;

        GMG_int.Ib_RMS = Rx_Mensagem2[3]<<8 | Rx_Mensagem2[2];
        var_Ext.GMG.Ib_RMS     = (float)(GMG_int.Ib_RMS) * GANHOI_inv;

        GMG_int.Ic_RMS = Rx_Mensagem2[5]<<8 | Rx_Mensagem2[4];
        var_Ext.GMG.Ic_RMS     = (float)(GMG_int.Ic_RMS) * GANHOI_inv;

    }else if(status == Rx_ID_Mensagem3){

        CANMessageGet(CANB_BASE, Rx_ID_Mensagem3, &RxObj_Mensagem3, true);
        CANIntClear(CANB_BASE, Rx_ID_Mensagem3);

        GMG_int.Va_RMS = Rx_Mensagem3[1]<<8 | Rx_Mensagem3[0];
        var_Ext.GMG.Va_RMS     = (float)(GMG_int.Va_RMS) * GANHOV_inv;

        GMG_int.Vb_RMS = Rx_Mensagem3[3]<<8 | Rx_Mensagem3[2];
        var_Ext.GMG.Vb_RMS     = (float)(GMG_int.Vb_RMS) * GANHOV_inv;

        GMG_int.Vc_RMS = Rx_Mensagem3[5]<<8 | Rx_Mensagem3[4];
        var_Ext.GMG.Vc_RMS     = (float)(GMG_int.Vc_RMS) * GANHOV_inv;

        GMG_int.Freq  =  Rx_Mensagem3[7]<<8 | Rx_Mensagem3[6];
        var_Ext.GMG.Freq      =  (float)(GMG_int.Freq) * GANHOF_inv;
    }

    CANGlobalIntClear(CANB_BASE, CAN_GLB_INT_CANINT0);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}


void Controle_PotMin_GMG(Variaveis_Controle_Pot_GMG *GMG){

    if(GMG->Enable==1){

        GMG->cont_div++;

        if(GMG->cont_div >= GMG->N_div){

            GMG->cont_div = 0;

            GMG->erro_pot = GMG->pot_ref - GMG->pot_med;

            if(GMG->erro_pot < -GMG->erro_pot_max){
                GMG->erro_pot = GMG->erro_pot + GMG->erro_pot_max;
            }else if(GMG->erro_pot > GMG->erro_pot_max){
                GMG->erro_pot = GMG->erro_pot - GMG->erro_pot_max;
            }else{
                GMG->erro_pot = 0.0;
            }

            GMG->pot_FV_aux = GMG->KP * GMG->erro_pot + var_Ext.InvFV.Potencia_Med_FV;

            GMG->pot_FV_ref = GMG->Pf * GMG->pot_FV_ref + (1.0 - GMG->Pf) * GMG->pot_FV_aux;

            if(GMG->pot_FV_ref > GMG->POT_PV_MAX) GMG->pot_FV_ref = GMG->POT_PV_MAX;
            if(GMG->pot_FV_ref < GMG->POT_PV_MIN) GMG->pot_FV_ref = GMG->POT_PV_MIN;
        }
    }else{

        GMG->pot_FV_aux = 0.0;
        GMG->pot_FV_ref=pot_FV_ref_CPU1;  // Caso o controle esteja OFF recebe e referencia de pot do PV do CPU1
    }

}



/*void Controle_PotMin_GMG(Variaveis_Controle_Pot_GMG *GMG){

    if(GMG->Enable==1){

        GMG->cont_div++;

        if(GMG->cont_div >= GMG->N_div){

            GMG->cont_div = 0;

            GMG->erro_pot = GMG->pot_ref - GMG->pot_med;

            GMG->erro_pot_filt = (1.0f - GMG->Pf) * GMG->erro_pot_filt + GMG->Pf * GMG->erro_pot;

            if(GMG->erro_pot_filt < -GMG->erro_pot_max){
                GMG->erro_pot_filt = GMG->erro_pot_filt + GMG->erro_pot_max;
            }else if(GMG->erro_pot_filt > GMG->erro_pot_max){
                GMG->erro_pot_filt = GMG->erro_pot_filt - GMG->erro_pot_max;
            }else{
                GMG->erro_pot_filt = 0.0;
            }

            GMG->pot_FV_PI = GMG->KP * GMG->erro_pot_filt + GMG->xi_pot;

            GMG->pot_FV_ref = GMG->pot_FV_PI;
            if(GMG->pot_FV_ref > GMG->POT_PV_MAX) GMG->pot_FV_ref = GMG->POT_PV_MAX;
            if(GMG->pot_FV_ref < GMG->POT_PV_MIN) GMG->pot_FV_ref = GMG->POT_PV_MIN;

            GMG->xi_pot = GMG->xi_pot + GMG->KI * GMG->erro_pot_filt + GMG->KW * (GMG->pot_FV_ref - GMG->pot_FV_PI);

        }
    }else{
        GMG->xi_pot = 0.0;
        GMG->pot_FV_PI = 0.0;
        GMG->erro_pot_filt = 0.0;

        if(GMG->Enable==0){
            GMG->pot_FV_ref=0;
            GMG->Enable=2;
        }

    }

}*/

//
// End of file
//
