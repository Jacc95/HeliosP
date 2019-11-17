// ModIO 2.4, con XC8/25k50    Arch:ModIO 24.c    2017-12-08
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Al generar un proyecto nuevo, poner ---- INTERNAL OSCILATOR ----
//------------------------------------------------------------------------------
#define PpVersion 0x0824  //Aplicacion 08 = MIO, Mod Entr/sal, version 2.4
//#define PpVersion 0x0823  //Aplicacion 08 = MIO, PruebaMod Entr/sal, version 2.3
//#define PpVersion 0x0822  //Aplicacion 08 = MIO, PruebaMod Entr/sal, version 2.2
//#define PpVersion 0x0327  //Aplicacion 03 = PruebaMemRoller version 2.6
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
// 07h: MrAgregaReg (MrUlt+1)      Dir-07h-Reg->     (Dir+80)-07h-Dir-Ult-FHr-Reg
// 08h: MrLeeReg (MrDirV)          Dir-08h           (Dir+80)-08h-Reg-Dir-Ult
// 09h: MrUlt (MrDirV=MrUltV)      Dir-09h           (Dir+80)-09h-Dir-Ult
// 0Ah: MrSgte (MrDirV++)          Dir-0Ah           (Dir+80)-0Ah-Dir-Ult
// 0Bh: MrAnt  (MrDirV--)          Dir-0Bh           (Dir+80)-0Bh-Dir-Ult
// 0Ch: MrModificaMrDirV           Dir-0Ch-DirV      (Dir+80)-0Ch-Dir-Ult
// 0Dh: MrLeeRegAnt (MrDirV--)     Dir-0Dh           (Dir+80)-0Dh-Reg-Dir-Ult
// 0Eh: Lee 8 bytes Mem EEPROM ser Dir-0Eh-Dir       (Dir+80)-8Bytes 
// 0Fh: Escribe 1 Byte Mem EEPROM  Dir-0Fh-Dir-Byte  (Dir+80)-0Fh
// 10h: Fija Inicio p/ficticio	   Dir-10h           (Dir+80)-10h-FeHr
// 11h: Tomar datos p/ficticio     Dir-11h           (Dir+80)-11h-Dir-Ult-FeHr-Reg
// 12h: Contesta con TempSeg
// 13h: Habilita MioCiclo
// 14h: Esc 5 bytes a EEPROM Ser   Dir-14h-Dir       (Dir+80)-14h
// 20h: xxx Historial
// 21h: MseLeeSgtes 8 bytes        Dir-21h           (Dir+80)-21h-8Bytes
//******************************************************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ConfigurationBits.c"
#include "PruebaLed20.c"


//------------------------------------------------------------------------------
#define DisableInterrupts   GIE = 0;
#define EnableInterrupts    GIE = 1;
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
#define PruebaTpo1      // contador segundos
//------------------------------------------------------------------------------

void Ppal_Init();
void Ppal_Ciclo();
void RsInterr();
void Ppal_Interr();
void Ppal_RsComando(); 

//------------------------------------------------------------------------------
unsigned short PpalRsCMD;
short MioCicloHabilitado=0;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "ConfigurationBits.c"
#include "Maestro21.c"
#include "I2C30.c"
#include "Mse21.c"
//#include "Historial10.c"
#include "RTC21.c"
//#include "RTC-dummy21.c"
#include "MemoryRoller29.c" 
#include "RutinaTpo20.c"
#include "ADC30.c"
#include "RutinaModIO15.c"
//#include "ModIORutinas16.c"
//#include "DatFic11.c"

//#include "RsComando13.c"
//------------------------------------------------------------------------------
#ifdef PruebaTpo1
    int TempSeg;
#endif 
//------------------------------------------------------------------------------

void Ppal_Init(){ 
    
        Delay_ms(100);
        // __delay_ms (100);
        RsTxBuffer [0] = 02;
        RsTxBuffer [1] = 'h'; // 68
        RsTxBuffer [2] = 'p'; // 70
        RsTxBuffer [3] = 'j'; // 6A
        RsTxBuffer [4] = 'n';
        RsTxEnviaMsg(3);
        Delay_ms(1500);
        
        Tpo_init();
        ADC_Init();
        I2CInit();
        RtcInit();
        MrInicializa();         // debe ser la primera rutina del Mem Roller
        MrInit();
        MIOInit();   ///////////////////////////////////////////
//        DatFicInit();
        
//------------------------------------------------------------------------------
#ifdef PruebaTpo1
    TempSeg=0;
#endif 
//------------------------------------------------------------------------------
 
}

//---------------- -------------------------------------------------------------
void Ppal_Interr(){
//------------------------------------------------------------------------------
#ifdef PruebaPpal3           // Llega a Ppal_Ciclo
    LedParpadea(3);
#endif
//------------------------------------------------------------------------------
    MioInterr();     //Lee las entradas analog y las guarda en MIOAdc
    
//    MIOAdcLeer();
    
    Tpo_interr();                 //Comienza con la interrupcion de 50 ms
    
    LedAmarilloInterr(); //Solo para pruebas 
}

//------------------------------------------------------------------------------

void Ppal_Ciclo() {
//------------------------------------------------------------------------------
#ifdef PruebaPpal2           // Llega a Ppal_Ciclo
    LedParpadea(2);
#endif
//------------------------------------------------------------------------------
    Tpo_Ciclo();  
    
    MioCiclo();  //almacena los datos importantes del Adc y los guarda en 
                 //memory roller

//------------------------------------------------------------------------------
#ifdef PruebaTpo1
    if (TpoSegF){
        TempSeg++;
        TpoSegF=0;
    }
#endif 
//------------------------------------------------------------------------------

} 

