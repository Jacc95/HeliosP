// ModIORutinas 1.6, PIC18F25k50    Arch:ModIORutinas 16.c    2017-12-08
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//                          Estructura de datos
//MIOTpoDeltaMin: valor minimo para escritura al registro 
//MIOTpoDeltaMax: valor máxiimo 
//MIOAdcDelta[5]
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//short MioCicloHabilitado=1;
int MIOAdcAct[5],MIOAdcAnt[5],MIOAdcMin[5],MIOAdcDelta[5],
    MIOAdcDivisor[5],MIOTpoDeltaMax,MIOTpoDeltaMin,MIOAdcLeido[5]; 

int Adc2_5,Adc5v,Adc5v1;

//++++++++++++++++++++++++++++++Pruebas+++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//int MIOTpoDeltaMin=60;         //si hay mucho cambio se registra cada min.
int MIOTpoDeltaMax=180;        //si hay poco cambio se registra cada 3 min.

int MIOTpoDeltaMin=60;         //si hay mucho cambio se registra cada min.
//int MIOTpoDeltaMax=600;   //si hay poco cambio se registra cada 10 min.

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------

unsigned short MIOSegCont;

void MIOInit (){        // Inicializa Mod entrada/Salida
                        // Puerto A como entradas analogicas
    TRISA = 255;        //Puerto A son entradas
    ANSELA = 0x2f;      //AN0->AN4 (RA0->RA3,RA5) : son analogicas
    
//    MIOAdcDelta[0]=2;   //tomar el valor con diferenciade 2para que no registre
//    MIOAdcDelta[1]=2;   //datos con diferencia de uno (muy iguales)
//    MIOAdcDelta[2]=2;
//    MIOAdcDelta[3]=2;
//    MIOAdcDelta[4]=2;

    MIOAdcDelta[0]=3;   //tomar el valor con diferenciade 2para que no registre
    MIOAdcDelta[1]=3;   //datos con diferencia de uno (muy iguales)
    MIOAdcDelta[2]=3;
    MIOAdcDelta[3]=3;
    MIOAdcDelta[4]=3;

    MIOAdcDivisor[0]=1;
    MIOAdcDivisor[1]=1;
    MIOAdcDivisor[2]=1;
    MIOAdcDivisor[3]=1;
    MIOAdcDivisor[4]=1;

    MIOAdcMin[0]=0;
    MIOAdcMin[1]=0;
    MIOAdcMin[2]=0;
    MIOAdcMin[3]=0;
    MIOAdcMin[4]=0;

    MrReg[0]=0;
    MrReg[1]=0;
    MrReg[2]=0;
    MrReg[3]=0;
    MrReg[4]=0;

    MIOAdcAnt[0]=255;
    MIOAdcAnt[1]=255;
    MIOAdcAnt[2]=255;
    MIOAdcAnt[3]=255;
    MIOAdcAnt[4]=255;
   
    MioCicloHabilitado=1;   
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//inicializa las variables.
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

}

