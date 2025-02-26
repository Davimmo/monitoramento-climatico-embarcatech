// Bibliotecas padrão do C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Bibliotecas da Raspberry Pi Pico
#include "pico/stdlib.h"
#include "pico/bootrom.h"

// Bibliotecas específicas do projeto
#include "include/joystick.h"
#include "include/ssd1306.h"
#include "include/leds.h"
#include "include/button.h"

#define LOG(var) printf("%s: %d\n", #var, var);
#define TEMPO_DEBOUNCE 150
#define LIMIAR 150




// int16_t intensidade_chuva = 0;
// int16_t intensidade_chuva = 0;
// int16_t intensidade_chuva = 0;



void verificar_joystick(int16_t x, int16_t y) {
    //Intensidade dos leds vária de acordo com o módulo do vetor direção

    uint8_t intensidade;
    int16_t x_absoluto=abs(x);
    int16_t y_absoluto=abs(y);
    intensidade = x_absoluto>y_absoluto?x:y;
    
    bool sol = false, chuva = false, vento = false;
    int direcao = -1;

    if(x>LIMIAR && y_absoluto<LIMIAR){
        direcao=2;
        //Leste
    }else if (x<-LIMIAR && y_absoluto<LIMIAR)
    {   direcao=6;
        //Oeste
    }else if (y>LIMIAR && x_absoluto<LIMIAR)
    {   direcao=0;
        //Norte
    }else if (y<-LIMIAR && x_absoluto<LIMIAR)
    {   direcao=4;
        //Sul
    }else if (x>LIMIAR && y>LIMIAR)
    {   direcao=1;
        //Nordeste
    }else if (x>LIMIAR && y<-LIMIAR)
    {   direcao=3;
        //Sudeste
    }else if (x<-LIMIAR && y<-LIMIAR)
    {   direcao=5;
        //Sudoeste
    }else if (x<-LIMIAR && y>LIMIAR)
    {   direcao=7;
        //Noroeste
    }else if (x_absoluto<LIMIAR && y_absoluto<LIMIAR)
    {   direcao=-1;
        //Nada a ser feito
    }
     
    
    
    
    //Chuva: vento: green, chuva: blue, sol: red
    switch (direcao) {
        case -1:
            led_intensity(LED_GREEN,0);
            led_intensity(LED_BLUE,0);
            led_intensity(LED_RED,0);
            printf("Caso -1\n");
            break;
        case 0:
            led_intensity(LED_GREEN,0);
            led_intensity(LED_BLUE,0);
            led_intensity(LED_RED,0);
            printf("Caso 0: Norte\n");
            break; // Norte
        case 1:
            vento = true;
            led_intensity(LED_GREEN,intensidade);

            led_intensity(LED_RED,0);
            led_intensity(LED_BLUE,0);
            printf("Caso 1: Nordeste\n");
            break; // Nordeste
        case 2:
            chuva = true;
            led_intensity(LED_BLUE,intensidade);
            LOG(intensidade);

            led_intensity(LED_BLUE,0);
            led_intensity(LED_GREEN,0);
            printf("Caso 2: Leste\n");
            break; // Leste
        case 3:
            chuva = true; 
            vento = true;
            led_intensity(LED_GREEN,intensidade); 
            led_intensity(LED_BLUE,intensidade); 
            led_intensity(LED_RED,0); 
            printf("Caso 3: Sudeste\n");
            break; // Sudeste
        case 4:
            sol = true;
            led_intensity(LED_RED,intensidade); 
            led_intensity(LED_GREEN,0); 
            led_intensity(LED_BLUE,0);
            printf("Caso 4: Sul\n"); 
            break; // Sul
        case 5:
            sol = true;
            vento = true; 
            led_intensity(LED_RED,intensidade);
            led_intensity(LED_GREEN,intensidade);
            led_intensity(LED_BLUE,0);
            printf("Caso 5: Sudeste\n");
            break; // Sudoeste
        case 6:
            sol = true;
            chuva = true; 
            led_intensity(LED_RED,intensidade);
            led_intensity(LED_BLUE,intensidade);
            led_intensity(LED_GREEN,0);
            printf("Caso 6: Oeste\n");
            break; // Oeste
        case 7:
            sol = true; 
            chuva = true; 
            vento = true; 
            led_intensity(LED_GREEN,intensidade);
            led_intensity(LED_BLUE,intensidade);
            led_intensity(LED_RED,intensidade);
            printf("Caso 7: Noroeste \n");
            break; // Noroeste
    }
}


int main()
{   //Define o botão do joystick como bootsel
    // habilita a entrada no modo BOOTSELL ao pressionar o botao
    // do Joystick da BitDogLab
    init_button_with_interrupt(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);

    stdio_init_all();
    led_init(LED_RED);
    led_init(LED_GREEN);
    led_init(LED_BLUE);
    joystick_init(JOYSTICK_X_PIN, JOYSTICK_Y_PIN);
    

    while (true) {
        // Debug utilizando porta serial
        int16_t joystick_x = joystick_read(JOYSTICK_X_PIN, 10, 510);
        int16_t joystick_y = joystick_read(JOYSTICK_Y_PIN, 10, 510);

        verificar_joystick(joystick_x, joystick_y);
    }
}
