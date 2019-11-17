
int MonDir, Dummy, LecturaAN;
unsigned short EntradaAN;
//unsigned short Ind, EntradaAN;

char Car1;

void * Ptr1;

char LeeRAM(int Dir) {
    Ptr1 = (void *) Dir;
    memcpy(&Car1, Ptr1, 1);
    return (Car1);
}

void EscRAM(int Dir, void* Car2P) {
    Ptr1 = (void *) Dir;
    //    Car1 = * ((char*) Car2P);
    memcpy(Ptr1, Car2P, 1);
    return;
}
unsigned int Dir1;
unsigned short NBytes1, MonRsCMD;
char Hora1[7], EEByte[7], Buffer1[20];

void MstRsComando() { //  Solo si es para mi (RsDirDispositivo)
    //unsigned short MonRsCMD;
    unsigned short Ind;
    MonRsCMD = RsRxBuffer [1];
    RsTxBuffer[0] = RsDirDispositivo + 0x80;
    RsTxBuffer[1] = MonRsCMD;

    if ((MonRsCMD >= 0x80)) {
        MonDir = ((RsRxBuffer[2] * 256) + (RsRxBuffer[3]));
        switch (MonRsCMD) {
            case 0x80: //Leer de memoria RAM 8 bytes
                for (Ind = 0; Ind < 8; Ind++) {
                    RsTxBuffer[2 + Ind] = LeeRAM(MonDir + Ind);
                }
                //          RsTxBuffer [2] = LeeRAM (MonDir);
                RsTxEnviaMsg(10);
                RsRxRecibioMsgF = 0;
                break;

            case 0x81: //Escribir a memoria RAM un byte
                EscRAM(MonDir, &RsRxBuffer[4]);
                RsTxEnviaMsg(2);
                RsRxRecibioMsgF = 0;
                break;

                //       case 0x82 :
                //          for (Ind = 0; Ind < 8; Ind ++){
                //              RsTxBuffer[2+Ind] = EEPROM_Read (MonDir+Ind);
                //          }
                //          RsTxEnviaMsg(10);
                //          RsRxRecibioMsgF = 0;
                //          break;
                //
                //       case 0x83 :
                //          EEPROM_Write (MonDir,RsRxBuffer[4]);
                ////          Dummy = EEPROM_Read (MonDir);
                //          RsTxEnviaMsg(2);
                //          RsRxRecibioMsgF = 0;
                //          break;
                //
                //       case 0x84 :   //Leer de memoria eeprom serial
                //          MsLee (MonDir, RsRxBuffer[4], &RsTxBuffer[2]);  ////////////////
                //          RsTxEnviaMsg(RsRxBuffer[4] + 2);
                //          RsRxRecibioMsgF = 0;
                //          break;
                //
                //       case 0x85 :   //Escribir a memoria eeprom serial
                //          Nbytes1 = RsRxBuffer[4];
                //          for (Ind = 0; Ind < Nbytes1; Ind ++){
                //               Buffer1 [Ind] = RsRxBuffer[5+Ind];
                //          }
                //          MsEsc (MonDir, RsRxBuffer[4], &RsRxBuffer[5]);
                //          RsTxEnviaMsg(2);
                //          RsRxRecibioMsgF = 0;
                //          break;
                //
                //
                //       case 0x86 :  //ver hora
                //          RTCLee (Hora1);
                //          for (Ind = 0; Ind < 7; Ind ++){
                //              RsTxBuffer[2+Ind] = Hora1[Ind];
                //          }
                //          RsTxEnviaMsg(9);
                //          RsRxRecibioMsgF = 0;
                //          break;
                //
                //       case 0x87 :  //escribir hora
                //          for (Ind = 0; Ind < 7; Ind ++){
                //              Hora1[Ind] = RsRxBuffer[2+Ind];
                //          }
                //          RTCEscX (Hora1);
                //          RsTxEnviaMsg(2);
                //          RsRxRecibioMsgF = 0;
                //          break;
                //
                //       case 0x88 :   //leer adc
                //          LecturaAN = ADC_Read(RsRxBuffer[2]);
                //          RsTxBuffer[2] = LecturaAN >> 8;
                //          RsTxBuffer[3] = LecturaAN;
                //          RsTxEnviaMsg(4);
                //          RsRxRecibioMsgF = 0;
                //          break;

            default:
                //          RsTxBuffer[2] = 0xEE; gg
                RsTxBuffer[2] = 0xE0;
                RsTxEnviaMsg(3);
                RsRxRecibioMsgF = 0;
                break;
        } //switch
    }// if (MonCMD
    else { // if (Mon CMD < 0x80) o (> 0x83)
        Ppal_RsComando();
        //          RsTxBuffer[2] = 0xEE;
        //          RsTxEnviaMsg(3);
        //          RsRxRecibioMsgF = 0;
    } // (else) if (Mon CMD < 0x80) o (> 0x83)
} //Fin MstRsComando()
