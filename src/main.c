// Bibliotecas padrão do C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hardware/clocks.h"
#include "hardware/pio.h"

// Bibliotecas da Raspberry Pi Pico
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "include/ani.h"


// Bibliotecas específicas do projeto
#include "include/joystick.h"
#include "include/ssd1306.h"
#include "include/leds.h"
#include "include/button.h"

#define LOG(var) printf("%s: %d\n", #var, var);
#define TEMPO_DEBOUNCE 150
#define LIMIAR 50

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
    static int ultima_direcao = -1; // Última direção registrada

    uint8_t intensidade;
    int16_t x_absoluto = abs(x);
    int16_t y_absoluto = abs(y);
    intensidade = x_absoluto > y_absoluto ? x_absoluto : y_absoluto;

    bool sol = false, chuva = false, vento = false;
    int direcao = -1;

    // Determina a direção do joystick
    if (x > LIMIAR && y_absoluto <= LIMIAR)
        direcao = 2; // Leste
    else if (x < -LIMIAR && y_absoluto <= LIMIAR)
        direcao = 6; // Oeste
    else if (y > LIMIAR && x_absoluto <= LIMIAR)
        direcao = 0; // Norte
    else if (y < -LIMIAR && x_absoluto <= LIMIAR)
        direcao = 4; // Sul
    else if (x >= LIMIAR && y >= LIMIAR)
        direcao = 1; // Nordeste
    else if (x >= LIMIAR && y <= -LIMIAR)
        direcao = 3; // Sudeste
    else if (x <= -LIMIAR && y <= -LIMIAR)
        direcao = 5; // Sudoeste
    else if (x <= -LIMIAR && y >= LIMIAR)
        direcao = 7; // Noroeste

    // Se a direção mudou, apaga os LEDs antes de acender os novos
    if (direcao != ultima_direcao) {
        npClear();
        npWrite();
        led_intensity(LED_GREEN, 0);
        led_intensity(LED_BLUE, 0);
        led_intensity(LED_RED, 0);
        display_clear(&myDisplay);
        display_update(&myDisplay);
    }

    // Acende apenas os LEDs da direção atual e atualiza o display
    switch (direcao) {
        case -1:
            printf("Caso -1: Sem movimento\n");
            display_clear(&myDisplay);
            display_update(&myDisplay);
            break;
        case 0:
            printf("Caso 0: Norte\n");
            print_display(sol, chuva, vento);
            break;
        case 1:
            vento = true;
            led_intensity(LED_GREEN, intensidade);
            npSetLED(12, 0, 100, 0);
            print_display(sol, chuva, vento);
            break;
        case 2:
            chuva = true;
            led_intensity(LED_BLUE, intensidade);
            npSetLED(10, 0, 0, 100);
            print_display(sol, chuva, vento);
            break;
        case 3:
            chuva = true;
            vento = true;
            led_intensity(LED_GREEN, intensidade);
            led_intensity(LED_BLUE, intensidade);
            npSetLED(10, 0, 0, 100);
            npSetLED(12, 0, 100, 0);
            print_display(sol, chuva, vento);
            break;
        case 4:
            sol = true;
            led_intensity(LED_RED, intensidade);
            npSetLED(14, 100, 0, 0);
            print_display(sol, chuva, vento);
            break;
        case 5:
            sol = true;
            vento = true;
            led_intensity(LED_RED, intensidade);
            led_intensity(LED_GREEN, intensidade);
            npSetLED(14, 100, 0, 0);
            npSetLED(12, 0, 100, 0);
            print_display(sol, chuva, vento);
            break;
        case 6:
            sol = true;
            chuva = true;
            led_intensity(LED_RED, intensidade);
            led_intensity(LED_BLUE, intensidade);
            npSetLED(14, 100, 0, 0);
            npSetLED(10, 0, 0, 100);
            print_display(sol, chuva, vento);
            break;
        case 7:
            sol = true;
            chuva = true;
            vento = true;
            led_intensity(LED_GREEN, intensidade);
            led_intensity(LED_BLUE, intensidade);
            led_intensity(LED_RED, intensidade);
            npSetLED(10, 0, 0, 100);
            npSetLED(12, 0, 100, 0);
            npSetLED(14, 100, 0, 0);
            print_display(sol, chuva, vento);
            break;
    }

    npWrite(); // Atualiza os LEDs

    // Atualiza a última direção
    ultima_direcao = direcao;
}





int main(){   
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

    npInit(7);


    while (true) {
        // Debug utilizando porta serial
        int16_t joystick_x = joystick_read(JOYSTICK_X_PIN, 10, 510);
        int16_t joystick_y = joystick_read(JOYSTICK_Y_PIN, 10, 510);

        verificar_joystick(joystick_x, joystick_y);
        sleep_ms(10);

    }
}
