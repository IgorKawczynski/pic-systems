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
void CustomCharacter (unsigned char *Pattern, const char Location)
{ 
int i=0; 
lcd_cmd(0x40+(Location*8));
for (i=0; i<8; i++)
lcd_dat (Pattern[i] );
}
unsigned char heart[] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0,0x0};//niestandardowy znak
unsigned char sad[] = {0x0,0xa,0xa,0x0,0xe,0x11,0x0,0x0};//niestandardowy znak
unsigned char happy[] = {0x0,0xa,0xa,0x0,0x11,0xe,0x0,0x0};//niestandardowy znak



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
    CustomCharacter(heart,1);//niestandardowy znak
    CustomCharacter(sad,2);//niestandardowy znak
    CustomCharacter(happy,3);//niestandardowy znak
    unsigned int i;
    char text[]="up z nietoperzy w chinskich restauracjach???";
    while(1)//nieskonczona petla
    {
       delay(1000);
       lcd_cmd(L_CLR); //Czyszczenie wyswietlacza
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str(" Masz juz dosc? "); //napis
       delay(800); //opoznienie
       lcd_cmd(L_L2); //Przejscie do drugiej linii
       lcd_str("    "); //napis
       lcd_dat(2); //wstawienie specjalnego znaku
       lcd_str("      ");
       lcd_dat(2);
       lcd_str("    "); 
       delay(2500);
       
       lcd_cmd(L_CLR); 
       lcd_cmd(L_L1); 
       lcd_str("Zwyklego nudnego"); 
       lcd_cmd(L_L2); 
       lcd_str("  jedzenia???   ");
       delay(2500);
       
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1);
       lcd_str("Tych samych dan "); 
       lcd_cmd(L_L2);
       lcd_str("   u matki???   ");
       delay(2500);
       
       lcd_cmd(L_CLR); 
       lcd_str("Zarcia nudnych z"); 
       delay(500); 
       i=0;
       while(text[i]!='\0'){//tekst wymaga podzielania na czesci zeby wyswietlic w jednej lini
           if(i==24){
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("toperzy w chinsk");
            }  
            lcd_cmd(0x18); //przesuniecie w prawo
 			lcd_dat(text[i]);//wstawienie znaku z tablicy
            
 			delay(100);
 			i++; 
		}

       delay(1000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); 
       lcd_str("   PAPRYKARZ    "); 
       lcd_cmd(L_L2); 
       lcd_str("  SZCZECINSKI   ");
       
       delay(1000); 
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); 
       lcd_dat(1); 
       lcd_dat(1); 
       lcd_dat(1); 
       lcd_str("PAPRYKARZ"); 
       lcd_dat(1); 
       lcd_dat(1); 
       lcd_dat(1);
       lcd_str(" ");
       lcd_cmd(L_L2);
       lcd_dat(1); 
       lcd_dat(1);
       lcd_str("SZCZECINSKI");
       lcd_dat(1);
       lcd_dat(1); 
       lcd_str(" ");
       
       delay(1500);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); 
       lcd_str("    Oto twoje   "); 
       lcd_cmd(L_L2);
       lcd_str("   rozwiazanie  ");
       
       delay(1500);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1);
       lcd_str("     ");
       lcd_dat(3);
       lcd_str("   ");
       lcd_dat(3);
       lcd_str("      ");
       lcd_cmd(L_L2);
       lcd_str("     ");
       lcd_dat(3);
       lcd_str("   ");
       lcd_dat(3);
       lcd_str("      ");
       delay(1000);
       lcd_cmd(L_CLR);
    }
    return;
}
