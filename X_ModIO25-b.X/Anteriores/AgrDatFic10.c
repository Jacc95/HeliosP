// Arch: AgrDatFic.c          Rutina para agregar datos ficticios

unsigned long Hora,Min,Seg;
//float x;
int x1=3;
int x2=6;
int x3=9;

unsigned char Hora12Inicio[6];

void AgrDatFicInit(){       //funcion de toma hr actual -> lo pone en hrinicio
    
    Hora12Inicio[3]=0;
    Hora12Inicio[4]=0;
    Hora12Inicio[5]=0;
}



//void AgrDatFicComandos(){

//    case 0x10: //fija inicio para la hora 12 "real"
//        RtcLee();
//        
//    Hora12Inicio[3] = RtcLee(3);
//    Hora12Inicio[4] = RtcLee(4);
//    Hora12Inicio[5] = RtcLee(5);
//   
//        
//        break;
//        
//    case 0x11: //tomar datos de la hora 12 
//        RtcLee();
//        Hora=RtcFechaHora[3]-Hora12Inicio[3];
//        Min=RtcFechaHora[4]-Hora12Inicio[4];
//        Seg=RtcFechaHora[5]-Hora12Inicio[5];
//        
//        x=((Hora*60)+Min+(Seg/60.0));
//        break;
//}


char AgrDatFicFormula(float x) {
    
    RtcLee();
    Hora=RtcFechaHora[3]-Hora12Inicio[3];
    Min=RtcFechaHora[4]-Hora12Inicio[4];
    Seg=RtcFechaHora[5]-Hora12Inicio[5];                                                    
    
    x=((Hora*60)+Min+(Seg/60.0));

    if (x<=x1){
        return( (char) (66.667*x+0.5) );
    } 
    
    if (x<=x2){
        return( (char) (250.5+(-16.666)*x) );
    }
    
//    if (x<=x3){
        return( (char) ( (33.333*x)-50+.5) );
//    }
}
