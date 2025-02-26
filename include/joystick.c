#include "joystick.h"

// Armazena os valores centrais do joystick, calibrados na inicialização
static uint16_t centro_x = 0;
static uint16_t centro_y = 0;

void joystick_init(uint8_t pino_x, uint8_t pino_y) {
    // Inicializa o módulo ADC (Conversor Analógico-Digital)
    adc_init();
    
    // Configura os pinos do joystick para entrada analógica
    adc_gpio_init(pino_x);
    adc_gpio_init(pino_y);
    
    // Realiza a calibração dos valores centrais do joystick
    // Lê os valores iniciais dos eixos X e Y para definir o ponto neutro
    adc_select_input(pino_x - 26);
    centro_x = adc_read();
    adc_select_input(pino_y - 26);
    centro_y = adc_read();
}

int16_t joystick_read(uint8_t pino, uint16_t zona_morta, uint16_t limite) {
    // Seleciona o canal ADC correspondente ao pino do eixo a ser lido
    adc_select_input(pino - 26);
    
    // Lê o valor atual do joystick no eixo especificado
    uint16_t valor_lido = adc_read();
    
    // Determina o valor central de referência para o eixo em questão
    uint16_t centro = (pino == JOYSTICK_X_PIN) ? centro_x : centro_y;
    
    // Calcula o desvio do valor lido em relação ao centro
    int16_t valor_ajustado = (int16_t)valor_lido - (int16_t)centro;
    
    // Converte a zona morta para a escala do ADC (0 a 4095)
    int16_t zona_morta_escalada = (zona_morta * 4095) / limite;
    
    // Aplica a zona morta: se o desvio for pequeno, considera como zero (sem movimento)
    if (valor_ajustado > -zona_morta_escalada && valor_ajustado < zona_morta_escalada) {
        return 0;
    }
    
    // Define o intervalo máximo esperado para saída (-limite/2 a +limite/2)
    int16_t intervalo = limite / 2;
    
    // Normaliza o valor ajustado dentro do intervalo esperado
    if (valor_ajustado < 0) {
        return (valor_ajustado * intervalo) / centro; // Ajuste para valores negativos
    } else {
        return (valor_ajustado * intervalo) / (4095 - centro); // Ajuste para valores positivos
    }
}
