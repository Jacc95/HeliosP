//I2C 3.0           Arch:I2C30.c           2017-06-17    RD FA
//------------------------------------------------------------------------------
//Rutinas:
//I2CInit()
//I2CStart()
//I2CStop()
//I2CEscByte(unsigned short I2CByte) {  // Manda Byte de W -> I2C
//unsigned short I2CLeeByte() {         // Recibe Byte de mem serial/RTC -> return
//I2CSendAck()
//I2CSendNAck()
//------------------------------------------------------------------------------
short   I2CAck;     // Valido despues de I2CEscByte. Pin=0 -> I2CAck=0

#define I2CPORTSCL  PORTBbits.RB1   // para el pic18F25K50
#define I2CPORTSDA  PORTBbits.RB0   // para el pic18F25K50
#define I2CTRISSCL  TRISBbits.RB1   // para el pic18F25K50
#define I2CTRISSDA  TRISBbits.RB0   // para el pic18F25K50
#define I2CSCL0     I2CTRISSCL=0;
#define I2CSCL1     I2CTRISSCL=1;
#define I2CSDA0     I2CTRISSDA=0;
#define I2CSDA1     I2CTRISSDA=1;
#define I2Cdelay    __delay_us (19);

#define MseDirMax   (64*1024)-1     // 64K-1 para 24LC512
#define MsePageSize 128             // 128 para 24Lc512


void I2CInit() {
    SSP1CON1 = 0x3B;
    I2CPORTSCL = 0;
    I2CPORTSDA = 0;
    I2CSCL1;
    I2CSDA1;
    I2Cdelay;
}

void I2CStart() {
    I2CSDA1
    I2CSCL1
    I2Cdelay //(600ns) 1300 ns min bus idle time
    I2Cdelay
    I2CSDA0
    I2Cdelay
    I2Cdelay
    I2CSCL0
    I2Cdelay
}

void I2CStop() {
    I2CSDA0
    I2Cdelay
    I2CSCL1
    I2Cdelay
    I2Cdelay
    I2CSDA1
    I2Cdelay
}

void I2CEscByte(unsigned short I2CByte) { // Manda Byte de W -> I2C
    // Ack en Bit: I2CAck (entrada=0 -> I2CAck=1)
    short Index; 
    I2CSCL0 // Por seguridad, pues ya estaba
    for (Index = 1; Index <= 8; Index++) {
        if ((I2CByte & 0x80) == 0) {
            I2CSDA0;
        } else {
            I2CSDA1;
        }
        I2CByte = I2CByte << 1;
        I2Cdelay;
        I2CSCL1;
        I2Cdelay;
        I2Cdelay;
        I2CSCL0;
        I2Cdelay;
    } // fin del for (Ind)
    // Espera Ack                    ; Espera Ack
    I2CSDA1 // Salida se desconecta (deja que el disp envie ack)
    I2Cdelay
    I2CSCL1
    I2Cdelay
    // I2CAck se usa para saber si el ult WRITE ya se termino
    //////// Se comento porque I2CAck no se usa. 
    if (I2CPORTSDA == 0) {I2CAck = 1;}  // Ack en I2CAck (entrada=0 -> I2CAck=1)
    else                 {I2CAck = 0;}
    I2Cdelay 
    I2CSCL0
    I2Cdelay
} // I2CEscByte

unsigned short I2CLeeByte() { // Recibe Byte de mem serial/RTC -> W
    unsigned short I2CByte;
    short Index;
    I2CSCL0 // Por seguridad, pues ya estaba
    I2CSDA1 // Deja que la mem transmita
    I2Cdelay
    I2CByte = 0;
    for (Index = 1; Index <= 8; Index++) {
        I2CByte = I2CByte << 1; // no necesario la 1a. vez
        I2CSCL1
        I2Cdelay
        I2Cdelay
        if (I2CPORTSDA == 1) {
            I2CByte += 1;
        } // si SDA==0, ya esta en I2CByte
        I2CSCL0
        I2Cdelay
    } // fin de for(Ind)
    I2CSDA1;
    I2Cdelay
    return (I2CByte);
} // I2CLeeByte

void I2CSendAck() {
    I2CSCL0
    I2Cdelay
    I2CSDA0
    I2Cdelay
    I2CSCL1
    I2Cdelay
    I2Cdelay
    I2CSCL0
    I2Cdelay
    I2CSDA1 // Quita el Ack
    I2Cdelay
}

void I2CSendNAck() {
    I2CSCL0
    I2Cdelay
    I2CSDA1
    I2Cdelay
    I2CSCL1
    I2Cdelay
    I2Cdelay
    I2CSCL0
    I2Cdelay
    I2CSDA1 // Quita el Ack. Por seguridad
}
