// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF

#include <xc.h>

#define LENA  PORTEbits.RE1
#define LDAT  PORTEbits.RE2
#define LPORT PORTD

#define L_ON	0x0F
#define L_OFF	0x08
#define L_CLR	0x01
#define L_L1	0x80
#define L_L2	0xC0		
#define L_CFG   0x38


void delay(unsigned int ms)//opoznienie
{
    unsigned int i;
    unsigned char j;
    
 for (i =0; i< ms; i++)
 {
 
  for (j =0 ; j < 200; j++)
   {
      Nop();
      Nop();
      Nop();
      Nop();
      Nop();
   }
 }
}
unsigned int adc()
{
ADCON0=0x05; //Potencjometr P2
ADCON0bits.GO=1;
while(ADCON0bits.GO == 1);
int vl1=(((ADRESL>>6)+1)*8)>>4; //ustawienie 3 mozliwosci przekrecenia
return (vl1+3); //poprawienie tych wartosci
}
void lcd_wr(unsigned char val)
{
  LPORT=val;
}

void lcd_cmd(unsigned char val)//funkcja do wysylania wartosci do rejestratora komend LCD
{
	LENA=1;
    lcd_wr(val);
    LDAT=0;
    LENA=0;
	LENA=1;
}
 
void lcd_dat(unsigned char val)//funkcja wyswietlajaca jeden znak
{
	LENA=1;
    lcd_wr(val);
    LDAT=1;
    LENA=0;
	LENA=1;
}

void lcd_init(void)//funkcja do inicjalizacji rejestru i pinow LCD
{
	LENA=0;
	LDAT=0;
	delay(20);
	LENA=1;
	
	lcd_cmd(L_CFG);
	delay(5);
	lcd_cmd(L_CFG);
    delay(1);
	lcd_cmd(L_CFG);
	lcd_cmd(L_OFF);
	lcd_cmd(L_ON);
	lcd_cmd(L_CLR);
	lcd_cmd(L_CFG);
    lcd_cmd(L_L1);
}

void lcd_str(const char* str)//funkcja do wyswietlenia calego zdania
{
 unsigned char i=0;
      
 while(str[i] != 0 )
 {
   lcd_dat(str[i]);
   i++;
 }  
}

void time(int minute1, int seconds1, int minute2, int seconds2)//licznik czasu
{   
    char* number[10]={"0","1","2","3","4","5","6","7","8","9"};//liczby do ustawienia czasu
    int tenm1; //zmienna odpowiedzialna za dziesiatki minut gracza1
    int tens1; //zmienna odpowiedzialna za dziesiatki sekund gracza1
    int tenm2; //zmienna odpowiedzialna za dziesiatki minut gracza2
    int tens2; //zmienna odpowiedzialna za dziesiatki sekund gracza2
    
    tenm1=minute1/10;//ustalenie wartosci dziesiatek minut gracza1
    tens1=seconds1/10;//ustalenie wartosci dziesiatek sekund gracza1
    tenm2=minute2/10;//ustalenie wartosci dziesiatek minut gracza2
    tens2=seconds2/10;//ustalenie wartosci dziesiatek sekund gracza2
    lcd_cmd(L_L1);//Ustawienie karetki w pierwszej linii
    lcd_str("Gracz1    Gracz2");//tekst
    lcd_cmd(L_L2);//przejscie do drugiej lini
    lcd_str(number[tenm1]);//wyswietlenie dziesiatek czasu dla gracza1
    lcd_str(number[minute1-(tenm1*10)]);//wyswietlenie jednostek czasu dla gracza1
    lcd_str(":");//tekst
    lcd_str(number[tens1]);//wyswietlenie dziesiatek sekund dla gracza1
    lcd_str(number[seconds1-(tens1*10)]);//wyswietlenie jednostek czasu dla gracza1
    lcd_str("      ");//tekst
    lcd_str(number[tenm2]);//wyswietlenie dziesiatek czasu dla gracza2
    lcd_str(number[minute2-(tenm2*10)]);//wyswietlenie jednostek czasu dla gracza2
    lcd_str(":");//tekst
    lcd_str(number[tens2]);//wyswietlenie dziesiatek sekund dla gracza2
    lcd_str(number[seconds2-(tens2*10)]);//wyswietlenie jednostek czasu dla gracza2
    
}

