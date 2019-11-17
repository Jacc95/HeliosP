//Memory Roller 2.9     Arch:MemRoller29.c         2017-08-30
//------------------------------------------------------------------------------
//#define MrPrueba1           // usar RAM
//------------------------------------------------------------------------------
#define MrLen       7       // debeser >= 7.  (5 de datos +2 de la hora)
#define MrPor       255     // indica asterisco
#define MrPorFecha  254     // indica registro de fecha
//#define MrNRegMax   10    // # Regs total (8000 para 24LC512)
#define MrNRegMax   100     // # Regs total (8000 para 24LC512)
#define MrGpoLen    32      // # bytes / grupo (pag, sector, etc). (128 para 24LC512)
bit MrInicializado = 0;

//#define MrLenCte 7              // debeser >= 7
//unsigned int MrNRegMax, MrGpoLen;
//unsigned int MrUltV;            // # ultimo registro escrito
//unsigned int MrDirV;            // # sgte reg a leer

int MrUltV;                     // # ultimo registro escrito
int MrDirV;                     // # sgte reg a leer

char MrReg [MrLen+5];           // Reg Len Max = MrLen+5 (0->(MrLen+5-1)
                                // necesario: MrLen + 2
char MrRegAux [MrLen+5];        // Reg Len Max = MrLen+5 (0->(MrLen+5-1)
//unsigned int MrFecha, MrHora;

unsigned int MrRegsXGpo;
unsigned int MrGpo;             // Grupo del registro
unsigned short MrNRegEnGpo;     // #Reg dentro del grupo
long MrRegPos;                  // Pos (#byte del 1er byte del registro

unsigned long MrFecha12;    // (AA<<16) + (MM<<8) + (DD<<1) + (0=AM/1=PM)
                            // fecha del registro/en este momento  
unsigned long MrFecha12Ult; // Ult Fecha del MemRoller
unsigned int MrHora12;      // H12 (bit 15-12), Min (bit 11-6), SS (bit 5-0)
//------------------------------------------------------------------------------
char RtcFechaHora[7];                   // "AAMMDDHHmmSSDS", [3]=HH=Hora(0-23)
//------------------------------------------------------------------------------
// Prototipos
//void RtcLee (); // Lee hora actual    // RTC -> RtcFechaHora
//void RtcEsc (); // Esc hora actual    // RtcFechaHora -> RTC
void MrSeparaFecha ();                  // RtcFechaHora -> MrFecha12, MrHora12
void MrBorraMem ();             // Borra TODA la memoria del mem roller

void RtcLee ();         // Lee hora actual       // RTC -> RtcFechaHora
void RtcEsc ();         // Esc hora actual       // RtcFechaHora -> RTC

//void MrFechaEsc (long MrFecha, short MrPos);
//long MrFechaLee (unsigned int MrPos);
//void MrHoraEsc();               // MrHora -> MrReg[0-1]
//void MrHoraLee();

void MrLeeReg ();
void MrUlt ();
void MrSgte ();
void MrAnt ();

void _MrPreparaPor ();
void _MrPreparaFecha ();
unsigned int _MrSgte (unsigned int MrDirX);

void _MrLee (unsigned int MrNReg, char* MrReg);
void _MrEsc (unsigned int MrNReg, char* MrReg);
void _CalculaRegPos (unsigned int MrNReg1);
//------------------------------------------------------------------------------
//-------------------------- RUTINAS -------------------------------------------
//------------------------------------------------------------------------------
;
void MrInicializa () {      // Init para pruebas
    unsigned short Ind;
//    MrPor = 255;            // Asterisco: byte1 = 0xFF
//    MrPorFecha = 254;       // Reg fecha: byte1 = 0xFE
//    MrLen = MrLenCte;       // 7 = long del registro (5 + 2(hora) )
//    MrNRegMax = 10;         // # Regs total
//    MrGpoLen = 20;          // # bytes / grupo
//    MrInicializado = 0;
    MrFecha12Ult = 0;                           // Inicialmente
    for (Ind = 0; Ind < MrLen; Ind++) { MrReg[Ind]=0; MrRegAux[Ind]=1; }
} // fin de MrInicializa

void MrFormat () {
    //unsigned int Ind;
    MrInicializado = 1;
    MrRegsXGpo = MrGpoLen / MrLen;
    //--------------------------------------------------------------------------
    #ifdef MrPrueba1        // Mem Roller en RAM
        MrBorraMem ();                  // Borra memoria de TODO el mem roller
    #endif  // MrPrueba1
    //--------------------------------------------------------------------------
    _MrPreparaPor ();                   // MrRegAux = "*"
    _MrEsc (MrNRegMax-1, MrRegAux);     // Escribe "*"
    _MrEsc (1, MrRegAux);               // Escribe "*"
    
    MrFecha12Ult = 0;                           // Inicialmente
    RtcLee ();  // Lee hora actual      // RTC -> RtcFechaHora;
    MrSeparaFecha ();                   // RtcFechaHora -> MrFecha12, MrHora12
    MrFecha12Ult = 0;                   // Inicialmente
    _MrPreparaFecha();                  // MrRegAux = RegFecha
    _MrEsc (0, MrRegAux);               // Escribe RegFecha

    MrFecha12Ult = MrFecha12;           // Inicialmente
    MrUltV = 0;
    MrDirV = 0;
} // Fin de MrFormat

