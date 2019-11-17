// Prueba de ModIO 10, con XC8    Arch:Programa principa.c    2017-07-15
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Comentado y escrito por MC, dise�o y estructura MC y RD.
//------------------------------------------------------------------------------
#define PpVersion 0x0710  //Aplicacion 01 = Programa Principal version 1.0
//------------------------------------------------------------------------------
//******************************************************************************
//------------------ COMANDOS DEL PROGRAMA PRINCIPAL ---------------------------
//******************************************************************************
// COMANDO                         Comando           Contesta
//******************************************************************************
// 00h: Pregunta la version        Dir-00h           (Dir+80)-00h-Ap-Vv
// 01h: Leer Entradas Analóg/dig   Dir-01h           (Dir+80)-01h-EA-EA-Dig
// 02h: Escribir a Sal Dig         Dir-02h-Dig       (Dir+80)-02h

// 03h: RTC Lee:                   Dir-03h    (Dir80)-03-AA,Mes,DD,HH,Min,SS,DS
// 04h: RTC Escribe                Dir-04h-AA,Mes,DD,HH,Min,SS,DS  (Dir+80)-04

// 05h: MrFormat                   Dir-05h           (Dir+80)-05h-Dir-Ult
// 06h: MrIinit                    Dir-06h           (Dir+80)-06h-Dir-Ult
// 07h: MrAgregaReg (MrUlt+1)      Dir-07h-Reg->     (Dir+80)-07h-Dir-Ult
// 08h: MrLeeReg (MrDirV)          Dir-08h           (Dir+80)-08h-Dir-Reg->
// 09h: MrUlt (MrDirV=MrUltV)      Dir-09h           (Dir+80)-09h-Dir-Ult
// 0Ah: MrSgte (MrDirV++)          Dir-0Ah           (Dir+80)-0Ah-Dir-Ult
// 0Bh: MrAnt  (MrDirV--)          Dir-0Bh           (Dir+80)-0Bh-Dir-Ult
// 0Ch: MrModificaMrDirV           Dir-0Ch-DirV      (Dir+80)-0Ch-Dir-Ult
// 0Eh: Lee 8 bytes Mem EEPROM ser Dir-0Eh-Dir       (Dir+80)-8Bytes 
// 0Fh: Escribe 1 Byte Mem EEPROM  Dir-0Fh-Dir-Byte  (Dir+80)-0Fh
//******************************************************************************
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ConfigurationBits.c"
//#include "Maestro21.c"
#include "MemoryRoller27.c"
#include "ADC30.c"

//#include "Mse21.c"
//#include "RTC21.c"


void Ppal_Ciclo();
void Ppal_Interr();
void Ppal_RsComando();


int MIOAdcAct[5],MIOAdcAnt[5],MIOAdcMin[5],_TpoSegF,MIOAdcDelta[5],
    MIOAdcDivisor[5],MIOTpoDeltaMax,MIOTpoDeltaMin;   

unsigned short MIOSegCont;




void Mio_Init() {
int MIOTpoDeltaMin=60;         //si hay mucho cambio se registra cada min.
int MIOTpoDeltaMax=600;   //si hay poco cambio se registra cada 10 min.

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


char MrReg[7];           //Preparamos el registro del Mrreg
}


void MioInter(){
   
    
  TpoInter();                 //Comienza con la interrupcion de 50 ms
    
    MIOAdcAct[0] = ADC_Lee(0);   //Lee las entradas analogicas de 0-4 y las guarda 
    MIOAdcAct[1] = ADC_Lee(1);    //en el arreglo de MIOAdc del 0-4
    MIOAdcAct[2] = ADC_Lee(2);  
    MIOAdcAct[3] = ADC_Lee(3);  
    MIOAdcAct[4] = ADC_Lee(4);
}

void MioCiclo() {
 

    
    if (_TpoSegF != 1) {     //Se debe activar la variable de la interrupci�n
        return;           // en la rutina de tiempo para realizar el MIO CICLO.
    }
    
    if (_TpoSegF==1) {        //Se activa la variable de 50 ms
        
        while(1){          //Comienza conteo cada segundo
            int i;
            for(i=0;i>=0;i++){
                MIOSegCont=i;
                __delay_ms(1000);
            }
        }
        if (MIOSegCont<MIOTpoDeltaMax){
        
            if (MIOSegCont<MIOTpoDeltaMin){
                return;
            }
            
        }
        if (MIOSegCont>=MIOTpoDeltaMin){
            
            if ((MIOAdcAct[0]-MIOAdcAnt[0])>=MIOAdcDelta[0]){
                
              MrReg[0]=((MIOAdcAct[0]-MIOAdcMin[0]))/(MIOAdcDivisor[0]);  
            }
            
             if ((MIOAdcAct[1]-MIOAdcAnt[1])>=MIOAdcDelta[2]){
                
              MrReg[1]=((MIOAdcAct[1]-MIOAdcMin[1]))/(MIOAdcDivisor[1]);  
            }
            
             if ((MIOAdcAct[2]-MIOAdcAnt[2])>=MIOAdcDelta[2]){
                
              MrReg[2]=((MIOAdcAct[2]-MIOAdcMin[2]))/(MIOAdcDivisor[2]);  
            }
            
             if ((MIOAdcAct[3]-MIOAdcAnt[3])>=MIOAdcDelta[3]){
                
              MrReg[0]=((MIOAdcAct[3]-MIOAdcMin[3]))/(MIOAdcDivisor[3]);  
            }
            
             if ((MIOAdcAct[4]-MIOAdcAnt[4])>=MIOAdcDelta[4]){
                
              MrReg[4]=((MIOAdcAct[4]-MIOAdcMin[4]))/(MIOAdcDivisor[4]);  
            }
            
            MrAgregaReg();
            MIOSegCont=0;
               
         }
        
        if (MIOSegCont>=MIOTpoDeltaMax){
            
           MrReg[0]=((MIOAdcAct[0]-MIOAdcMin[0]))/(MIOAdcDivisor[0]);
           MrReg[1]=((MIOAdcAct[1]-MIOAdcMin[1]))/(MIOAdcDivisor[1]);
           MrReg[2]=((MIOAdcAct[2]-MIOAdcMin[2]))/(MIOAdcDivisor[2]);
           MrReg[3]=((MIOAdcAct[3]-MIOAdcMin[3]))/(MIOAdcDivisor[3]);
           MrReg[4]=((MIOAdcAct[4]-MIOAdcMin[4]))/(MIOAdcDivisor[4]);
           
           MrAgregaReg();
           MIOSegCont=0;
        }
    }  
} 

//int main(int argc, char** argv) {
//    MstMain(); // main en el maestro (monitor)
//}