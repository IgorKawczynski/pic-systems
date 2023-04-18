#include <p24fj128ga010.h>

//Konfiguracja dla Explorer 16 z progr. icd2
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF)
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI)

#define SCALE 308L

unsigned int funct1(unsigned int display){// 8 bitowy licznik binarny zliczający w górę (0...255)
	return display=display+1; // zwiekszenie display o 1
}

unsigned int funct2(unsigned int display){// 8 bitowy licznik binarny zliczający w dół (255...0)
	return display=display-1; // zmniejszenie display o 1
}

unsigned int funct34(unsigned int display){// 8 bitowy licznik w kodzie Graya zliczający w górę i dół (repr. 0...255)
	return display ^(display>>1); // przesunąć liczbę w postaci binarnej o jeden bit w prawo
	// wykonać operację XOR na display i display przesunete o 1 bit w prawo
}

unsigned int funct56(unsigned int display){// 2x4 bitowy licznik w kodzie BCD zliczający w górę i dół (0...99)
	int result =0;
	int ten = display/10; //uzyskanie liczby dziesiatek
	int one = display%10; //uzyskanie liczby jednosci
	ten = ten << 4; //przesuniecie o 4bity w lewo w liczbie dziesiatek
	result = ten|one; // operacja or na liczbach jednosci i dziesiatek
	return result;
}

unsigned int funct7(unsigned int display){// 3 bitowy wężyk poruszający się lewo-prawo
	static int side = 1; //zmienna odpowiedzialna za ustawienie kierunku węża
						 //side=1 kierunek lewa, side=0 kierunek prawa
	if( side == 1){ //jesli jest lewa strona
			display=display<<1; //przesuniecie weza o 1 do przodu w lewa strone
	}
	else if(side == 0){ // jesli jest prawa strona 
			display=display>>1; // przesuniecie weza o 1 do przodu w prawa strone
	}

	if(display == 224){ //jesli waz dojdzie od konca lewej strony(maksymalna wartosc 224) to zmiana strony na prawą
			side=0;
	}
	else if(display == 7){  //jesli waz dojdzie od konca prawej strony(maksymalna wartosc 7) to zmiana strony na lewa
			side=1;
	}
	return display;
}

unsigned int funct8(){// 8. Kolejka
	static int stop = 128; // ustawienie zatrzymania kolejki
	static int kolejka = 1; // licznik trasy
	unsigned int display = 0; // nowa wartosc wyswietlania
	static int wait = 0; // ustawienie czekania

	if( display == 255){ // po wykonaniu kolejki restartuje ją
			display = 1;
			kolejka = 1;
			wait = 0;
			stop = 128;
	}
	else if(kolejka > stop){ // gdy kolejka ma wieksza wartosc od stopu(kolejka przekroczy lewa strone)
        wait = wait | stop; // dodanie wartosci stopu przez operator or (ustawienie lewej strony kolejki)
		kolejka = 1; // restart kolejki
        display = wait | kolejka; // ustawienie wartosci wyswietlania kolejki z uzgodnieniem oczekiwania
		stop = stop >> 1; // zmniejszenie stopu o jeden
		if(stop == 0){ // restart stopu i oczekiwania przy ostatniej trasie kolejki
            stop = 128;
            wait = 0;
		}
	}
    display = wait | kolejka; // ustawienie wyswietlania wartosci kolejki i uwzglednienie oczekiwania jesli jest
	kolejka = kolejka << 1; // przesuniecie kolejki do pzodu

	return display;
}


int k_bit(int n, int k)
{
     return (n>>k)&1; // Sprawdz przesuniety bit za pomoca AND
}

int funct9(int result)// 6 bitowy generator liczb pseudolosowych oparty o konfigurację 1110011
{
 int n, k1,k2,rand;
 n=result;
 k1=k_bit(n, 0); // k1=0bit liczby n
 k2=k_bit(n, 1); // k2=2bit liczby n
 k1=k1^k2; // wynik XOR(k1,k2)
 k2=k_bit(n, 4); // k2=4bit liczby n
 k1=k1^k2; // wynik XOR(k1,k2)
 k2=k_bit(n, 5); // k2=5bit liczby n
 k1=k1^k2; // wynik XOR(k1,k2)
 n=n>>1; // n= n przesuniete o 1bit w prawo
 rand=k1<<5; // rand= k1 przesuniete o 5bitow w lewo
 n=rand|n; // n= operacja OR(rand, n)
 result = n; 
 return result; //zwraca zawartosc n
}

