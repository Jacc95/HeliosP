// Prueba Memory Roller 2.6, con XC8    Arch:MemRollerPrueba26.c    2017-06-21
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Al generar un proyecto nuevo, poner ---- INTERNAL OSCILATOR ----
//------------------------------------------------------------------------------
#define PpVersion 0x0327  //Aplicacion 03 = PruebaMemRoller version 2.6
//------------------------------------------------------------------------------
//******************************************************************************
//------------------ COMANDOS DEL PROGRAMA PRINCIPAL ---------------------------
//******************************************************************************
// COMANDO                         Comando           Contesta
//******************************************************************************
// 00h: Pregunta la version        Dir-00h           (Dir+80)-00h-Ap-Vv
// 01h: Leer Entradas Anal�g/dig   Dir-01h           (Dir+80)-01h-EA-EA-Dig
// 02h: Escribir a Sal Dig         Dir-02h-Dig       (Dir+80)-02h

// 03h: RTC Lee:                   Dir-03h           (Dir80)-03-AA,Mes,DD,HH,Min,SS,DS
// 04h: RTC Escribe                Dir-04h-AA,Mes,DD,HH,Min,SS,DS  (Dir+80)-04

// 05h: MrFormat                   Dir-05h           (Dir+80)-05h-Dir-Ult
// 06h: MrIinit                    Dir-06h           (Dir+80)-06h-Dir-Ult
// 07h: MrAgregaReg (MrUlt+1)      Dir-07h-Reg->     (Dir+80)-07h-Dir-Ult
// 08h: MrLeeReg (MrDirV)          Dir-08h           (Dir+80)-08h-Dir-Reg->
// 09h: MrUlt (MrDirV=MrUltV)      Dir-09h           (Dir+80)-09h-Dir-Ult
// 0Ah: MrSgte (MrDirV++)          Dir-0Ah           (Dir+80)-0Ah-Dir-Ult
// 0Bh: MrAnt  (MrDirV--)          Dir-0Bh           (Dir+80)-0Bh-Dir-Ult
// 0Ch: MrModificaMrDirV           Dir-0Ch-DirV      (Dir+80)-0Ch-Dir-Ult
// 0Eh: Lee 8 bytes Mem EEPROM ser Dir-0Eh-Dir       (Dir+80)-8Bytes 
// 0Fh: Escribe 1 Byte Mem EEPROM  Dir-0Fh-Dir-Byte  (Dir+80)-0Fh
//******************************************************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ConfigurationBits.c"
#include "PruebaLed20.c"

//------------------------------------------------------------------------------
//#define LedAmarillo PORTCbits.RC0
//int LedContMs;
//#define True 1        // ya estan en configuration bits
//#define False 0       // ya estan en configuration bits
//------------------------------------------------------------------------------
//#define PruebaMst1      // Parpadea 1 al llegar a MstInit
//#define PruebaMst2      // Parpadea 2 al llegar a MstCiclo
//#define PruebaMst3      // Parpadea 3 al llegar a MstInterr
//#define PruebaPpal1     // Parpadea 1 al llegar a Ppal_Init
//#define PruebaPpal2     // Parpadea 2 al llegar a Ppal_Ciclo
//#define PruebaPpal3     // Parpadea 3 al llegar a Ppal_Interr
//------------------------------------------------------------------------------

unsigned short PpalRsCMD;
//char Hora[7], txt [20];

void Ppal_Init();
void Ppal_Ciclo();
void RsInterr();
void Ppal_Interr();
void Ppal_RsComando();

//------------------------------------------------------------------------------
#include "ConfigurationBits.c"
#include "Maestro21.c"
#include "I2C30.c"
#include "Mse21.c"
#include "RTC21.c"
//#include "RTC-dummy21.c"
#include "MemoryRoller27.c"

//#include "RsComando13.c"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void Ppal_Init() {
    //--------------------------------------------------------------------------
#ifdef PruebaPpal1           // Llega a Ppal_Init
    LedParpadea(1);
