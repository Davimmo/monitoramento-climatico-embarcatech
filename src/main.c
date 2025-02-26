// Bibliotecas padrão do C
#include <stdio.h>
#include <stdlib.h>

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







int main()
{   //Define o botão do joystick como bootsel
    // habilita a entrada no modo BOOTSELL ao pressionar o botao
    // do Joystick da BitDogLab
    init_button_with_interrupt(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);

    stdio_init_all();
    led_init(LED_RED);
    led_intensity(LED_RED, 100);
    

    while (true) {
        sleep_ms(10);
    }
}