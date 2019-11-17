//Memoria Serial EEPROM 2.1           Arch:Mse21.c          2017-06-17    RD FA
//------------------------------------------------------------------------------
//Rutinas:
//void MseEsc(unsigned int Dir, char* Datos, int NBytes) { // Esc a memoria serial
//void MseLee(unsigned int Dir, char* Datos, int NBytes) { // Lee de memoria serial
//------------------------------------------------------------------------------
// antes de este #include, poner #include I2C
//------------------------------------------------------------------------------

int MseUltDirLeido=0;

//void MseEsc(unsigned int Dir, char* Datos, int NBytes) { // Esc a memoria serial
//    int Index;
////    if  (  (Dir+NBytes) > (MseDirMax+1) ) ||    // Si excede el tama�o de memoria
////           ( Dir >= ((Dir/MsePageSize)*MsePageSize) )  ) { // o cruza Pag
////        LedParpadea(5);} 
//    I2CStart();
//    I2CEscByte(0xA0);
//    I2CEscByte((Dir>>8)&0xFF); 
//    I2CEscByte(Dir & 0xFF);
//    for (Index = 0; Index < NBytes; Index++) {
//        I2CEscByte(Datos[Index]);
//    }
//    I2CStop();
//} // fin de MseEsc
//

void MseEsc(unsigned int Dir, char* Datos, int NBytes) { // Esc a memoria serial
//    int Index;
    long Index;
//    if  (  (Dir+NBytes) > (MseDirMax+1) ) ||    // Si excede el tama�o de memoria
//           ( Dir >= ((Dir/MsePageSize)*MsePageSize) )  ) { // o cruza Pag
//        LedParpadea(5);} 
    I2CAck = 0;     // Espera Fin Escritura Ant
    for (Index = 0; (Index<100000) & (!I2CAck); Index++) { 
        I2CStart();
        I2CEscByte(0xA0);
    }
    I2CEscByte((Dir>>8)&0xFF); 
    I2CEscByte(Dir & 0xFF);
    for (Index = 0; Index < NBytes; Index++) {
        I2CEscByte(Datos[Index]);
    }
    I2CStop();
} // fin de MseEsc

//------------------------------------------------------------------------------
void MseLee(unsigned int Dir, char* Datos, int NBytes) { // Lee de memoria serial
//    int Index;
    long Index;
//   if ( (Dir+NBytes) > (MseDirMax+1) ){LedParpadea(5);} // Excede el tama�o de memoria
    I2CAck = 0;     // Espera Fin Escritura Ant
    for (Index = 0; (Index<100000) & (!I2CAck); Index++) {
        I2CStart();
        I2CEscByte(0xA0);
    }
    I2CEscByte((Dir>>8) & 0xFF); 
    I2CEscByte(Dir & 0xFF);
    I2CStart();
    I2CEscByte(0xA1);
    for (Index = 0; Index < (NBytes-1); Index++) {
        Datos[Index] = I2CLeeByte();
        I2CSendAck();
    }
    Datos[NBytes-1] = I2CLeeByte();
    I2CSendNAck();
    I2CStop();
    MseUltDirLeido = Dir;
} //fin MseLee
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