void MioInterr(){
    
  //Tpo_interr();               //Comienza con la interrupcion de 50 ms
    
//    Adc2_5 = ADC_Lee(4);        //valor ADC a 2.5
        
    MIOAdcLeido[0] = ADC_Lee(0);    //Lee las entradas analogicas de 0-4 y las guarda 
    MIOAdcLeido[1] = ADC_Lee(1);    //en el arreglo de MIOAdc del 0-4
    MIOAdcLeido[2] = ADC_Lee(2); 
//    MIOAdcLeido[3] = ADC_Lee(3); 
    MIOAdcLeido[3] = ADC_Lee(4); 
    
//    Adc5v1 = ADC_Lee(4)+Adc2_5; //Sumar valor del ADC 2.5 y variable creada
    

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//inicializa las variables.
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MIOAdcLeido2Act(){      //Lee valores del ADC actuales y los guarda en arreglo 
    DisableInterrupts   //fuera de la interrupcion         
            
    MIOAdcAct[0] = MIOAdcLeido[0];
    MIOAdcAct[1] = MIOAdcLeido[1];
    MIOAdcAct[2] = MIOAdcLeido[2];
    MIOAdcAct[3] = MIOAdcLeido[3];
//    Adc5v=Adc5v1;
  
    EnableInterrupts    //habilita interrupcion con datos ADC actuales guardados
}                       //en arreglo

//------------------------------------------------------------------------------

void MIOAdcAjuste2_5(){       //Prepara para calibracion a 2.5
 
    long k1=3931; 

    MIOAdcLeido2Act();    

    MIOAdcAct[0] = (MIOAdcAct[0]*k1)/1000;
    MIOAdcAct[1] = (MIOAdcAct[1]*k1)/1000;  
    MIOAdcAct[2] = (MIOAdcAct[2]*k1)/1000;
    MIOAdcAct[3] = (MIOAdcAct[3]*k1)/1000;
    
    if (MIOAdcAct[0]>4095){MIOAdcAct[0]=4095;}
    if (MIOAdcAct[1]>4095){MIOAdcAct[1]=4095;}
    if (MIOAdcAct[2]>4095){MIOAdcAct[2]=4095;}
    if (MIOAdcAct[3]>4095){MIOAdcAct[3]=4095;}
}
//void MIOAdcAjuste2_5(){       //Prepara para calibracion a 2.5
//    long kx,k1; 
////    kx = 2046000;       //kx=((1023)/ADC5V)*2000
//    kx = 10230000;       //kx=((1023)*10000
//    MIOAdcLeido2Act();    
//    k1 = kx/Adc5v;       //k1=(factor de ajuste) * 10,000
////    if (MIOAdcAct[0]<=1023){MIOAdcAct[0] = (MIOAdcAct[0]*k1)/2000;}
////    if (MIOAdcAct[1]<=1023){MIOAdcAct[1] = (MIOAdcAct[1]*k1)/2000;}
////    if (MIOAdcAct[2]<=1023){MIOAdcAct[2] = (MIOAdcAct[2]*k1)/2000;}
////    if (MIOAdcAct[3]<=1023){MIOAdcAct[3] = (MIOAdcAct[3]*k1)/2000;}
//
//    MIOAdcAct[0] = (MIOAdcAct[0]*k1)/10000;
//    MIOAdcAct[1] = (MIOAdcAct[1]*k1)/10000;
//    MIOAdcAct[2] = (MIOAdcAct[2]*k1)/10000;
//    MIOAdcAct[3] = (MIOAdcAct[3]*k1)/10000;
//    
//    if (MIOAdcAct[0]>1023){MIOAdcAct[0]=1023;}
//    if (MIOAdcAct[1]>1023){MIOAdcAct[1]=1023;}
//    if (MIOAdcAct[2]>1023){MIOAdcAct[2]=1023;}
//    if (MIOAdcAct[3]>1023){MIOAdcAct[3]=1023;}
//}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MioPreparaReg(){       // compacta  10bits-->8bits + 2bits
    //MrReg de la PC : MrReg {2} del Pic corresponde al MrReg[4] de la PC
    MrReg[2] = (MIOAdcAct[0] >> 4);   //cambia registros 10 bits a 8 bits
    MrReg[3] = (MIOAdcAct[1] >> 4);   //(toma 8 bits más significativos)
    MrReg[4] = MIOAdcAct[2] >> 4;
    MrReg[5] = MIOAdcAct[3] >> 4;
    
    MrReg[6] = (  ((MIOAdcAct[0] & 0xF) << 4) + //toma 2 bits menos significativos 
                  ((MIOAdcAct[1] & 0xF) ) ) & 0xFF;
    MrReg[7] = (  ((MIOAdcAct[2] & 0xF) << 4) + //toma 2 bits menos significativos 
                  ((MIOAdcAct[3] & 0xF) ) ) & 0xFF;
}
//void MioPreparaReg(){       // compacta  10bits-->8bits + 2bits
//    int temp2;
//    temp2 =((MIOAdcAct[0]-MIOAdcMin[0]))/(MIOAdcDivisor[0]);
//    if (temp2<0){temp2=0;}
//    if (temp2>255){temp2=255;}
//    MrReg[2]=temp2;
//
//    temp2 =((MIOAdcAct[1]-MIOAdcMin[1]))/(MIOAdcDivisor[1]);
//    if (temp2<0){temp2=0;}
//    if (temp2>255){temp2=255;}
//    MrReg[3]=temp2;
//
//    temp2 =((MIOAdcAct[2]-MIOAdcMin[2]))/(MIOAdcDivisor[2]);
//    if (temp2<0){temp2=0;}
//    if (temp2>255){temp2=255;}
//    MrReg[4]=temp2;
//
//    temp2 =((MIOAdcAct[3]-MIOAdcMin[3]))/(MIOAdcDivisor[3]);
//    if (temp2<0){temp2=0;}
//    if (temp2>255){temp2=255;}
//    MrReg[5]=temp2;
//
//    temp2 =((MIOAdcAct[4]-MIOAdcMin[4]))/(MIOAdcDivisor[4]);
//    if (temp2<0){temp2=0;}
//    if (temp2>255){temp2=255;}
//    MrReg[6]=temp2;


