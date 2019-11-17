// Arch: DatFic11.c          Rutinas para agregar registros con datos ficticios
//2017 Agosto 09  
//------------------------------------------------------------------------------
//debe de estar despues de rutinas de Mio
unsigned char Hora12Inicio[6];
short NReg;

void DatFicInit(){       //funcion de toma hr actual -> lo pone en hrinicio
    RtcLee();
    Hora12Inicio[3]=RtcFechaHora[3];
    Hora12Inicio[4]=RtcFechaHora[4];
    Hora12Inicio[5]=RtcFechaHora[5];
    NReg = 1;
}

void DatFicAgregaReg(){
    long x;          // numero de seg desde Hora12Inicio
    long y;
    int Hora,Min,Seg;
    int x1=180;         // 3 minutos
    int x2=360;         //6 minutos
    int x3=540;         //9 minutos
    
    MioPreparaReg();
    
    //calcula dato ficticio
    RtcLee();
    Hora=RtcFechaHora[3]-Hora12Inicio[3];
    Min=RtcFechaHora[4]-Hora12Inicio[4];
    Seg=RtcFechaHora[5]-Hora12Inicio[5];                                                    
    
    x=((Hora*3600)+(Min*60)+(Seg)); // numero de seg desde Hora12Inicio

    // x esta en seg. y= valor ficticio (depende de x)
    if (x<=x1){
        y=(int)(1.1111*x+0.5);   //primer rango en de segundos a seg
    } 
    
    else if (x<=x2){
        y=(int)(250.5+(-.2777)*x); //segundo rango en de segundos a seg
    }
    else 
        y=(int)((.5555*x)-50+.5); //tercer rango en de segundos a seg
    
    if (y<0) {y=0;}         // valores minimos
    if (y>255) {y=255;}     // valores maximos

    MrReg[3]= (char) y;
    
    MrReg[4] = NReg;
    NReg++;
    
    MrAgregaReg();

}  //Fin de DatFigAgr