//------------------------------------------------------------------------------



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

        case 0x01:  //Leer Entrada Analogica 0
            DisableInterrupts;
            memcpy (&(RsTxBuffer [2]),  &(MIOAdcLeido[0]), 2);
            memcpy (&(RsTxBuffer [4]),  &(MIOAdcLeido[1]), 2);
            memcpy (&(RsTxBuffer [6]),  &(MIOAdcLeido[2]), 2);
            memcpy (&(RsTxBuffer [8]),  &(MIOAdcLeido[3]), 2);
            EnableInterrupts;
            RsTxEnviaMsg(10);
//            MioInterr();/////////////////////////////////////////////////////
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
            MrLeeReg();     ///////
            MrAnt();        ///////
            MrSgte();       ///////
            MrLeeReg();
            memcpy(&RsTxBuffer[2], &MrReg[0], MrLen); // copia registro
            memcpy(&RsTxBuffer[2+MrLen], &MrDirV, 2);
            memcpy(&RsTxBuffer[4+MrLen], &MrUltV, 2);
            RsTxEnviaMsg(6 + MrLen); // MrDirV(LH), MrUltV (LH)
            break;
        case 0x09: // MrUlt
            MrUlt();
            memcpy(&RsTxBuffer[2], &MrFecha12Ult, 3);
            memcpy(&RsTxBuffer[5], &MrDirV, 5);
            memcpy(&RsTxBuffer[7], &MrUltV, 7);
            RsTxEnviaMsg(9); // MrDirV(LH), MrUltV (LH)
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
            MrAnt();
            MrLeeReg();
            memcpy(&RsTxBuffer[2], &MrReg[0], MrLen); // copia registro
            memcpy(&RsTxBuffer[2+MrLen], &MrDirV, 2);
            memcpy(&RsTxBuffer[4+MrLen], &MrUltV, 2);
            RsTxEnviaMsg(6 + MrLen); // MrDirV(LH), MrUltV (LH)
            break;

        case 0x0E: //Leer 8 bytes de memoria eeprom serial (02,0E,AdressHigh,AdressLow,NBytes))
            MseLee (MonDir, &RsTxBuffer[2], 8); ////////////////
            RsTxEnviaMsg(10);
            break;

        case 0x0F: //Escribir un byte a memoria eeprom serial
            MseEsc(MonDir, &RsRxBuffer[4], 1);
            RsTxEnviaMsg(2);        // acknowledge
            break;
        
        case 0x14: //Escribir 5 byte a memoria eeprom serial
            MseEsc(MonDir, &RsRxBuffer[4], 5);
            RsTxEnviaMsg(2);        // acknowledge
            break;
        
        case 0x10: //fija inicio para la hora 12 "real" y Deshabilita MioCiclo
            //DatFicInit();
            MioCicloHabilitado=0;   // Deshabilita MioCiclo
            
            memcpy(&RsTxBuffer[2],RtcFechaHora, 7);        // copia en Bin
            RsTxEnviaMsg(9);            
            break;
        
        case 0x11: //tomar datos de la hora 12 
//            DatFicAgregaReg();
            memcpy(&RsTxBuffer[2], &MrDirV, 2);
            memcpy(&RsTxBuffer[4], &MrUltV, 2);
            memcpy(&RsTxBuffer[6], &RtcFechaHora[0], 6); // RtcFechaHora
            memcpy(&RsTxBuffer[12], &MrReg[0], MrLen); // copia registro
            RsTxEnviaMsg(12 + MrLen); // MrDirV(LH), MrUltV (LH)
            break;
 
        case 0x12:  //contesta con TempSeg
            memcpy(&RsTxBuffer[2], &TempSeg, 2);
            RsTxEnviaMsg(4); // TempSeg 
            break;
 
        case 0x13:  //Habilita MioCiclo
            MioCicloHabilitado=1;   // Deshabilita MioCiclo
            RsTxEnviaMsg(2); // TempSeg 
            break;
            
//        case 0x14:
//            MioPreparaReg();
//            MioCiclo();
//            memcpy(&RsTxBuffer[2], &MrDirV, 2);
//            memcpy(&RsTxBuffer[4], &MrUltV, 2);
//            memcpy(&RsTxBuffer[6], &RtcFechaHora[0], 6); // RtcFechaHora
//            memcpy(&RsTxBuffer[12], &MrReg[0], MrLen); // copia registro
//            RsTxEnviaMsg(12 + MrLen); // MrDirV(LH), MrUltV (LH)
//            break;
//        case 0x20:  // Borra historial: HiBorra() 
//            HiBorra();
//            RsTxEnviaMsg(2); // TempSeg 
//            break;
        case 0x21:  // Mse lee sgtes 8 bytes
            MseUltDirLeido += 8;
            MseLee (MseUltDirLeido, &RsTxBuffer[2], 8); ////////////////
            RsTxEnviaMsg(10);
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
    //RsInterr ();
    MstInterr();
    if (MstPpEjecutaF == 1) {
        if (MstPpInicializadoF == 1) {
            Ppal_Interr();
        }
    }
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
    MstMain(); // main en el maestro (monitor)
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
