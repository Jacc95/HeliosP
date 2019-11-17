//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "RTC21.c"


unsigned short TpoMsCont,Tpo50msF, TpoSegF, TpoMinF,TpoHoraF,TpoDiaF;
unsigned short TpoRtcSegAnt, TpoRtcMinAnt, TpoRtcHoraAnt, TpoRtcDiaAnt;
unsigned long TpoSegEnDia;

//char RtcFechaHora[5];
//void RtcLee();

void Tpo_init() {
    TpoMsCont = 0;
    Tpo50msF = 0;
    TpoSegF = 0;
    TpoMinF = 0;
    TpoHoraF = 0;
    TpoDiaF = 0;
    RtcLee();
    TpoRtcSegAnt=RtcFechaHora[5];      //Guarda los datos del RTC en las 
    TpoRtcMinAnt=RtcFechaHora[4];      //variables de TpoRtc
    TpoRtcHoraAnt=RtcFechaHora[3];
    TpoRtcDiaAnt=RtcFechaHora[2];
    
    
}

void Tpo_interr() {
                      
    TpoMsCont++;  //conteo ascendente hasta llegar a los 50 ms 
    
    if (TpoMsCont >= 50) {
        TpoMsCont = 0;
        Tpo50msF = 1;
    }
}

void Tpo_Ciclo() {
    if (!Tpo50msF) {return;}  //si no se activa la bandera se sale 
    
    RtcLee();                // si se activa lee el Rtc 
    if (RtcFechaHora[5] == TpoRtcSegAnt) {return;} //si es el mismo valor fin
    TpoSegF=1;// else enciende la primer bandera que refleja los segundos
    TpoRtcSegAnt = RtcFechaHora[5];  //guarda el dato actual en la variable 
                         //anterior para dejar en historial          
    if (RtcFechaHora[4] == TpoRtcMinAnt) {return;}
    TpoMinF=1;
    TpoRtcMinAnt = RtcFechaHora[4];
    
    if (RtcFechaHora[3] == TpoRtcHoraAnt) {return;}
    TpoHoraF=1;
    TpoRtcHoraAnt = RtcFechaHora[3];
    
    if (RtcFechaHora[2] == TpoRtcDiaAnt) {return;}
    TpoDiaF=1;
    TpoRtcDiaAnt = RtcFechaHora[2];
    
}
