// Bibliotecas padrão do C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hardware/clocks.h"
#include "hardware/pio.h"

// Bibliotecas da Raspberry Pi Pico
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h"


// Bibliotecas específicas do projeto
#include "include/joystick.h"
#include "include/ssd1306.h"
#include "include/leds.h"
#include "include/button.h"

#define LOG(var) printf("%s: %d\n", #var, var);
#define TEMPO_DEBOUNCE 150
#define LIMIAR 150

#define NUM_PIXELS 25
#define OUT_PIN 7

//vetor para criar imagem na matriz de led - 1
double desenho[25] =   {0.0, 0.3, 0.3, 0.3, 0.0,
    0.0, 0.3, 0.0, 0.3, 0.0, 
    0.0, 0.3, 0.3, 0.3, 0.0,
    0.0, 0.3, 0.0, 0.3, 0.0,
    0.0, 0.3, 0.3, 0.3, 0.0};

uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812b
//rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        
            valor_led = matrix_rgb(desenho[24-i], r=0.0, g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
}


display myDisplay;

const char* bool_to_string(bool value) {
    return value ? "true" : "false";
}

void print_display(bool sol, bool chuva, bool vento) {
    display_clear(&myDisplay);
    display_draw_string(10, 20, sol ? "Sol: true" : "Sol: false", true, &myDisplay);
    display_draw_string(10, 30, chuva ? "Chuva: true" : "Chuva: false", true, &myDisplay);
    display_draw_string(10, 40, vento ? "Vento: true" : "Vento: false", true, &myDisplay);
    display_update(&myDisplay);
}




void verificar_joystick(int16_t x, int16_t y) {
    //Intensidade dos leds vária de acordo com o módulo do vetor direção

    uint8_t intensidade;
    int16_t x_absoluto=abs(x);
    int16_t y_absoluto=abs(y);
    intensidade = x_absoluto>y_absoluto?x_absoluto:y_absoluto;
    
    bool sol = false, chuva = false, vento = false;
    int direcao = -1;

    if(x>LIMIAR && y_absoluto<=LIMIAR){
        direcao=2;
        //Leste
    }else if (x<-LIMIAR && y_absoluto<=LIMIAR)
    {   direcao=6;
        //Oeste
    }else if (y>LIMIAR && x_absoluto<=LIMIAR)
    {   direcao=0;
        //Norte
    }else if (y<-LIMIAR && x_absoluto<=LIMIAR)
    {   direcao=4;
        //Sul
    }else if (x>=LIMIAR && y>=LIMIAR)
    {   direcao=1;
        //Nordeste
    }else if (x>=LIMIAR && y<=-LIMIAR)
    {   direcao=3;
        //Sudeste
    }else if (x<=-LIMIAR && y<=-LIMIAR)
    {   direcao=5;
        //Sudoeste
    }else if (x<=-LIMIAR && y>=LIMIAR)
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
            display_clear(&myDisplay);
            display_update(&myDisplay);
            
            break;
        case 0:
            
            led_intensity(LED_GREEN,0);
            led_intensity(LED_BLUE,0);
            led_intensity(LED_RED,0);
            printf("Caso 0: Norte\n");
            print_display(sol,chuva,vento);
            break; // Norte
            
        case 1:
            vento = true;
            led_intensity(LED_GREEN,intensidade);

            led_intensity(LED_RED,0);
            led_intensity(LED_BLUE,0);
            printf("Caso 1: Nordeste\n");
            print_display(sol,chuva,vento);
            break; // Nordeste
        case 2:
            chuva = true;
            led_intensity(LED_BLUE,intensidade);
            led_intensity(LED_RED,0);
            led_intensity(LED_GREEN,0);
            printf("Caso 2: Leste\n");
            print_display(sol,chuva,vento);
            break; // Leste
        case 3:
            chuva = true; 
            vento = true;
            led_intensity(LED_GREEN,intensidade); 
            led_intensity(LED_BLUE,intensidade); 
            led_intensity(LED_RED,0); 
            printf("Caso 3: Sudeste\n");
            print_display(sol,chuva,vento);
            break; // Sudeste
        case 4:
            sol = true;
            led_intensity(LED_RED,intensidade); 
            led_intensity(LED_GREEN,0); 
            led_intensity(LED_BLUE,0);
            printf("Caso 4: Sul\n");
            print_display(sol,chuva,vento);
            break; // Sul
        case 5:
            sol = true;
            vento = true; 
            led_intensity(LED_RED,intensidade);
            led_intensity(LED_GREEN,intensidade);
            led_intensity(LED_BLUE,0);
            printf("Caso 5: Sudeste\n");
            print_display(sol,chuva,vento);
            break; // Sudoeste
        case 6:
            sol = true;
            chuva = true; 
            led_intensity(LED_RED,intensidade);
            led_intensity(LED_BLUE,intensidade);
            led_intensity(LED_GREEN,0);
            printf("Caso 6: Oeste\n");
            print_display(sol,chuva,vento);
            break; // Oeste
        case 7:
            sol = true; 
            chuva = true; 
            vento = true; 
            led_intensity(LED_GREEN,intensidade);
            led_intensity(LED_BLUE,intensidade);
            led_intensity(LED_RED,intensidade);
            printf("Caso 7: Noroeste \n");
            print_display(sol,chuva,vento);
            break; // Noroeste
    }
}


int main(){   
    PIO pio = pio0; 
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;
    
    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    //configurações da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    desenho_pio(desenho,valor_led,pio,sm,r,g,b);
    

    // habilita a entrada no modo BOOTSELL ao pressionar o botao
    // do Joystick da BitDogLab
    init_button_with_interrupt(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);

    stdio_init_all();
    led_init(LED_RED);
    led_init(LED_GREEN);
    led_init(LED_BLUE);
    joystick_init(JOYSTICK_X_PIN, JOYSTICK_Y_PIN);
    
    display_init(&myDisplay);
    display_clear(&myDisplay);
    display_update(&myDisplay);
    while (true) {
        // Debug utilizando porta serial
        int16_t joystick_x = joystick_read(JOYSTICK_X_PIN, 10, 510);
        int16_t joystick_y = joystick_read(JOYSTICK_Y_PIN, 10, 510);

        verificar_joystick(joystick_x, joystick_y);
        sleep_ms(1000);
    }
}
