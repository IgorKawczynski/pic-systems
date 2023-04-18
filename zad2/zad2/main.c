#include <p24fj128ga010.h>

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF)
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI)

#define SCALE 308L

/* numer którym dostajemy siê do czujnika temperatury */

#define TSENS 4
#define AINPUTS 0xffcf
#define MAXTEMP 250 //Ustawienie maksymalnej temperatury
int temp, temp2;
void ADCinit(int amask)

{
	AD1PCFG = amask;
	AD1CON1 = 0x00e0;
	AD1CSSL = 0;
	AD1CON2 = 0;
	AD1CON3 = 0x1f02;
	AD1CON1bits.ADON = 1;
}

int readADC(int ch)

{
	AD1CHS = ch;
	AD1CON1bits.SAMP = 1;
	while (!AD1CON1bits.DONE);
	return ADC1BUF0;
}
void delay(unsigned int ms) //opóznienie
{
    unsigned int i;
    unsigned char j;

 for (i =0; i< ms; i++)
 {
		if(PORTDbits.RD6 == 0)
		{
			return;
		}
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
void alarm()// funkcja odpowiedzialna za mruganie diodami przez okolo 3sekundy a potem utrzymuje sta³e œwiecenie na wszytskich diodach
{
	int temp; // zmienna odpowiedzialna za temperature
	unsigned int x=15; //licznik
	while(x > 0){// petla dokonuje migania przez okolo 3 sekundy
	temp = readADC(TSENS); //odczyt temperatur
	if(temp<MAXTEMP) // jesli temperatura jest mniejsza to wylacz alarm
	{
		break;
	}
	PORTA = 1; //wlaczenie jednej diody
	delay(100); // opoznienie 100 milisekund
	PORTA = 0; // wylaczenie diody
	delay(100); // opoznienie 100 milisekund
	x--;
	}
	while(1){ // nieskonczona petla dokonuje stalego swiecenia wszystkich diod
		temp = readADC(TSENS);	//odczyt temperatury
		if(temp < MAXTEMP){ // jesli temperatura jest mniejsza to wylacz alarm
			break;
		}
		PORTA = 255; //ustawienie swiecenia wszystkich diod
		if(PORTDbits.RD6 == 0)
		{
			return;
		}
	}
}

int main(void)
{
	TRISA = 0xFF00;
	TRISD = 0xFFFF;

	unsigned char display = 0;
	unsigned int szybkosc = 10L;
	PORTA = 0x0000;
	ADCinit(AINPUTS); /*inicjalizacja konwertera AD*/

		while (1) { //nieskonczona petla
		temp = readADC(TSENS); //czytanie temperatury
		if(temp > MAXTEMP) // jesli temperatura jest wieksza od maksymalnej to wlacz alarm
		{
			alarm(); // funkcja mrugajaca diodami
			if(PORTDbits.RD6 == 0) // mozliwosc wylaczenie alarmu za pomoca klawisza RD6
			{
				return;
			}
		}
		else //jesli temperatura jest mniejsza od maksymalnej to alarm jest wy³¹czony
		{
			PORTA=0; // wy³¹czenie diod
			if(PORTDbits.RD6 == 0) // mozliwosc wylaczenie alarmu za pomoca klawisza RD6
			{
				return;
			}
		}
	}
}
