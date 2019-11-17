  
// Red Serie 13
//------------------------------------------------------------------------------
//bit RsTxTransF, RsRxRecibioMsgF;

#define RsDirDispositivo 0x02

#define PsRxRecibioCarF RCIF  //bit5 PIR1.RCIF
#define PsTxPuedeTransF TXIF  //bit4 PIR1.TXIF
//#define RsBOT   'b' //  254
//#define RsEOT   'f' //   255
#define RsBOT      254
#define RsEOT      255
#define RsCarDoble 250
#define RsRxBufferLenMax  30
#define RsTxBufferLenMax  30
char RsRxBuffer [RsRxBufferLenMax];
char RsTxBuffer [RsTxBufferLenMax];
int RsRxBufferLen, RsTxBufferLen;
int RsRxIndex, RsTxIndex;
char RsRxCar, RsTxCar, RsRxCS, RsTxCS;
 
 short RsTxTransF, RsRxRecibioMsgF;
 short RsRxCarDobleF, RsTxTransmitiendoF, RsTxCarDobleF;

//==============================================================================
char RsCheckSum (int RsLen, char* RsBuffer) {
  // return 0xfd;
   int CheckSum1,Ind,Car1;
   char CheckSum2;
   CheckSum1 = 0;
   for(Ind = 0; Ind < RsLen; Ind ++)
   {
      Car1 = RsBuffer [Ind];
//    cout<< "RsCheckSum  "<<Car1 <<" " <<CheckSum1 <<endl;
      if(   (Car1 & 0x80)!= 0   ) Car1 ++;
      CheckSum1 = CheckSum1 + Car1;
   }
//   return((0-CheckSum1) & 0x7F);
   CheckSum2 = (char) ((0-CheckSum1) & 0x7F);
   return (CheckSum2);
}
//==============================================================================
// Puerto Serie
char PsRxRecibeCar (){
     return (RCREG1) ;
}
void PsTxEnviaCar (char car){
     TXREG1 = car;
}
     
//int PsLeeError() {
//     return (RCSTA1);
//}

void PsInit (long Baudrate) {
     SPBRG = 207;
     SPBRGH = 0;
     if (Baudrate == 115200) {SPBRG = 16;}
     BAUDCON = 0x0A;
     RCSTA = 0x90;
     TXSTA = 0x24;
     PsRxRecibioCarF = 0;
     PsTxPuedeTransF = 1;
}
//------------------------------------------------------------------------------
void RsInit (long Baudrate){
    PsInit(Baudrate);
    RsRxCarDobleF = 0;
    RsTxCarDobleF = 0;
    RsRxRecibioMsgF = 0;
    RsRxIndex = -1;
}

void RsTxEnviaMsg (int RsTxLen){        // RsTxLen=#bytes a enviar 
    if (RsTxTransmitiendoF){return;}
//    RsTxBuffer [RsTxBufferLen] = RsCheckSum (RsTxBufferLen,RsTxBuffer);
    unsigned int Temp1 = (unsigned int)RsTxLen;
    RsTxBuffer [Temp1] = RsCheckSum (RsTxLen,RsTxBuffer);
    RsTxIndex = -2;
    RsTxBufferLen = RsTxLen;
    RsTxTransmitiendoF = 1;
}

void PsRxInterrupt () {     // Interrupcion de recepcion (red serie)
    if (PsRxRecibioCarF){
        // RsRxCar = RCREG;   
        RsRxCar = PsRxRecibeCar();
        if (!RsRxRecibioMsgF) {         // Solo le hace caso si ya proceso msg ant
            if (RsRxIndex == -1) {
                 if (RsRxCar == RsBOT) {
                     RsRxIndex = 0;
                     RsRxCarDobleF = 0;
                 }
            }
            else if (RsRxCar != RsEOT) {
                if (RsRxCar == RsBOT){
                    RsRxIndex = 0;
                    RsRxCarDobleF = 0;
                }
                else {   //no BOT
                    if (
                    RsRxCar ==  RsCarDoble) {RsRxCarDobleF = 1;}
                    else {  // No Car Doble
                        if (RsRxCarDobleF) {RsRxCar += RsCarDoble; RsRxCarDobleF=0;}
                        if (RsRxIndex >= RsRxBufferLenMax){RsRxIndex = -1;}
                        else {
                            RsRxBuffer[RsRxIndex] = RsRxCar;
                            RsRxIndex ++;
                        }  // < Max
                    } // No Car Doble
                } // No BOT
            } // !EOT
            else { // EOT
//                RsRxCS = RsCheckSum(RsRxIndex-2,RsRxBuffer);
//                RsRxCS = RsRxBuffer[RsRxIndex -1];      // No checa check Sum
//                if (RsRxCS != RsRxBuffer[RsRxIndex -1]) {RsRxIndex = -1;} // CheckSum Incorrecto
//                else  //CheckSum correcto
                if (RsRxBuffer[0] == RsDirDispositivo) {
                    RsRxBufferLen = RsRxIndex -1;
                    RsRxRecibioMsgF =1;
                }  //CheckSum correcto
            } // EOT
        }  // !NoRecibioMsgF
    } // RecibioCar
} // Fin de PsRxInterrupt

//        RsRxCar  RsRxRecibioMsgF, RsRxIndex, RsRxCarDobleF
//        RsTxCar  , RsTxIndex, RsTxCarDobleF

void PsTxInterrupt () {     // Interrupcion de Transmision
    if (  (!RsTxTransmitiendoF) || (!PsTxPuedeTransF)  ) {return;}
    // sgte: solo si Transmitiendo y Puede Transmitir
//    if (RsTxIndex == -1) {PsTxEnviaCar(RsBOT); RsTxIndex ++;}  // BOT
    if (RsTxIndex < 0) {PsTxEnviaCar(RsBOT); RsTxIndex ++;}  // BOT
    else if (RsTxIndex <= RsTxBufferLen) {      // No BOT, no EOT
        RsTxCar = RsTxBuffer[RsTxIndex];
        if (RsTxCar >= RsCarDoble) {    // Si es Car Doble
            if (!RsTxCarDobleF) {        // Primera vez, Envia Car Doble
                PsTxEnviaCar (RsCarDoble);
                RsTxCarDobleF = 1;
            }
            else{                        // Despues de Car Doble
                PsTxEnviaCar(RsTxCar - RsCarDoble);
                RsTxCarDobleF = 0;
                RsTxIndex++;
            }
        } // fin de fue Car Doble
        else {PsTxEnviaCar(RsTxCar); RsTxIndex ++;}   // No Car Doble
    } // No EOT
    else {      // Al final, Enviar EOT
        PsTxEnviaCar (RsEOT);
        RsTxTransmitiendoF = 0;
    } // fin de envio EOT
} // Fin interrupcion de Transmision
void RsInterr () {
    PsRxInterrupt ();
    PsTxInterrupt ();
}   