//------------------------------------------------------------------------------
void MrInit () {
    MrFecha12Ult = 0;
    MrInicializado = 1;
    MrRegsXGpo = MrGpoLen / MrLen;
    for (MrDirV = 0; MrDirV < MrNRegMax; MrDirV++) {
        MrLeeReg ();
        if (MrReg[0] == MrPor) { MrUltV = MrDirV-1; break; }  // ???? -----------------------
        else if(MrReg[0]==MrPorFecha) {memcpy(&MrFecha12Ult,&MrReg[1],3);}
    } // for (MrDirV
    if (MrDirV == 0) {          // '*' en reg 0
        MrDirV = MrNRegMax-1;
        MrLeeReg();
        if (MrReg[0] == MrPor) { MrUltV = MrNRegMax-2; }
        else                   { MrUltV = MrNRegMax-1; }
    }
    if (MrDirV >= MrNRegMax) {  // si no encontr� "*"
        _MrPreparaPor ();
        _MrEsc (MrNRegMax-1, MrRegAux);     // Escribe "*" en MrNRegMax-1
        MrUltV = MrNRegMax-2;
    }   // if (MrDirV >=
    
    if (MrFecha12Ult == 0) {        // si no ha encontrado Fecha: la busca
        for (MrDirV = MrNRegMax-1; MrDirV >= 0; MrDirV--) {
            MrLeeReg ();
            if (MrReg[0] == MrPor) {break;}     // si "*", termina
            if (MrReg[0] == MrPorFecha) {  // si fecha, la toma
                memcpy(&MrFecha12Ult, &MrReg[1], 3);
                break;
            }
        }   // for (MrDirV  .
    }   // fin de si no ha encontrado fecha.
    
    MrDirV = MrUltV;
} // Fin de MrInit
//------------------------------------------------------------------------------
void MrAgregaReg (){    // Si hay cambio de Fecha, agregar reg de fecha
                        // Esc Asterisco (MrPor), Esc MrReg
    unsigned int MrTemp;
    RtcLee ();                          //Rtc = 1307
    MrSeparaFecha();                    // RtcFechaHora -> MrFecha12, MrHora12
    MrUltV = _MrSgte (MrUltV);
    MrTemp = _MrSgte (MrUltV);
    if (MrFecha12 != MrFecha12Ult) {    // Esc fecha si es diferente
        _MrPreparaPor ();               // MrRegAux = "*"
        _MrEsc (MrTemp, MrRegAux);      // Escribe "*"
        _MrPreparaFecha();              // MrRegAux = RegFecha
        _MrEsc (MrUltV, MrRegAux);      // Escribe RegFecha
        MrUltV = MrTemp;
        MrTemp = _MrSgte (MrUltV);
        MrFecha12Ult = MrFecha12;
    }
    if ( (MrUltV == 0) ) {              // fecha en reg 0
        _MrPreparaPor ();               // MrRegAux = "*"
        _MrEsc (MrTemp, MrRegAux);      // Escribe "*"
        _MrPreparaFecha();              // MrRegAux = RegFecha
        _MrEsc (MrUltV, MrRegAux);      // Escribe RegFecha
        MrUltV = MrTemp;
        MrTemp = _MrSgte (MrUltV);
    }
    _MrPreparaPor ();                   // MrRegAux = "*"
    _MrEsc (MrTemp, MrRegAux);          // Escribe "*"
    memcpy(&MrReg[0], &MrHora12, 2);
    _MrEsc (MrUltV, MrReg);             // Escribe Registro agregado
}   // fin de MrAgregaReg
        
void _MrPreparaPor () {                 // Asterisco en RegAux
    unsigned short Ind;
    for (Ind = 1; Ind < MrLen; Ind++) { MrRegAux[Ind]=0xF0; }
    MrRegAux [0] = MrPor;               // MrRegAux = "*"
}   // _MrPreparaPor

void _MrPreparaFecha () {               // Fechas en RegAux
    unsigned short Ind;
    for (Ind = 1; Ind < MrLen; Ind++) { MrRegAux[Ind]=0xF1; }
    MrRegAux [0] = MrPorFecha;          // MrRegAux = RegFecha
    memcpy (&MrRegAux[1], &MrFecha12, 3);     // Fecha Nueva
    memcpy (&MrRegAux[4], &MrFecha12Ult, 3);  // Fecha Anterior
}   // _MrPreparaFecha


void MrLeeReg (){
    _MrLee (MrDirV, MrReg);
}   // fin de MrLeeReg

