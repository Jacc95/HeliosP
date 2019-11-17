// Maestro (Mini Sist Operativo).   Arch:Maestro21.c          2017-06-21
//------------------------------------------------------------------------------
// 
// * File:   Maestro21.c
//
//// Maestro  (Monitor) 2016-10-5
//
#include "RedSerie13.c"

#include "RsComando14.c"

#define True 1
#define False 0
//
char MstPpInicializadoF, MstPpEjecutaF;
//
//
void MstInterr() {
    RsInterr ();
    
    if (0) {
        IntDelay_ms (1);
        IntLedParpadea (1);
        LedAmarilloInterr();
    }
    
}

//void interrupt Interr () {
//     TMR2IF = 0;//PIR1 & 0xFD;  //PIR1.F1 = 0
          //RsInterr ();
          //MstInterr();
//     if (MstPpEjecutaF == 1){
//        if (MstPpInicializadoF == 1){
//           Ppal_Interr();
//        }
//     }
//}

void MstInit() {
    PORTB = 0;
    TRISB = 0;

    //ADCON1 = 0x0F;        // para pic18f2550
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
//    OSCCON = 0x72; //oscilador interno a 8 MHz   // 2550  //en Config Bits.c
    OSCCON = 0x62; //oscilador interno a 8 MHz     // 25K50
    PR2 = 124;    //4Mhz=249 8Mhz=124
    T2CON = 6;    //4MHz=5   8Mhz=6
    //PIE1 = PIE1 & 0xFF;  //PIE1.F1=1
    TMR2IE = 1;
    INTCON = 0xC0;
    LedAmarilloInit ();
    RsInit(9600);
    //--------------------------------------------------------------------------
    #ifdef PruebaMst1           // Llega a MstInit
        LedParpadea (1);
    #endif
    //--------------------------------------------------------------------------
    if(0){
        Delay_ms (1);
        LedParpadea (1);
    }
} // MstInit()

void MstCiclo(){
    //--------------------------------------------------------------------------
    #ifdef PruebaMst2           // Llega a MstCiclo
        LedParpadea (2);
    #endif
    //--------------------------------------------------------------------------
    
    if (RsRxRecibioMsgF) {MstRsComando();}
}

int MstMain() {
    
     MstPpEjecutaF = 1;
     MstPpInicializadoF = 0;
     MstInit();
     if (  (MstPpEjecutaF == 1) && (MstPpInicializadoF == 0)  ){
        Ppal_Init();
        MstPpInicializadoF = 1;
     }
     while (True) {
         MstCiclo();
         if (MstPpEjecutaF == 1) {
            if (MstPpInicializadoF == 0) {
               Ppal_Init();
               MstPpInicializadoF = 1;
            }
            Ppal_Ciclo ();
         }
     }
}