int main(void){
unsigned long i;
unsigned char display=0; //zmienna odpowiedzialna za wyświetlanie
//inicjalizacja
 PORTA=0x0000;
 TRISA=0xFF00;
 TRISD=0xFFFF;
 unsigned int przelacznik=0; //zmienna odpowiedzialna za przełączanie podprogramów
 unsigned int acctual=0; //zmienna która zawiera aktualną liczbe wyswietlania
 unsigned int snake=7; //zmienna odpowiedzialna za wężyk, równa 7 ponieważ 7 w binarnych to 111
 unsigned int prng=1; // zmienna odpowiedzialna za generator liczb pseudolosowych
again:
	Nop();
	PORTA=(unsigned int) display;
	for(i=500L*SCALE;i>0;i--) Nop();
	// Zmiana podprogramu:
	// RD13 - kolejny program, RD6 - poprzedni program
	if (PORTDbits.RD13 == 0){//sprawdz czy przycisk zostal użyty
		if(przelacznik == 9){//przy ostatnim podprogramie gdy kliknie sie przycisk RD13 uruchamia podprogram 0 
			przelacznik=0;
		}
		else{
	    	przelacznik=przelacznik+1;// po klinieciu przycisku RD13 uruchamia kolejny podprogram
		}
	}
	else if(PORTDbits.RD6 == 0){//sprawdz czy przycisk zostal użyty
	    if(przelacznik == -1){//przy pierwszym podprogramie gdy kliknie sie przycisk RD6 uruchamia podprogram 8
			przelacznik=8;
		}
		else{
			przelacznik=przelacznik-1;// po klinieciu przycisku RD6 uruchamia poprzedni podprogram
		}
	}

	// Ustawienie podprogramów:
	if (przelacznik==0){ // 1. 8 bitowy licznik binarny zliczający w górę (0...255)
		display=funct1(display);// wczytanie funkcji do wyswietlenia
		acctual = display;// zapisanie aktualnej wyswietlanej liczby
	}
	else if (przelacznik==1){ // 2. 8 bitowy licznik binarny zliczający w dół (255...0)
		display=funct2(display);// wczytanie funkcji do wyswietlenia
		acctual = display;// zapisanie aktualnej wyswietlanej liczby
	}
	else if (przelacznik==2){ // 3. 8 bitowy licznik w kodzie Graya zliczający w górę (repr. 0...255)
		display=funct34(acctual++);// wczytanie funkcji do wyswietlenia z ostatnia wyswietlana wartoscia ktora jest zwiekszana o 1
		acctual = display;// zapisanie aktualnej wyswietlanej liczby
	}
	else if (przelacznik==3){ // 4. 8 bitowy licznik w kodzie Graya zliczający w dół (repr. 255...0)
		display=funct34(acctual--);// wczytanie funkcji do wyswietlenia z ostatnia wyswietlana wartoscia ktora jest zmniejszana o 1
		acctual = display;// zapisanie aktualnej wyswietlanej liczby
	}
	else if (przelacznik==4){ // 5. 2x4 bitowy licznik w kodzie BCD zliczający w górę (0...99)
		if(acctual >=100){ //restart acctual do 0 gdy jest >= 100
				acctual = 0;
		}
		display=funct56(acctual++);// wczytanie funkcji do wyswietlenia z ostatnia wyswietlana wartoscia ktora jest zwiekszana o 1
	}
	else if (przelacznik==5){ // 6. 2x4 bitowy licznik w kodzie BCD zliczający w dół (99...0)
		if(acctual <=0){//restart acctual do 99 gdy jest <= 0
				acctual = 99;
		}
		display=funct56(acctual--);// wczytanie funkcji do wyswietlenia z ostatnia wyswietlana wartoscia ktora jest zmniejszana o 1
	}
	else if (przelacznik==6){ // 7. 3 bitowy wężyk poruszający się lewo-prawo
		display=snake;// wczytanie zmiennej snake do wyswietlania
		snake=funct7(snake++);// zapis do zmiennej snake funkcji z wartoscia snake zwiekszajacą sie o 1
	}
	else if (przelacznik==7){ // 8 .Kolejka
		display=funct8(); //wyswietlenie funkcji
	}
	else if (przelacznik==8){ // 9. 6 bitowy generator liczb pseudolosowych oparty o konfigurację 1110011
		display=prng; // display pobiera wartosc zmiennej prng
    	display = funct9(prng);// wczytanie funkcji do wyswietlenia z wartoscia prng
    	if(prng>=63)//reatrt zmiennej prng do 0 gdy jest wieksza równa 63
    	{
        prng=0;
    	}
    	prng=display;//wczytanie wyswietlanej wartosci do prng
	}

	goto again;

}
