//Rutinas para registrar historial 
//------------------------------------------------------------------------------
// llamar HiBorra () para borrar el historial. (comando 20)
// llamar HiRegistra (HiLong, HiInt), para registrar estos datos
//------------------------------------------------------------------------------
#define HiInicio    0x2000
#define HiMax       0x2400-1
int HiCont=0;

void HiBorra () {
    char HiDatos [10];
    int HiInd;
    HiCont=0;
    for (HiInd = 0; HiInd<8; HiInd++) {HiDatos[HiInd] = 0;}

    for (HiInd=HiInicio; HiInd<=HiMax+1; HiInd+=8) {
        MseEsc (HiInd, HiDatos, 8);
    }
}

void HiRegistra (long HiLong, int HiInt) {
    char HiDatos [20];
    int HiInd, HiDir;
    HiDir = HiInicio + (HiCont*8);
    if ( (HiDir < HiInicio) || (HiDir > HiMax) ) {
        memcpy (&HiDatos[0], &HiCont, 2) ;
        memcpy (&HiDatos[2], &HiLong, 4) ;
        memcpy (&HiDatos[6], &HiInt, 2) ;
        memcpy (&HiDatos[8], &HiDir, 2) ;
        MseEsc (HiMax+1, HiDatos, 10);
        LedParpadea (4);
    }
    memcpy (&HiDatos[0], &HiCont, 2) ;
    memcpy (&HiDatos[2], &HiLong, 4) ;
    memcpy (&HiDatos[6], &HiInt, 2) ;
    MseEsc (HiDir, HiDatos, 8);
    
    HiCont ++;
} 

