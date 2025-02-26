#ifndef ANI_H
#define ANI_H

//Declaração das funções da animação
void npInit(uint pin);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear();
void npWrite();

#endif