//    MrReg[2] = (MIOAdcAct[0] >> 2)&0xFF;   //cambia registros de 10 bits a 8 bits
////    MrReg[2] = (char) (MIOAdcAct[0] >> 2);   //cambia registros de 10 bits a 8 bits
//    MrReg[3] = (MIOAdcAct[1] >> 2)&0xFF;   //(toma los 8 bits mas significativos)
////    MrReg[4] = MIOAdcAct[2] >> 2;
//    MrReg[4] = MIOAdcAct[0] & 0xFF;     //////
//    MIOAdcAct[3] = MIOAdcAct[1];///////////////
//    MrReg[5] = MIOAdcAct[3] >> 2;

    //MrReg[2] = (MIOAdcAct[0] >> 2)&0xFF;   //cambia registros 10 bits a 8 bits
    //MrReg[3] = (MIOAdcAct[1] >> 2)&0xFF;   //(toma 8 bits más significativos)
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//    MrReg[2] = (MIOAdcAct[0] >> 2);   //cambia registros 10 bits a 8 bits
//    MrReg[3] = (MIOAdcAct[1] >> 2);   //(toma 8 bits más significativos)
//    MrReg[4] = MIOAdcAct[2] >> 2;
//    MrReg[5] = MIOAdcAct[3] >> 2;
//    
//    MrReg[6] = (  ((MIOAdcAct[0] & 3) << 6) + //toma 2 bits menos significativos 
//                  ((MIOAdcAct[1] & 3) << 4) +
//                  ((MIOAdcAct[2] & 3) << 2) +
//                   (MIOAdcAct[3] & 3)  ) & 0xFF;
//}
//-----------------------------------------------------------------------------//
//------------------------------------------------------------------------------

void MioCiclo() {       // Agrega regs cada 1/10 mseg. 
    int temp2;              //temporal de limites 
    if (!MioCicloHabilitado) {return;}
    //  if (TpoSegF != 1) {     //Se debe activar la variable de la interrupción
    if (!TpoSegF) {     //Se debe activar la variable de la interrupción
        return;           // en la rutina de tiempo para realizar el MIO CICLO.
    }                   //SI NO LO HACE, FIN
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
    TpoSegF=0;  //se desactiva para hacer de nuevo el ciclo
    MIOSegCont++;  //empieza a contar los segundos 
    if (MIOSegCont<MIOTpoDeltaMax){  // Si >= TpoDeltaMax -> esc mem roller
        //----------------------------------------------------------------------
        if (MIOSegCont<MIOTpoDeltaMin){  //primera condicion antes de arrancar 
            return;
        }
        //----------------------------------------------------------------------
        MIOAdcAjuste2_5();    //Ajuste de lectura adc

        if (    (abs(MIOAdcAct[0]-MIOAdcAnt[0])<MIOAdcDelta[0]) &&
                (abs(MIOAdcAct[1]-MIOAdcAnt[1])<MIOAdcDelta[1]) &&
                (abs(MIOAdcAct[2]-MIOAdcAnt[2])<MIOAdcDelta[2]) &&
                (abs(MIOAdcAct[3]-MIOAdcAnt[3])<MIOAdcDelta[3]) ) {
            return;
        }
    }
    else {MIOAdcAjuste2_5();}     //Ajuste de lectura adc    
    
    //--------------------------------------------------------------------------
    //Escribir reg en mem roller        
    MioPreparaReg();        // compacta
    
    MrAgregaReg();       // Mem Roller agrega Hora12
    MIOSegCont=0;

    MIOAdcAnt[0]=MIOAdcAct[0];
    MIOAdcAnt[1]=MIOAdcAct[1];
    MIOAdcAnt[2]=MIOAdcAct[2];
    MIOAdcAnt[3]=MIOAdcAct[3];
    //MIOAdcAnt[4]=MIOAdcAct[4];
}  
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



