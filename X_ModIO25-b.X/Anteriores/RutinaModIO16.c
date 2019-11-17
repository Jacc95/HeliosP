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
    MIOAdcDivisor[5],MIOTpoDeltaMax,MIOTpoDeltaMin; 

int MIOTpoDeltaMin=60;         //si hay mucho cambio se registra cada min.
int MIOTpoDeltaMax=600;   //si hay poco cambio se registra cada 10 min.
int Adc2_5,Adc5v,Adc5v1;


unsigned short MIOSegCont;

void MIOInit (){        // Inicializa Mod entrada/Salida
    
    MIOAdcDelta[0]=2;   //tomar el valor con diferenciade 2para que no registre
    MIOAdcDelta[1]=2;   //datos con diferencia de uno (muy iguales)
    MIOAdcDelta[2]=2;
    MIOAdcDelta[3]=2;
    MIOAdcDelta[4]=2;

    MIOAdcDivisor[0]=1;
    MIOAdcDivisor[1]=1;
    MIOAdcDivisor[2]=1;
    MIOAdcDivisor[3]=1;
    MIOAdcDivisor[4]=1;

    MIOAdcMin[0]=430;
    MIOAdcMin[1]=430;
    MIOAdcMin[2]=430;
    MIOAdcMin[3]=430;
    MIOAdcMin[4]=430;

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
    
    Adc2_5 = ADC_Lee(4);        //valor ADC a 2.5
        
    MIOAdcAct[0] = ADC_Lee(0);    //Lee las entradas analogicas de 0-4 y las guarda 
    MIOAdcAct[1] = ADC_Lee(1);    //en el arreglo de MIOAdc del 0-4
    MIOAdcAct[2] = ADC_Lee(2); 
    MIOAdcAct[3] = ADC_Lee(3); 
    MIOAdcAct[4] = ADC_Lee(4);
    
    Adc5v1 = ADC_Lee(4)+Adc2_5; //Sumar valor del ADC 2.5 y variable creada
    

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//inicializa las variables.
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MIOAdcLeer(){      //Lee valores del ADC actuales y los guarda en arreglo 
    DisableInterrupts   //fuera de la interrupcion         
            
    MIOAdcAct[0] = MIOAdcAct1[0];
    MIOAdcAct[1] = MIOAdcAct1[1];
    MIOAdcAct[2] = MIOAdcAct1[2];
    MIOAdcAct[3] = MIOAdcAct1[3];
    Adc5v=Adc5v1;
    
    EnableInterrupts    //habilita interrupcion con datos ADC actuales guardados
}                       //en arreglo

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MIOAdcPre(){       //Prepara para calibracion a 2.5
    long kx,k1;
    
    kx = 2046000;       //kx=((1023)/ADC5V)*2000

    k1 = kx/Adc5v;
    
    if (MIOAdcAct[0]<=1023){MIOAdcAct[0] = (MIOAdcAct[0]*k1)/2000;}
    if (MIOAdcAct[1]<=1023){MIOAdcAct[1] = (MIOAdcAct[1]*k1)/2000;}
    if (MIOAdcAct[2]<=1023){MIOAdcAct[2] = (MIOAdcAct[2]*k1)/2000;}
    if (MIOAdcAct[3]<=1023){MIOAdcAct[3] = (MIOAdcAct[3]*k1)/2000;}
    
    if (MIOAdcAct[0]>1023){MIOAdcAct[0]=1023;}
    if (MIOAdcAct[1]>1023){MIOAdcAct[1]=1023;}
    if (MIOAdcAct[2]>1023){MIOAdcAct[2]=1023;}
    if (MIOAdcAct[3]>1023){MIOAdcAct[3]=1023;}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MioPreparaReg(){
    int temp2;

    temp2 =((MIOAdcAct[0]-MIOAdcMin[0]))/(MIOAdcDivisor[0]);
    if (temp2<0){temp2=0;}
    if (temp2>255){temp2=255;}
    MrReg[2]=temp2;

    temp2 =((MIOAdcAct[1]-MIOAdcMin[1]))/(MIOAdcDivisor[1]);
    if (temp2<0){temp2=0;}
    if (temp2>255){temp2=255;}
    MrReg[3]=temp2;

    temp2 =((MIOAdcAct[2]-MIOAdcMin[2]))/(MIOAdcDivisor[2]);
    if (temp2<0){temp2=0;}
    if (temp2>255){temp2=255;}
    MrReg[4]=temp2;

    temp2 =((MIOAdcAct[3]-MIOAdcMin[3]))/(MIOAdcDivisor[3]);
    if (temp2<0){temp2=0;}
    if (temp2>255){temp2=255;}
    MrReg[5]=temp2;

    temp2 =((MIOAdcAct[4]-MIOAdcMin[4]))/(MIOAdcDivisor[4]);
    if (temp2<0){temp2=0;}
    if (temp2>255){temp2=255;}
    MrReg[6]=temp2;
}
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

        if ( (abs(MIOAdcAct[0]-MIOAdcAnt[0])<MIOAdcDelta[0]) &&
             (abs(MIOAdcAct[1]-MIOAdcAnt[1])<MIOAdcDelta[1]) &&
             (abs(MIOAdcAct[2]-MIOAdcAnt[2])<MIOAdcDelta[2]) &&
             (abs(MIOAdcAct[3]-MIOAdcAnt[3])<MIOAdcDelta[3]) &&
             (abs(MIOAdcAct[4]-MIOAdcAnt[4])<MIOAdcDelta[4]) ) {
            return;
        }
//        if ((MIOSegCont>MIOTpoDeltaMin) && 
//            (MIOSegCont=MIOTpoDeltaMax)){return;}
    }

    //--------------------------------------------------------------------------
    //Escribir reg en mem roller        
    MioPreparaReg();
    
    MrAgregaReg();       // Mem Roller agrega Hora12
    MIOSegCont=0;

    MIOAdcAnt[0]=MIOAdcAct[0];
    MIOAdcAnt[1]=MIOAdcAct[1];
    MIOAdcAnt[2]=MIOAdcAct[2];
    MIOAdcAnt[3]=MIOAdcAct[3];
    MIOAdcAnt[4]=MIOAdcAct[4];
}  
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



