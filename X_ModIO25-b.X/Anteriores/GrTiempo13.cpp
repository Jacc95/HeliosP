// Rutinas de Tiempo para Graficas Version 1.3         2017-04-24
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// -------------------------- RUTINAS DE TIEMPO --------------------------------
//------------------------------------------------------------------------------
// R U T I N A S :
// TpoInit:             // void TpoInit ()           // inicializa 
// Fijo2Tpo:            // unsigned long Fijo2Tpo () {     // Segs desde el 5 de abril de 2017, 15:30:12
// FechaHora12ToTpo:    // unsigned long FechaHora12ToTpo () {     // Fecha12:Hora12 -> Tpo
                        // FechaHora= { [0]=year/[1]=mes/[2]=diames/[3]=HH(0-23)/[4]=Min/[5]=Seg }
// Tpo2FechaHora        // void Tpo2FechaHora ()     // Tpo -> FechaHora [ [0]=year, ..., [5]=Seg }
                        // FechaHora= { [0]=year/[1]=mes/[2]=diames/[3]=HH(0-23)/[4]=Min/[5]=Seg }
// Tpo2SemanaHora       // void Tpo2SemanaHora (int* SemanaP, char* SemanaHora) {  
                        // Tpo -> (Semana, SemanaHora) {[0]=dweek,[1]=hora,[2]=min,[3]=Seg}
// PruebaTime           // PruebaTime ()        // Prueba rutinas
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// -------------------------- RUTINAS DE TIEMPO --------------------------------
//------------------------------------------------------------------------------
// void TpoInit () ;
// unsigned long Fijo2Tpo ();     // Segs desde el 5 de abril de 2017, 15:30:12
// unsigned long FechaHora12ToTpo ();     // Fecha12:Hora12 -> Tpo
// void Tpo2FechaHora ();         // Tpo -> FechaHora [ [0]=year, ..., [5]=Seg }
// void Tpo2SemanaHora (int* SemanaP, char* SemanaHora);
//        // Tpo -> (Semana, SemanaHora) {[0]=dweek,[1]=hora,[2]=min,[3]=Seg}
//        // dias desde Tpo_Ref (1/ene/2012)
//------------------------------------------------------------------------------
unsigned long Tpo;              // #seg desde 1/ene/2012 (domingo)
struct tm Hora_tm, tm_Ref;      // Utilizado por rutinas de tiempo: mktime, loccaltime
char Fecha12[3];                // ('dddd dddP') (mes) (anio), P=AmPm (0=AM,1=PM)
unsigned int Hora12;            // 'HHHH MMMM MM SS SSSS' (4bits=Hora(0-11), 6b=Min, 6b=Seg
char FechaHora[6];              // AA,MM,Dmes,HH,Min,Seg.  [0]=AA(17=2017), ..., [6]=Seg

