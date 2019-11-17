//RTC Dummy 1.0         2017-01-23      // Fin:
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
char RtcFechaHora[7];               // "AAMMDDHHmmSSDS", [3]=HH=Hora(0-23)
            // cada caracter es un numero en binario (AA,MM,DD,HH,mm,SS,DS)
//------------------------------------------------------------------------------
void RtcInit (){ // Inicializa RTC
    RtcFechaHora [0] = 17;              // 2017
    RtcFechaHora [1] = 1;               // enero
    RtcFechaHora [2] = 1;               // 1
    RtcFechaHora [3] = 14;              // 14 horas (2pm)
    RtcFechaHora [4] = 0;               // 0 min
    RtcFechaHora [5] = 0;               // 0 seg
    RtcFechaHora [6] = 1;               // domingo
}  // fin de RtcInit
//------------------------------------------------------------------------------
/*  Para pruebas iniciales, usamos solo 2 alternativas de c/u (seg,min,etc)
void RtcLee () {        // Lee hora actual       // RTC -> RtcFechaHora
                        // Dummy: sgte segundo
    RtcFechaHora[5] ++;                 // sgte Seg
    if (RtcFechaHora[5] >= 60) {
        RtcFechaHora[5] = 0;
        RtcFechaHora[4] ++;                 // sgte Min (0-59)
        if (RtcFechaHora[4] >= 60) {
            RtcFechaHora[4] = 0;
            RtcFechaHora[3] ++;                 // sgte Hora (0-23)
            if (RtcFechaHora[3] >= 24) {
                RtcFechaHora[3] = 0;
                RtcFechaHora[2] ++;                 // sgte Dia del Mes
                if (RtcFechaHora[2] >= 30) {
                    RtcFechaHora[2] = 1;
                    RtcFechaHora[1] ++;                 // sgte Mes
                    if (RtcFechaHora[1] > 12) {
                        RtcFechaHora[1] = 1;
                        RtcFechaHora[0] ++;                 // sgte Anio
    }   }   }   }   }
}  // fin de RtcLee
/ **/
//------------------------------------------------------------------------------
void RtcLee () {        // Lee hora actual       // RTC -> RtcFechaHora
                        // Dummy: sgte segundo.   Solo 2 alternativas c/u
    RtcFechaHora[5] ++;                 // sgte Seg (0-1)
    if (RtcFechaHora[5] >= 2) {
        RtcFechaHora[5] = 0;
        RtcFechaHora[4] ++;                 // sgte Min (0-1)
        if (RtcFechaHora[4] >= 2) {
            RtcFechaHora[4] = 0;
            RtcFechaHora[3] ++;                 // sgte Hora (0,1,14=0xE)
            if (RtcFechaHora[3] == 2) {RtcFechaHora[3] = 14;}
            if (RtcFechaHora[3] >= 15) {
                RtcFechaHora[3] = 0;
                RtcFechaHora[2] ++;                 // sgte Dia del Mes (1-2)
                if (RtcFechaHora[2] >= 3) {
                    RtcFechaHora[2] = 1;
                    RtcFechaHora[1] ++;                 // sgte Mes (1-2)
                    if (RtcFechaHora[1] >= 3) {
                        RtcFechaHora[1] = 1;
                        RtcFechaHora[0] ++;                 // sgte Anio
    }   }   }   }   }
}  // fin de RtcLee
//------------------------------------------------------------------------------
void RtcEsc () {        // Esc hora actual       // RtcFechaHora -> RTC
}  // fin de RtcEsc
//------------------------------------------------------------------------------