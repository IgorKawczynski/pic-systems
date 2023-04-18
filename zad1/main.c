#include <stdio.h>
#include <stdlib.h>

void LED_On(int ledTable[8], int index) {
    ledTable[index] = 1;
}

void LED_Off(int ledTable[8], int index) {
    ledTable[index] = 0;
}

void LED_Print(int leds[8]) {
    printf("{%d", leds[0]);
    for(int i = 1; i < 8; i++) {
        printf(", %d", leds[i]);
    }
    printf("}");
}

int intoGray(int x) {
    return x ^ (x >> 1);
}

int outGray(int gray) {
    int x = gray;
    while (gray > 0) {
        gray = gray >> 1;
        x = x ^ gray;
    }
    return x;
}

void LED_OnBCD(int ledTable[4], int index) {
    ledTable[index] = 1;
}

void LED_OffBCD(int ledTable[4], int index) {
    ledTable[index] = 0;
}

void LED_PrintBCD(int leds[4]) {
    printf("{%d, %d, %d, %d}", leds[3], leds[2], leds[1], leds[0]);
}


int main()
{
    // Deklaracja LED'ów
    int leds[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    int LED_D10 = 0;
    int LED_D9 = 1;
    int LED_D8 = 2;
    int LED_D7 = 3;
    int LED_D6 = 4;
    int LED_D5 = 5;
    int LED_D4 = 6;
    int LED_D3 = 7;

// Wyprintowanie LED'ów
    LED_Print(leds);

// // Zaœwietlenie LED'ów
//     LED_On(leds, LED_D10);

// // Wyprintowanie LED'ów
//     LED_Print(leds);

// !!!!!!!!!!!!!!!!!!!!!! KONWERTER OD 0 DO 255 !!!!!!!!!!!!!!!!!!!!!!
    int startValue = 0;
    int ledTable[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    while ( 1 )
        {
            int bin = startValue;
            for(int x = 0; bin > 0; x++) {
                ledTable[x] = bin % 2;
                bin = bin / 2;
            }
            (ledTable[0] == 1) ? LED_On(leds, LED_D3) : LED_Off(leds, LED_D3);
            (ledTable[1] == 1) ? LED_On(leds, LED_D4) : LED_Off(leds, LED_D4);
            (ledTable[2] == 1) ? LED_On(leds, LED_D5) : LED_Off(leds, LED_D5);
            (ledTable[3] == 1) ? LED_On(leds, LED_D6) : LED_Off(leds, LED_D6);
            (ledTable[4] == 1) ? LED_On(leds, LED_D7) : LED_Off(leds, LED_D7);
            (ledTable[5] == 1) ? LED_On(leds, LED_D8) : LED_Off(leds, LED_D8);
            (ledTable[6] == 1) ? LED_On(leds, LED_D9) : LED_Off(leds, LED_D9);
            (ledTable[7] == 1) ? LED_On(leds, LED_D10) : LED_Off(leds, LED_D10);

            startValue = startValue + 1;
            if(startValue > 255) {
                startValue = 0;
                break;
        }
    }
    LED_Print(leds);


// !!!!!!!!!!!!!!!!!!!!!! KONWERTER OD 255 DO 0 !!!!!!!!!!!!!!!!!!!!!!
    int startValue2 = 255;
    int ledTable2[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    int leds2[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    while ( 1 )
    {
        int bin2 = startValue2;
        for(int x2 = 0; bin2 > 0; x2++) {
            ledTable2[x2] = bin2 % 2;
            bin2 = bin2 / 2;
        }
        (ledTable2[0] == 1) ? LED_Off(leds2, LED_D3) : LED_On(leds2, LED_D3);
        (ledTable2[1] == 1) ? LED_Off(leds2, LED_D4) : LED_On(leds2, LED_D4);
        (ledTable2[2] == 1) ? LED_Off(leds2, LED_D5) : LED_On(leds2, LED_D5);
        (ledTable2[3] == 1) ? LED_Off(leds2, LED_D6) : LED_On(leds2, LED_D6);
        (ledTable2[4] == 1) ? LED_Off(leds2, LED_D7) : LED_On(leds2, LED_D7);
        (ledTable2[5] == 1) ? LED_Off(leds2, LED_D8) : LED_On(leds2, LED_D8);
        (ledTable2[6] == 1) ? LED_Off(leds2, LED_D9) : LED_On(leds2, LED_D9);
        (ledTable2[7] == 1) ? LED_Off(leds2, LED_D10) : LED_On(leds2, LED_D10);

        startValue2 = startValue2 - 1;
        if(startValue2 < 0) {
            startValue2 = 255;
            break;
        }
    }
    LED_Print(leds2);


    // !!!!!!!!!!!!!!!!!!!!!! KONWERTER NA KOD GRAYA OD 0 DO 255 !!!!!!!!!!!!!!!!!!!!!!
    int startValue3 = 0;
    int ledTable3[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int leds3[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    while (startValue3 <= 255)
    {
        int gray3 = intoGray(startValue3);
        for(int x3 = 0; gray3 > 0; x3++) {
            ledTable3[x3] = gray3 % 2;
            gray3 = gray3 / 2;
        }
        (ledTable3[0] == 1) ? LED_On(leds3, LED_D3) : LED_Off(leds3, LED_D3);
        (ledTable3[1] == 1) ? LED_On(leds3, LED_D4) : LED_Off(leds3, LED_D4);
        (ledTable3[2] == 1) ? LED_On(leds3, LED_D5) : LED_Off(leds3, LED_D5);
        (ledTable3[3] == 1) ? LED_On(leds3, LED_D6) : LED_Off(leds3, LED_D6);
        (ledTable3[4] == 1) ? LED_On(leds3, LED_D7) : LED_Off(leds3, LED_D7);
        (ledTable3[5] == 1) ? LED_On(leds3, LED_D8) : LED_Off(leds3, LED_D8);
        (ledTable3[6] == 1) ? LED_On(leds3, LED_D9) : LED_Off(leds3, LED_D9);
        (ledTable3[7] == 1) ? LED_On(leds3, LED_D10) : LED_Off(leds3, LED_D10);

        startValue3 = startValue3 + 1;
        if(startValue3 > 255) {
            startValue3 = 0;
            break;
        }
    }
    LED_Print(leds3);


    // !!!!!!!!!!!!!!!!!!!!!! KONWERTER Z KODU GRAYA OD 255 DO 0 !!!!!!!!!!!!!!!!!!!!!!
    int startValue4 = 255;
    int ledTable4[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    int leds4[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    while (startValue4 >= 0)
    {
        int num4 = outGray(startValue4);
        for(int x4 = 0; num4 > 0; x4++) {
            ledTable4[x4] = num4 % 2;
            num4 = num4 / 2;
        }
        (ledTable4[0] == 1) ? LED_Off(leds4, LED_D3) : LED_On(leds4, LED_D3);
        (ledTable4[1] == 1) ? LED_Off(leds4, LED_D4) : LED_On(leds4, LED_D4);
        (ledTable4[2] == 1) ? LED_Off(leds4, LED_D5) : LED_On(leds4, LED_D5);
        (ledTable4[3] == 1) ? LED_Off(leds4, LED_D6) : LED_On(leds4, LED_D6);
        (ledTable4[4] == 1) ? LED_Off(leds4, LED_D7) : LED_On(leds4, LED_D7);
        (ledTable4[5] == 1) ? LED_Off(leds4, LED_D8) : LED_On(leds4, LED_D8);
        (ledTable4[6] == 1) ? LED_Off(leds4, LED_D9) : LED_On(leds4, LED_D9);
        (ledTable4[7] == 1) ? LED_Off(leds4, LED_D10) : LED_On(leds4, LED_D10);

        startValue4 = startValue4 - 1;
        if(startValue4 < 0) {
            startValue4 = 255;
            break;
        }
    }
    LED_Print(leds4);
    printf("\n\n\n");


    // !!!!!!!!!!!!!!!!!!!!!! KONWERTER Z KODU BCD OD 0 DO 99 (2x4bit) !!!!!!!!!!!!!!!!!!!!!!
    int num = 0;
    while (num < 100) {
        int ones = num % 10;
        int tens = num / 10;
        int ledTableOnes[4] = {0, 0, 0, 0};
        int ledTableTens[4] = {0, 0, 0, 0};

        // Przekszta³cenie jednoœci na kod BCD
        for (int i = 0; i < 4; i++) {
            ledTableOnes[i] = ones % 2;
            ones = ones / 2;
        }

        // Przekszta³cenie dziesi¹tek na kod BCD
        for (int i = 0; i < 4; i++) {
            ledTableTens[i] = tens % 2;
            tens = tens / 2;
        }

        // Wyœwietlenie jednoœci
        printf("1-4Bit: ");
        LED_PrintBCD(ledTableOnes);
        printf("\n");

        // Wyœwietlenie dziesi¹tek
        printf("4-8Bit: ");
        LED_PrintBCD(ledTableTens);
        printf("\n\n");

        num++;
    }

    // !!!!!!!!!!!!!!!!!!!!!! KONWERTER Z KODU BCD OD 99 DO 0 (2x4bit) !!!!!!!!!!!!!!!!!!!!!!
    int num2 = 99;
    while (num2 >= 0) {
        int ones = num2 % 10;
        int tens = num2 / 10;
        int ledTableOnes[4] = {0, 0, 0, 0};
        int ledTableTens[4] = {0, 0, 0, 0};

        // Przekszta³cenie jednoœci na kod BCD
        for (int i = 0; i < 4; i++) {
            ledTableOnes[i] = ones % 2;
            ones = ones / 2;
        }

        // Przekszta³cenie dziesi¹tek na kod BCD
        for (int i = 0; i < 4; i++) {
            ledTableTens[i] = tens % 2;
            tens = tens / 2;
        }

        // Wyœwietlenie jednoœci
        printf("1-4Bit: ");
        LED_PrintBCD(ledTableOnes);
        printf("\n");

        // Wyœwietlenie dziesi¹tek
        printf("4-8Bit: ");
        LED_PrintBCD(ledTableTens);
        printf("\n\n");

        num2--;
    }



    return 0;
}