#endif
    //--------------------------------------------------------------------------
    PORTC = 0;
    PORTCbits.RC6 = 1; // necesario?

    RsInit(9600);
    Delay_ms(100);
    // __delay_ms (100);
    RsTxBuffer [0] = 02;
    RsTxBuffer [1] = 'h'; // 68
    RsTxBuffer [2] = 'p'; // 70
    RsTxBuffer [3] = 'j'; // 6A
    RsTxBuffer [4] = 'n';
    RsTxEnviaMsg(3);
    Delay_ms(1500);
    //__delay_ms(1500);

    //MrFecha = 0x00123456;
    //strcpy (MrReg, "abcdefg");
    I2CInit(); // Inicializa RTC (e I2C)    

    RtcInit(); // Inicializa RTC (e I2C)
    MrInicializa();
    //    MrFormat(); // Prueba en RAM. es necesario formatear RAM
}

//------------------------------------------------------------------------------

void Ppal_Ciclo() {
    //--------------------------------------------------------------------------
#ifdef PruebaPpal2           // Llega a Ppal_Ciclo
    LedParpadea(2);
#endif
    //--------------------------------------------------------------------------
    //    RsTxBuffer [0] = 0xAA;
    //    RsTxBuffer [1] = 0xBB;   // 68
    //    RsTxBuffer [2] = 0xCC;   // 70
    //    RsTxEnviaMsg (3);
    //    Delay_ms(1000);
}

//------------------------------------------------------------------------------

void Ppal_Interr() {
    //--------------------------------------------------------------------------
#ifdef PruebaPpal3           // Llega a Ppal_Ciclo
    LedParpadea(3);
#endif
    //--------------------------------------------------------------------------
    RsInterr();

    LedAmarilloInterr();

    //    LedContMs -= 1;
    //    if (LedContMs <= 1) {
    //        if (LedAmarillo == 1){
    //           LedAmarillo = 0;
    //           LedContMs = 400;
    //        }
    //        else {
    //           LedAmarillo = 1;
    //           LedContMs = 400;
    //        }
    //    }

} // Fin Ppal_Interrupt

//---------------- --------------------------------------------------------------

