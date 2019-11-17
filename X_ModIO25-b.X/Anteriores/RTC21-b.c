//RTC 2.1           Arch:RTC21.c           2017-04-17       RD FA
//------------------------------------------------------------------------------
//Rutinas:
//RtcEsc()
//RtcLee()
//------------------------------------------------------------------------------

//char Bcd1,Hex1,Char1;
char RtcFechaHora[7];   // "AAMMDDHHmmSSDS", [3]=HH=Hora(0-23)
void RtcEsc();          // Esc hora actual       // RtcFechaHora -> RTC
void RtcLee();          // Lee hora actual       // RTC -> RtcFechaHora

char Hex2Bcd (char Hex1) {
    char Bcd1;
    Bcd1 = ((Hex1/10)<< 4) +(Hex1 % 10);
    return (Bcd1);
}

char Bcd2Hex (char Bcd1){
char Hex1;
    Hex1 = ((Bcd1 >> 4)*10) +(Bcd1 & 0x0F);
    return (Hex1);
}
//------------------------------------------------------------------------------
void RtcInit (){ // Inicializa RTC
    RtcLee();
    if (  ( RtcFechaHora[5] & (0x80) ) != 0  ) {
        RtcFechaHora [0] = 11;              // 2011
        RtcFechaHora [1] = 5;               // mayo
        RtcFechaHora [2] = 10;              // 10
        RtcFechaHora [3] = 1;               // 1 horas (1am)
        RtcFechaHora [4] = 2;               // 2 min
        RtcFechaHora [5] = 3;               // 3 seg
        RtcFechaHora [6] = 1;               // domingo
        RtcEsc();
    } // if
}  // fin de RtcInit
//------------------------------------------------------------------------------
void RtcEsc() { // Esc hora actual       // RtcFechaHora -> RTC
    I2CStart(); // issue start signal
    I2CEscByte(0xD0); // address DS1307
    I2CEscByte(0); // empezar desde el registro 0 (REG0) (segundos)
    I2CEscByte(Hex2Bcd(RtcFechaHora[5]) & 0x7F); // segundos, CH=0
    I2CEscByte(Hex2Bcd(RtcFechaHora[4])); // minutos
    I2CEscByte(Hex2Bcd(RtcFechaHora[3])); // Horas (24-hours mode)
    I2CEscByte(Hex2Bcd(RtcFechaHora[6])); // dia semana (1 domingo 7 sabado)
    I2CEscByte(Hex2Bcd(RtcFechaHora[2])); // dia mes
    I2CEscByte(Hex2Bcd(RtcFechaHora[1])); // mes
    I2CEscByte(Hex2Bcd(RtcFechaHora[0])); // a�o
    I2CEscByte(0);                        // No salida SQW
    I2CStop(); // issue stop signal
} // fin de RtcEsc
////------------------------------------------------------------------------------
void RtcLee() { // Lee hora actual       // RTC -> RtcFechaHora
    char Char1;
    I2CStart();
    I2CEscByte(0xD0);   
    I2CEscByte(0); 
    I2CStart();
    I2CEscByte(0xD1);
    Char1 = I2CLeeByte(); //segundo
    if ( (Char1 & 0x80) == 0 )  {RtcFechaHora[5] = Bcd2Hex(Char1);}     //sec
    else                        {RtcFechaHora[5] = Char1;}
    I2CSendAck();
    
    Char1 = I2CLeeByte(); //minuto
    RtcFechaHora[4] = Bcd2Hex(Char1);
    I2CSendAck();
    
    Char1 = I2CLeeByte(); //Hora
    RtcFechaHora[3] = Bcd2Hex(Char1);
    I2CSendAck();
    
    Char1 = I2CLeeByte(); //dia semana (1=domingo, 7=sabado)
    RtcFechaHora[6] = Bcd2Hex(Char1);
    I2CSendAck();
    
    Char1 = I2CLeeByte(); //dia
    RtcFechaHora[2] = Bcd2Hex(Char1);
    I2CSendAck();
    
    Char1 = I2CLeeByte(); //mes
    RtcFechaHora[1] = Bcd2Hex(Char1);
    I2CSendAck();
    
    Char1 = I2CLeeByte(); //a�o
    RtcFechaHora[0] = Bcd2Hex(Char1);
    I2CSendNAck();
    I2CStop();
} //fin RtcLee