void main(void) {
    
    //Inicjalizacja konwertera analogowo cyfrowego
    ADCON0=0x01;
    ADCON1=0x0B;
    ADCON2=0x01;
    
    TRISA=0xC3;
    TRISB=0x3F;   
    TRISC=0x01;
    TRISD=0x00;
    TRISE=0x00;
    
    lcd_init(); //Inicjalizacja wyswietlacza
    lcd_cmd(L_CLR); //Czyszczenie wyswietlacza
    int minute=0; //zmienna odpowiedzlna za ustawienie czasu dla obu graczy
    int minutep1=0; //zmienna odpowiedzlna za ustawienie minut dla gracza1
    int minutep2=0; //zmienna odpowiedzlna za ustawienie minut dla gracza2
    int secondsp1=0; //zmienna odpowiedzlna za ustawienie sekund dla gracza1
    int secondsp2=0; //zmienna odpowiedzlna za ustawienie sekund dla gracza2
    unsigned char display = 0;
    unsigned int tmp = 0;
    int end=0; //zmienna konczaca wybor czasu
    while(1)//nieskonczona petla do ustawienia czasu gry
    {    
        char* number[10]={"0","1","2","3","4","5","6","7","8","9"};//liczby do ustawienia czasu
        tmp=((unsigned int)adc());//odczytanie potencjometru
        switch(tmp) // zamiana danych zeby byly do wyboru tylko 3,1,5
        {
            case 3: //ustawienie 3 minut
                display = 3;
                minute=display;
                minutep1=minute;
                minutep2=minute;
                break;
            case 4: //ustawienie 1 minuty
                display = 1;
                minute=display;
                minutep1=minute;
                minutep2=minute;
                break;
            case 5: //ustawienie 5 minut
                display = 5;
                minute=display;
                minutep1=minute;
                minutep2=minute;
                break;
        }
        int tenm; //zmienna odpowiedzialna za dziesiatki minut gry
        tenm=minute/10;//ustalenie wartosci dziesiatek minut gry
        lcd_cmd(L_L1);//Ustawienie karetki w pierwszej linii
        lcd_str("Czas: ");//tekst
        lcd_str(number[tenm]);//wyswietlenie dziesiatek czasu
        lcd_str(number[minute-(tenm*10)]);//wyswietlenie jednostek czasu
        lcd_str(":00      ");//tekst
        
        //przycisk jednego z graczy spowoduje akceptacje czasu
        for(int i=0;i<=10;i++) //przez 10milisekund sprawdza przycisk
        {//przycisk akceptuje czas
            if(PORTBbits.RB3 == 0)
            {
                end=1;
            }
            if(PORTBbits.RB5 == 0)
            {
                end=1;
            }
            delay(1);
        }
        
        if(end==1)
        {
            break;
        }
    }
    lcd_cmd(L_L2); 
    lcd_str("Start: przycisk ");//tekst
    delay(100);
    
    // wybór który gracz zaczyna
    while(1)
    {//po kliknieciu guzika gracza ten gracz zaczyna
        if(PORTBbits.RB3 == 0)//przycisk pierwszego gracza
        {
            delay(500);
            goto timep1;//przejscie do odliczania czasu gracza 1
        }
        if(PORTBbits.RB5 == 0)//przycisk drugiego gracza
        {
            delay(500);
            goto timep2;//przejscie do odliczania czasu gracza 2
        }
    }
    
    timep2://odliczanie czasu gracza2  i przejscie do sprawdzenia przycisku
        while(1)//nieskonczona petla
        {
            if(minutep2+secondsp2>0){
                if(secondsp2>0) // sprawdza czy sekundy maja wartosc
                {
                    secondsp2--; //odejmuje sekunde
                }
                else// jesli sekundy nie maja wartosciu to odejmuje jedna minute i ustawia 59sekund
                {
                    minutep2--; //odejmuje minute
                    secondsp2=59; //ustawia sekundy
                } 
                goto buttonp2;//przejscie do sprawdzenia przycisku gracza2
            }
            else{ //jesli nie ma czasu
                goto losep2; //przejscie do komunikatu o przegranej gracza 2
            }
        }
    
    
    timep1://odliczanie czasu gracza1 i przejscie do sprawdzenia przycisku
        while(1)//nieskonczona petla
        {
            if(minutep1+secondsp1>0){
                if(secondsp1>0) // sprawdza czy sekundy maja wartosc
                {
                    secondsp1--; //odejmuje sekunde
                }
                else// jesli sekundy nie maja wartosciu to odejmuje jedna minute i ustawia 59sekund
                {
                    minutep1--; //odejmuje minute
                    secondsp1=59; //ustawia sekundy
                } 
                goto buttonp1; //przejscie do sprawdzenia przycisku gracza2
            }
            else{ //jesli nie ma czasu
                goto losep1; //przejscie do komunikatu o przegranej gracza 1
            }
        }
    
    
    buttonp2: // wyswietla czas i sprawdza uzycie przycisku gracza2
              // jesli gracz2 nie uzyje przycisku dalej odlicza jego czas
              // jesli gracz2 uzyje przycisku przejscie do odliczania czasu gracza1
        time(minutep1, secondsp1, minutep2, secondsp2); //wyswietlenie czasu
        for(int j=0;j<=1000;j++) //przez sekunde sprawdza przycisk
        {
            delay(1);
            if(PORTBbits.RB5 == 0)//przycisk pierwszego gracza
            {   
                //minutep2=minute; restart czasu(nie jestem pewny czy czas ma sie resetowac po wykonaniu ruchu)
                //secondsp2=0;
                goto timep1; // przejscie do odliczania czasu gracza2
            }
        }    
         
        goto timep2; // przejscie do odliczania czasu gracza2
    
    
    buttonp1: // wyswietla czas i sprawdza uzycie przycisku gracza1
              // jesli gracz1 nie uzyje przycisku dalej odlicza jego czas
              // jesli gracz1 uzyje przycisku przejscie do odliczania czasu gracza2
            time(minutep1, secondsp1, minutep2, secondsp2); //wyswietlenie czasu
            for(int j=0;j<=1000;j++) //przez sekunde sprawdza przycisk
            {
                delay(1);
                if(PORTBbits.RB3 == 0)//przycisk pierwszego gracza
                {
                    //minutep1=minute; restart czasu(nie jestem pewny czy czas ma sie resetowac po wykonaniu ruchu)
                    //secondsp1=0;
                    goto timep2; // przejscie do odliczania czasu gracza2
                }
            }
            goto timep1; // przejscie do odliczania czasu gracza1
    
    
    losep2: // komunikat o przegranej gracza 2
        lcd_cmd(L_CLR); //Czyszczenie wyswietlacza
        lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
        lcd_str("Gracz 2 przegral");
        lcd_cmd(L_L2); //Przejscie do drugiej lini
        lcd_str("gre przez czas!!");
        delay(10000);
        return;//po 10 sekundach resetuje gre
    
    
    losep1: // komunikat o przegranej gracza 2
        lcd_cmd(L_CLR); //Czyszczenie wyswietlacza
        lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
        lcd_str("Gracz 1 przegral");
        lcd_cmd(L_L2); //Przejscie do drugiej lini
        lcd_str("gre przez czas!!");
        delay(10000);
        return;//po 10 sekundach resetuje gre
}