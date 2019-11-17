//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//PruebaLed20.c           Pruebas, 2.0         2017-04-05 
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#define LedDisableInterrupts INTCON &= 0x7F;   // bcf INTCON,7
//#define EnableInterrupts  INTCON |= 0x80;   // bsf INTCON,7

void Delay_ms (int delay);
void LedParpadea (int Nveces);      // Parpadea Nveces, indefinidamente

void IntDelay_ms (int delay);
void IntLedParpadea (int Nveces);      // Parpadea Nveces, indefinidamente

#define LedAmarillo PORTCbits.RC0
int LedContMs;

void LedAmarilloInit () {
    TRISCbits.RC0 = 0;              // LedAmarillo es salida
    LedContMs = 1;
    //if (False) {LedParpadea (1);}   // No se usa    
}

// Dentro de la Interrupcion
void LedAmarilloInterr () {         // Llamar cada ms para parpadear c/400 ms
    LedContMs -= 1;
    if (LedContMs <= 1) {
        if (LedAmarillo == 1){
           LedAmarillo = 0;
           LedContMs = 100;
        }
        else {                      // if LedAmarillo == 0
           LedAmarillo = 1;
           LedContMs = 100;
        }
    }
}

void LedParpadea (int Nveces) {     // Parpadea Nveces, indefinidamente
    int I;
    LedDisableInterrupts;
    LedAmarillo = 0;
    Delay_ms (1000);
    while (True) {
        for (I=1; I<=Nveces; I++) {
            LedAmarillo = 1;
            Delay_ms (400);
            LedAmarillo = 0;
            Delay_ms (400);
        }
        Delay_ms (500);             // Delay de separacion
    }
    //Fin();                          // No se usa
}

void Delay_ms (int delay) {
    int I;
    for (I=1; I<=delay; I++) { __delay_ms(1); }
}

//void Fin () {                       // Llamar para terminar el programa
//    while (True) {;}
//}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void IntLedParpadea (int Nveces) {     // Parpadea Nveces, indefinidamente
    int I;
    LedDisableInterrupts;
    LedAmarillo = 0;
    IntDelay_ms (1000);
    while (True) {
        for (I=1; I<=Nveces; I++) {
            LedAmarillo = 1;
            IntDelay_ms (400);
            LedAmarillo = 0;
            IntDelay_ms (400);
        }
        IntDelay_ms (500);             // Delay de separacion
    }
    //IntFin();                          // No se usa
}

void IntDelay_ms (int delay) {
    int I;
    for (I=1; I<=delay; I++) { __delay_ms(1); }
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
