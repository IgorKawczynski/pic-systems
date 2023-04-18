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
void power_select(int num) //wybór mocy mikrofali
{
    char* power[4]={"800W","600W","350W","200W"};
    lcd_cmd(L_L2); //Ustawienie karetki w pierwszej linii
    lcd_str("Moc:   ");
    lcd_str(power[num]); // wyswietlenie odpowiedniej mocy
    lcd_str("     ");
    delay(600);

}
void time_select(int minute, int seconds)//ustalenie czasu
{   
    char* number[10]={"0","1","2","3","4","5","6","7","8","9"};//liczby do ustawienia czasu
    int tenm; //zmienna odpowiedzialna za dziesiatki minut
    int tens; //zmienna odpowiedzialna za dziesiatki sekund
    
    
    tenm=minute/10;//ustalenie wartosci dziesiatek minut
    tens=seconds/10;//ustalenie wartosci dziesiatek sekund
    lcd_cmd(L_L1);//Ustawienie karetki w pierwszej linii
    lcd_str("Czas:   ");//tekst
    lcd_str(number[tenm]);//wyswietlenie dziesiatek czasu
    lcd_str(number[minute-(tenm*10)]);//wyswietlenie jednostek czasu
    lcd_str(":");//tekst
    lcd_str(number[tens]);//wyswietlenie dziesiatek sekund
    lcd_str(number[seconds-(tens*10)]);//wyswietlenie jednostek czasu
    lcd_str("      ");
    
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
    int powernum =0; //zmienna odpowiedzialna za wybór mocy
    int minute=0; //zmienna odpowiedzlna za ustawienie minut
    int seconds=0; //zmienna odpowiedzlna za ustawienie sekund
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
    lcd_str("                    ");
    int start=0; //zmienna odpowiedzialna za start/stop
    while(1)//nieskonczona petla
    {
        if(PORTBbits.RB5 == 0) //po uzyciu przycisku RB5 wyswietli moc i przelaczy ja
        {
            power_select(powernum);//funkcja zmiany i wyswietlenia mocy
            powernum++;
            if(powernum==4){
                powernum=0;
            }
        }
        if(PORTBbits.RB4 == 0) //po uzyciu przycisku RB4 wyswietli czas ktory zostanie zwiekszony o 1 minute
        {
            minute++; //zwiekszenie minut
            if(minute>99){ //po przekroczeniu 99minut restartuje minuty
                minute=1;
            }
            time_select(minute, seconds); //funkcja
            delay(600);
        }
        if(PORTBbits.RB3 == 0)//po uzyciu przycisku RB3 wyswietli czas ktory zostanie zwiekszony o 10 sekund
        {
            seconds+=10; //zwiekszenie sekund
            if(seconds==60){ //po przekroczeniu 60sekund dodaje jedna minute
                if(minute<99){
                    seconds=0;
                    minute++;
                }
                else{
                    seconds=0;
                    minute=1;
                }
            }
            time_select(minute, seconds);//funkcja
            delay(600);
        }
        if(PORTBbits.RB2 == 0)// start/stop
        {
            if(start==0){
                delay(100);
                start=1;
            }
        }
        
        
        if(PORTBbits.RB1 == 0) //restart
        {
            minute=0;
            seconds=0;
            powernum=0;
            lcd_cmd(L_CLR); //Czyszczenie wyswietlacza
            lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
            lcd_str("                    ");
            delay(600);
        } 
        if(start==1){     //dzia?a dopoki znowu sie nie uzyje przycisku
            if(minute+seconds>0){
                if(seconds>0) // sprawdza czy sekundy maja wartosc
                {
                    seconds--; //odejmuje sekunde
                }
                else// jesli sekundy nie maja wartosciu to odejmuje jedna minute i ustawia 59sekund
                {
                    minute--;
                    seconds=59;
                } 
                time_select(minute, seconds); //funkcja
            }
            else{
                delay(1000);
                start=0;//wylaczenie startu
                time_select(minute, seconds); //funkcja
                lcd_cmd(L_CLR); //Czyszczenie wyswietlacza
                lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
                lcd_str("                    ");
            }
            for(int i=0;i<=1000;i++)//przez sekunde sprawdza przycisk stop/start
            {
                delay(1);
                if(PORTBbits.RB2 == 0)// start/stop
                 {
                    start=0;
                 }
            }
        }
    }
}