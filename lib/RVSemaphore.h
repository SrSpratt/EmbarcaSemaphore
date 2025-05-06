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
#define PWMLEVEL 0


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

/**
 * @brief Desenha um carro na tela.
 * @param ssd Estrutura do display.
 * @param color Booleano que indica para o laço pintar ou não um pixel.
 * @param frame Envio de frame para o caso de animação.
 */
void draw_car(ssd1306_t *ssd, bool color, int frame);

/**
 * @brief Desenha uma exclamação na tela.
 * @param ssd Estrutura do display.
 * @param color Booleano que indica para o laço pintar ou não um pixel.
 */
void draw_attention(ssd1306_t *ssd, bool color);

/**
 * @brief Desenha um X na tela.
 * @param ssd Estrutura do display.
 * @param color Booleano que indica para o laço pintar ou não um pixel.
 */
void draw_forbidding(ssd1306_t *ssd, bool color);

/**
 * @brief Controla o display para mostrar as figuras de acordo com os estados do semáforo.
 */
void vDisplay3Task();

/**
 * @brief Controla o botão alternar a flag de estado noturno.
 */
void vButtonTask();

#endif