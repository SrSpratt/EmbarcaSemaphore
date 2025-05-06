#ifndef RVSEMAPHORE_H
#define RVSEMAPHORE_H

#include <RVgeneral.h>
#include <RVsketches.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#define led1 11
#define led3 13
#define buttonA 5

#define buzzerA 10
#define buzzerB 21
#define PWMLEVEL 128


/**
 * @brief Enumeração que guarda os estados do semáforo.
 */
typedef enum {
    GREEN,
    YELLOW,
    RED
} stage;


/**
 * @brief Controla os LEDs e os estados do semáforo no modo diurno.
 */
void vBlinkSemaphoreTask();

/**
 * @brief Desenha as figuras na matriz de LEDs de acordo com o estado do semáforo.
 * @param sketch Estrutura de desenho.
 * @param led_cfg Configuração inicial de LEDs.
 * @param pio Configuração PIO.
 * @param vector_size Tamanho do vetor de figuras.
 */
void draw_new(sketch sketch, uint32_t led_cfg, pio pio, const uint8_t vector_size);

/**
 * @brief Controla as figuras da matriz de LEDs.
 */
void vMatrixTask();

/**
 * @brief Controla o semáforo no modo noturno, piscando o LED amarelo e mudando o estado para "amarelo".
 */
void vBlinkNocturnalTask();

/**
 * @brief Controla o buzzer para emitir os sons de acordo com os estados do semáforo.
 */
void vSoundTask();

void draw_stick(ssd1306_t *ssd, bool color, int frame);

void draw_attention(ssd1306_t *ssd, bool color);

void draw_forbidding(ssd1306_t *ssd, bool color);

void vDisplay3Task();

void vButtonTask();

#endif