time_t Tpo_Ref;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TpoInit () {
    tm_Ref.tm_year = 112;
    //tm_Ref.tm_year = 70;        ///////////
    tm_Ref.tm_mon = 0;
    tm_Ref.tm_mday = 1;
    tm_Ref.tm_hour = 0;
    tm_Ref.tm_min = 0;
    tm_Ref.tm_sec = 0;
    tm_Ref.tm_isdst = 0;
    
    Tpo_Ref = mktime (&tm_Ref);  // time_t desde 1/ene/2012 (domingo)
        // se escoge domingo para que las semanas vayan de domingo a sabado.
} // TpoInit
//------------------------------------------------------------------------------
unsigned long Fijo2Tpo () {     // Segs desde el 5 de abril de 2017, 15:30:12
    struct tm tm_4;
    time_t Tpo_time_t;
    unsigned long Tpo1;
    double TpoDouble;

    tm_4.tm_year = 117;             // 5 de abril de 2017, 15:30:12
    tm_4.tm_mon = 3;                // abril = 4-1 = 3
    tm_4.tm_mday = 5;
    tm_4.tm_hour = 15;
    tm_4.tm_min = 30;
    tm_4.tm_sec = 12;
    
    tm_4.tm_isdst = 0;              // Supone no hay horario de verano

    Tpo_time_t = mktime (&tm_4);    // time_t desde 1/ene/2012 (domingo)
    printf ("Tpo_time_t = %15d\n",Tpo_time_t);
    TpoDouble = difftime (Tpo_time_t, Tpo_Ref);
//    printf ("tamanio de TpoDouble = %5d\n",sizeof (TpoDouble));

    printf ("TpoDouble = %15.12e\n",TpoDouble);
    Tpo1 = (unsigned long) TpoDouble;
    return (Tpo1);
} // Fijo2Tpo
//------------------------------------------------------------------------------
unsigned long FechaHora12ToTpo () {     // Fecha12:Hora12 -> Tpo
    short AmPm;                 // 0=AM, 1=PM
    struct tm tm_1;
    double TpoDouble;           // seg desde Tpo_Ref
    tm_1.tm_year = (int)Fecha12[2]+100;     // year = anio - 1900
    tm_1.tm_mon =  Fecha12[1]-1;
    tm_1.tm_mday = Fecha12[0] >> 1;
    AmPm = Fecha12[0] & 1;
    tm_1.tm_hour =  (Hora12 >> 12) + (AmPm*12);
    tm_1.tm_min = ( (Hora12 >>  6) & 0x3F );
    tm_1.tm_sec = ( (Hora12      ) & 0x3F );
    
    TpoDouble = difftime (mktime(&tm_1), Tpo_Ref);
    // TpoDuble = 4010000000;       // debe marcar error
    // TpoDuble = -1;               // debe marcar error
    if ( (TpoDouble < 0) || (TpoDouble > 4E9) ) {
        printf ("\nHora2Tpo:  Tpo fuera de rango: %E\n", TpoDouble);
        system ("pause");
        TpoDouble = 0;
    }
    return ((unsigned long) TpoDouble);
}   // FechaHora12ToTpo
//------------------------------------------------------------------------------
void Tpo2FechaHora () {         // Tpo -> FechaHora [ [0]=year, ..., [5]=Seg }
        // FechaHora= { [0]=year/[1]=mes/[2]=diames/[3]=HH(0-23)/[4]=Min/[5]=Seg }
    struct tm * tm_1P;
    time_t Tpo_time_t;

    Tpo_time_t = Tpo + Tpo_Ref;         // Tpo = segs desde Tpo_Ref
    tm_1P = localtime (&Tpo_time_t);

    FechaHora [0] = (* tm_1P).tm_year - 100;
    FechaHora [1] = (* tm_1P).tm_mon+1;
    FechaHora [2] = (* tm_1P).tm_mday;
    FechaHora [3] = (* tm_1P).tm_hour;
    FechaHora [4] = (* tm_1P).tm_min;
    FechaHora [5] = (* tm_1P).tm_sec;
} // Tpo2FechaHora
//------------------------------------------------------------------------------
void Tpo2SemanaHora (int* SemanaP, char* SemanaHora) {  
        // Tpo -> (Semana, SemanaHora) {[0]=dweek,[1]=hora,[2]=min,[3]=Seg}
    unsigned int Dia=Tpo / 86400;       // dias desde Tpo_Ref (1/ene/2012)
    unsigned int Segs = Tpo % 86400;
    SemanaHora [0] = Dia % 7;
    SemanaHora [1] = Segs / 3600;
    SemanaHora [2] = (Segs / 60) % 60;
    SemanaHora [3] = Segs % 60;
    *SemanaP = Dia / 7;
} // Tpo2SemanaHora
//------------------------------------------------------------------------------
void PruebaTime () {
    time_t T1;
    unsigned long Tpo1, Tpo2, Tpo3;

    TpoInit();
    printf ("tamanio = %5d\n",sizeof (T1));
    Tpo1 = Tpo_Ref;
//    printf ("Tpo1 = %15.12d\n",Tpo1);
    printf ("Tpo1    = %15d\n",Tpo1);
    printf ("Tpo_Ref = %15d\n",Tpo_Ref);
    struct tm tm_2;
    struct tm * tm_3P;
    tm_3P = localtime (&Tpo_Ref);
    tm_2 = * tm_3P;
    printf ("dweek = %5d\n",tm_2.tm_wday);

    Fecha12 [0] = (5 * 2) + 1;          // dia 5, pm
    Fecha12 [1] = 4;                    // abril
    Fecha12 [2] = 17;                   // 2017
    Hora12 = (3<<12) + (30<<6) + (12);  // 3pm : 30min : 12seg.   (15:30:12)
    Tpo3 = FechaHora12ToTpo ();
    printf ("Tpo3 = %15d\n",Tpo3);

    Tpo2 = Fijo2Tpo ();                 // Segs desde el 5 de abril de 2017, 15:30:12
    printf ("Tpo2 = %15d\n",Tpo2);
    system ("pause");
} // PruebaTime
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
