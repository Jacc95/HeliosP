//ADC3.0                 Archivo ADC30.c            2017-06-30
//------------------------------------------------------------------------------
//Rutinas ADC_Init, ADC_Lee

void ADC_Init() { 
    //Puerto A
    TRISA = 0x2F;       //Puertos RA0 -> RA2, RA3 (Ref3.3v) & RA5 inputs. The rest either outputs or N.C.
    //Puerto B
    TRISB = 0xFF;       //RB2 -> RB5 son entradas, RB6 -> RB7 programmer & el resto overwrite despues
    TRISC = 0x80;       //RC7 es entrada RX del UART
    
    ANSELA = 0x2F;      //AN0->AN4 (RA0->RA3,RA5) : son analogicas
    ANSELB = 0x3C;      //RB2 -> RB5 son entradas analogicas
    
    //Configuracion de ADC pag 294 - 296 Datasheet
    ADCON2 = 0xAD;      //1TAD = 2us,justificado a la derecha, 333kHz oscillator 
//  ADCON1 = 0;         // Vref+ = Vdd, Vref- = Vss
    ADCON1 = 0x04;      // Vref+ = AN3, Vref- = Vss.  USAMOS Vref+ (AN3)
    ADCON0 = 0x01;      //Activar ADC
    
    //ADIF = 0;         //borrar bandera adc
    //ADIE = 1;         //activar interrupcion de adc
    //PEIE = 1;         //Activar interrupciones perifericas
    //GO = 1;           //Empezar a tomar lectura
}

int ADC_Lee(short AdcCanal) {
    short ind;
    //ADCON0 &= 0x01; //Limpiar el canal a leer anterior
    ADCON0 = 1+(AdcCanal << 2); //Selecciona el canal a leer nuevo
    //Preguntar acerca de la operacion anterior
    __delay_us (3);        ////////
    GO = 1; //Empezar a tomar lectura
    for (ind = 0; ind <= 60; ind++) {  ////
        if (GO == 0) {break;}
    }
    return ( (ADRESH * 256) + ADRESL ); //Preguntar por que es 256 y no 255
}
//------------------------------------------------------------------------------
//int ADC_LeeFuera(short AdcCanal) {
//    short ind;
//    //ADCON0 &= 0x01; //Limpiar el canal a leer anterior
//    ADCON0 = 1+(AdcCanal << 2); //Selecciona el canal a leer nuevo
//    __delay_us (30);        ////////
//    GO = 1; //Empezar a tomar lectura
//    for (ind = 0; ind <= 600; ind++) {  ////
//        if (GO == 0) {break;}
//    }
//    return ( (ADRESH * 256) + ADRESL );
//}
//------------------------------------------------------------------------------