void MrUlt (){
    MrDirV = MrUltV;
}   // fin de MrUlt

void MrSgte (){
    MrDirV = _MrSgte (MrDirV);
}   // fin de MrSgte

unsigned int _MrSgte (unsigned int MrDirX){
    MrDirX ++;
    if (MrDirX >= MrNRegMax) { return (0); }
    return (MrDirX);
}   // fin de _MrSgte

void MrAnt (){
    MrDirV --;
    if (MrDirV < 0) { MrDirV = MrNRegMax-1; }
}   // fin de MrAnt
// **/

//------------------------------------------------------------------------------
void _CalculaRegPos (unsigned int MrNReg1) {
    MrGpo = MrNReg1 / MrRegsXGpo;
    MrNRegEnGpo = MrNReg1 % MrRegsXGpo;
    MrRegPos = (MrGpo * MrGpoLen) + (MrNRegEnGpo * MrLen);
}   // fin de CalculaPosReg
//------------------------------------------------------------------------------
//void MrFechaEsc (long MrFecha12, short MrPos) {     // Esc fecha a MrRegAux
//    // MrFecha -> MrRegFecha [MrPos -> (MrPos+2)]
//    memcpy (&(MrRegAux[MrPos]), &MrFecha12, 3);
//}   // Fin de MrFechaEsc
//
//long MrFechaLee (unsigned int MrPos){               // Lee fecha de MrReg
//    // Uso: Fecha = MrFechaLee (MrPos)
//    // MrReg [MrPos -> (MrPos+2)] -> return(Fecha)
//    long MrFecha1;
//    memcpy (&MrFecha12, &(MrReg[MrPos]), 3);
//    return (MrFecha1);
//}   // Fin de MrFechaLee
//
//void MrHoraEsc(){               // MrHora -> MrReg [0-1]
//    memcpy (&(MrReg[0]), &MrHora, 2);
//} //Fin de MrHoraEsc
//
//void MrHoraLee(){               // MrReg [0-1] -> MrHora
//    memcpy (&MrHora, &(MrReg[0]), 2);
//} //Fin de MrHoraLee
//------------------------------------------------------------------------------
void MrSeparaFecha() {          // RtcFechaHora -> MrFecha12, MrHora12
    unsigned short MrAmPm;
    MrHora12 = RtcFechaHora[3] & 0x1F;                  // Hora (0-23)
    if (MrHora12 < 12) {MrAmPm = 0;}
    else               {MrAmPm = 1; MrHora12 -= 12;}    // Hora12 (0-11)
    MrFecha12 = RtcFechaHora[0];    // AA MM DD am/pm           // anio
    MrFecha12 = (MrFecha12<<8) + RtcFechaHora[1];               // mes
    MrFecha12 = (MrFecha12<<8) + (RtcFechaHora[2]<<1) + MrAmPm; // dia + am/pm
    MrHora12 = (( (MrHora12<<6)+RtcFechaHora[4] ) <<6) +RtcFechaHora[5];//min,seg
}
//------------------------------------------------------------------------------
// Prueba de Memory Roller en Mem RAM
#ifdef MrPrueba1
#endif  // MrPrueba1
//------------------------------------------------------------------------------
#ifdef MrPrueba1
#define  MrMemMax    100
char MrMem [MrMemMax];

void MrBorraMem () {                   // Borra memoria de TODO el mem roller
    unsigned int Ind;
    for (Ind = 0; Ind < MrMemMax; Ind++) { MrMem[Ind] = 0; }
} // fin de MrBorraMem

void _MrLee (unsigned int MrNReg, char* MrReg){
    unsigned short Ind;
    _CalculaRegPos (MrNReg);
    for (Ind = 0; Ind < MrLen; Ind++) {
        MrReg [Ind] = MrMem [MrRegPos + Ind];
    }
}   // fin de _MrLee

void _MrEsc (unsigned int MrNReg, char* MrReg){
    unsigned short Ind;
    _CalculaRegPos (MrNReg);
    for (Ind = 0; Ind < MrLen; Ind++) {
        MrMem [MrRegPos + Ind] = MrReg [Ind];
    }
}   // fin de _MrEsc
#endif  // MrPrueba1
//------------------------------------------------------------------------------
#ifndef MrPrueba1   // No RAM 

void _MrLee (unsigned int MrNReg, char* MrReg1){
    //unsigned short Ind;
    _CalculaRegPos (MrNReg);
    MseLee (MrRegPos, MrReg1, MrLen);
}   // fin de _MrLee

void _MrEsc (unsigned int MrNReg, char* MrReg1){
    //unsigned short Ind;
    long HiCars;
    _CalculaRegPos (MrNReg);
    MseEsc (MrRegPos, MrReg1, MrLen);
    memcpy (&HiCars, &MrReg1[0], 4);
    HiRegistra (HiCars,MrRegPos);
}   // fin de _MrEsc
#endif  // MrPrueba1
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
