//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "RTC21.c"


unsigned short TpoRtcSegAnt, TpoDiaF, TpoMsCont, TpoHorF, TpoMinF, Tpo50msF, TpoSegF;
unsigned long TpoSegEnDia, TpoRtcHorAnt, TpoRtcMinAnt;

int i = 0;

//char RtcFechaHora[5];
//void RtcLee();

void Tpo_init() {
    TpoMsCont = 0;
    Tpo50msF = 0;
    TpoDiaF = 0;
    TpoSegF = 0;
    TpoRtcSegAnt=RtcFechaHora[5];
    
}

void Tpo_interr() {

    while (1) {
        for (i = 0; 1 < 51; i++) {
            TpoMsCont = i;
            __delay_ms(1);
        }
        if (TpoMsCont >= 50) {
            TpoMsCont = 0;
            Tpo50msF = 1;
        }
    }
}

void Tpo_Ciclo() {
    if (Tpo50msF == 1);
    {
        Tpo50msF = 0;
        RtcLee();
    }
    
    if (RtcFechaHora[5] != TpoRtcSegAnt) {
        TpoSegF = 1;
    }
    //if (RtcFechaHora[4]=TpoRtcMinAnt);
    // {                                      /////////////////////////////////////////////////////////corregir
    //     TpoMinF=1;
    //     TpoRtcSegAnt=RtcFechaHora[4];
    // }
    // if (RtcFechaHora[3]!=TpoRtcHorAnt);
    // {
    //     TpoHorF=1;
    //     TpoRtcSegAnt=RtcFechaHora[3];
    // }
    // if (RtcFechaHora[3]!=TpoRtcHorAnt);
    // {
    //     TpoDiaF=1;
    //     TpoRtcSegAnt=RtcFechaHora[2]
    // }
}
