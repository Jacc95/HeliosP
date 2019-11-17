//ADC3.0                 Archivo ADC30.c            2017-06-30
//------------------------------------------------------------------------------
//Rutinas ADC_Init, ADC_Lee

void ADC_Init() {       // Puerto A: An0, AN1, AN2, AN3, AN4(RA5): Analogicas 
            // RA4, RA6, RA7:Entradas Digitales
    TRISA = 255;        //Puerto A son entradas
    ANSELA = 0x2f;      //AN0->AN4 (RA0->RA3,RA5) : son analogicas
    ADCON2 = 0xAD; //1TAD = 2us,justificado a la derecha 
//  ADCON1 = 0;     // Vref+ = Vdd, Vref- = Vss
    ADCON1 = 4;     // Vref+ = AN3, Vref- = Vss.  USAMOS Vref+ (AN3)
    ADCON0 = 0x01; //Activar ADC
    //ADIF = 0;       //borrar bandera adc
    //ADIE = 1;       //activar interrupcion de adc
    //PEIE = 1;       //Activar interrupciones perifericas
    //GO = 1;         //Empezar a tomar lectura
}

int ADC_Lee(short AdcCanal) {
    short ind;
    //ADCON0 &= 0x01; //Limpiar el canal a leer anterior
    ADCON0 = 1+(AdcCanal << 2); //Selecciona el canal a leer nuevo
    __delay_us (3);        ////////
    GO = 1; //Empezar a tomar lectura
    for (ind = 0; ind <= 60; ind++) {  ////
        if (GO == 0) {break;}
    }
    return ( (ADRESH * 256) + ADRESL );
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