void Ppal_RsComando() { //  Solo si es para mi (RsDirDispositivo)
    // unsigned short MonRsCMD;
    PpalRsCMD = RsRxBuffer [1];
    RsRxRecibioMsgF = 0; // marca de recibido

    MonDir = ((RsRxBuffer[2] * 256) + (RsRxBuffer[3])); // Por si se necesita
    switch (MonRsCMD) {
        case 0x00: //version
            RsTxBuffer [2] = ((PpVersion >> 8) & (0xFF));
            unsigned short ind = (PpVersion & (0x00FF));
            RsTxBuffer [3] = ind;
            //            RsTxBuffer [3] = ( (PpVersion     ) & (0xFF) );
            RsTxEnviaMsg(4);
            break;

            //        case 0x01:  //Leer Entradas Anl�gicas y digitales
            //            DisableInterrupts;
            //            RsTxBuffer [2] = PortA;
            //            RsTxBuffer [3] = ( (ANA0 >> 8) & (0xFF) );
            //            RsTxBuffer [4] = ( (ANA0     ) & (0xFF) );
            //            RsTxBuffer [5] = ( (ANA1 >> 8) & (0xFF) );
            //            RsTxBuffer [6] = ( (ANA1     ) & (0xFF) );
            //            RsTxBuffer [7] = ( (ANA2 >> 8) & (0xFF) );
            //            RsTxBuffer [8] = ( (ANA2     ) & (0xFF) );
            //            EnableInterrupts;
            //            RsTxEnviaMsg(9);
            //            break;
            //        case 0x02:  //Escribir a Salidas Digitales
            ////            PortC.f2 = ( (RsRxBuffer[2] >> 1) & (0x01) );
            ////            PortC.f1 = ( (RsRxBuffer[2]     ) & (0x01) );
            //            Portc = RsRxBuffer[2];
            //            RsTxEnviaMsg(2);
            //            break;// */

        case 0x03:  // RTC Lee: (02,04); ans: (82,04,AA,Mes,DD,HH,Min,SS,DS)
            RtcLee ();
            memcpy(&RsTxBuffer[2],RtcFechaHora, 7);        // copia en Bin
            //RsTxBuffer[4] = 0x9C;
            RsTxEnviaMsg(9);            
            break;
        case 0x04:  // RTC Escribe (inicializa) (02,03,AA,Mes,DD,HH,Min,SS,DS)  
            memcpy(RtcFechaHora, &RsRxBuffer[2], 7);        // copia en Bin
            RtcEsc ();
            RsTxEnviaMsg(2);            // acknowledge
            break;

        case 0x05:  // Mr Format (inicializa Mr) (02,05)
            MrFormat();
            memcpy(&RsTxBuffer[2], &MrDirV, 2);
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            RsTxEnviaMsg(6); // MrDirV(LH), MrUltV (LH)
            break;
        case 0x06: // Mr Init (busca ultimo reg ["*"] 
            MrInit();
            memcpy(&RsTxBuffer[2], &MrDirV, 2);
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            RsTxEnviaMsg(6); // MrDirV(LH), MrUltV (LH)
            break;
        case 0x07: // Mr Agrega Reg    // (02,07, Reg(max 5 bytes)
            memcpy(&MrReg[2], &RsRxBuffer[2], MrLen - 2); // copia registro
            MrAgregaReg(); // Agrega Reg
            memcpy(&RsTxBuffer[2], &MrDirV, 2);
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            memcpy(&RsTxBuffer[6], &RtcFechaHora[0], 6); // RtcFechaHora
            memcpy(&RsTxBuffer[12], &MrReg[0], MrLen); // copia registro
            RsTxEnviaMsg(12 + MrLen); // MrDirV(LH), MrUltV (LH)
            break;
        case 0x08: // MrRegLee   contesta: (82,08,MrDirV,MrUltV,Hora12, Reg)
            MrLeeReg();
            memcpy(&RsTxBuffer[2], &MrDirV, 2);
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            memcpy(&RsTxBuffer[6], &MrReg[0], MrLen); // copia registro
            RsTxEnviaMsg(6 + MrLen); // MrDirV(LH), MrUltV (LH)
            break;
        case 0x09: // MrUlt
            MrUlt();
            memcpy(&RsTxBuffer[2], &MrDirV, 2);
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            RsTxEnviaMsg(6); // MrDirV(LH), MrUltV (LH)
            break;
        case 0x0A: // MrSgte
            MrSgte();
            memcpy(&RsTxBuffer[2], &MrDirV, 2);
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            RsTxEnviaMsg(6); // MrDirV(LH), MrUltV (LH)
            break;
        case 0x0B: // MrAnt
            MrAnt();
            memcpy(&RsTxBuffer[2], &MrDirV, 2); 
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            RsTxEnviaMsg(6); // MrDirV(LH), MrUltV (LH)
            break;
        case 0x0C: // Modifica MrDirV
            memcpy(&MrDirV, &RsRxBuffer[2], 2); // copia MrDirV
            memcpy(&RsTxBuffer[2], &MrDirV, 2);
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            RsTxEnviaMsg(6); // MrDirV(LH), MrUltV (LH)
            break;
        case 0x0D: // MrRegLeeAnt   contesta: (82,0D,MrDirV,MrUltV,Hora12, Reg)
            MrSgte();
            MrLeeReg();
            memcpy(&RsTxBuffer[2], &MrDirV, 2);
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            memcpy(&RsTxBuffer[6], &MrReg[0], MrLen); // copia registro
            RsTxEnviaMsg(6 + MrLen); // MrDirV(LH), MrUltV (LH)
            break;

        case 0x0E: //Leer 8 bytes de memoria eeprom serial (02,0E,AdressHigh,AdressLow,NBytes))
            MseLee (MonDir, &RsTxBuffer[2], 8); ////////////////
            RsTxEnviaMsg(10);
            break;

        case 0x0F: //Escribir un byte a memoria eeprom serial
            MseEsc(MonDir, &RsRxBuffer[4], 1);
//            RsTxEnviaMsg(2);        // acknowledge
            RsTxEnviaMsg(2);        // acknowledge
            break;

        default:
            //       RsTxBuffer[2] = 0xEE;
            RsTxBuffer[2] = 0xE1;
            RsTxEnviaMsg(3);
            RsRxRecibioMsgF = 0;
            break;
    } //switch
} // fin de Ppal_RsComando
// */
//------------------------------------------------------------------------------

void interrupt Interr() {
    TMR2IF = 0; //PIR1 & 0xFD;  //PIR1.F1 = 0
    // RsInterr ();
    // MstInterr();
    if (MstPpEjecutaF == 1) {
        if (MstPpInicializadoF == 1) {
            Ppal_Interr();
        }
    }
}

//------------------------------------------------------------------------------

//int main(int argc, char** argv) {
//    MstMain(); // main en el maestro (monitor)
//}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
