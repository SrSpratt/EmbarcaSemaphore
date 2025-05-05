#include "pico/stdlib.h"
#include <RVgeneral.h>
#include <RVsketches.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include <stdio.h>

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define led1 11
#define led2 12
#define led3 13


bool nocturnal = false;

void vBlinkLed1Task()
{
    gpio_init(led1);
    gpio_set_dir(led1, GPIO_OUT);
    while (true)
    {
        if (!nocturnal){
            gpio_put(led1, true);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_put(led1, false);
            vTaskDelay(pdMS_TO_TICKS(1000));
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void vBlinkLed2Task()
{
    gpio_init(led2);
    gpio_set_dir(led2, GPIO_OUT);
    while (true)
    {
        if (!nocturnal){
            gpio_put(led2, true);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_put(led2, false);
            vTaskDelay(pdMS_TO_TICKS(1000));
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void vBlinkLed3Task()
{
    gpio_init(led3);
    gpio_set_dir(led3, GPIO_OUT);
    while (true)
    {
        if (!nocturnal){
            gpio_put(led3, true);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_put(led3, false);
            vTaskDelay(pdMS_TO_TICKS(1000));
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void vBlinkNocturnal()
{
     gpio_init(led1);
     gpio_init(led3);
     gpio_set_dir(led1, GPIO_OUT);
     gpio_set_dir(led3, GPIO_OUT);
    while(true){
        if (nocturnal){
            gpio_put(led1, true);
            gpio_put(led3, true);
            vTaskDelay(pdMS_TO_TICKS(2000));
            gpio_put(led1, false);
            gpio_put(led3, false);
            vTaskDelay(pdMS_TO_TICKS(500));
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void vDisplay3Task()
{
    
    init_interfaces();
    ssd1306_t ssd;
    init_display(&ssd);

    char str_y[5]; // Buffer para armazenar a string
    int contador = 0;
    bool cor = true;
    while (true)
    {
        sprintf(str_y, "%d", contador); // Converte em string
        contador++;                     // Incrementa o contador
        ssd1306_fill(&ssd, !cor);                          // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);      // Desenha um retângulo
        ssd1306_line(&ssd, 3, 25, 123, 25, cor);           // Desenha uma linha
        ssd1306_line(&ssd, 3, 37, 123, 37, cor);           // Desenha uma linha
        ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 6); // Desenha uma string
        ssd1306_draw_string(&ssd, "EMBARCATECH", 20, 16);  // Desenha uma string
        ssd1306_draw_string(&ssd, "  FreeRTOS", 10, 28); // Desenha uma string
        ssd1306_draw_string(&ssd, "Contador  LEDs", 10, 41);    // Desenha uma string
        ssd1306_draw_string(&ssd, str_y, 40, 52);          // Desenha uma string
        ssd1306_send_data(&ssd);                           // Atualiza o display
        sleep_ms(735);
    }
}

volatile uint32_t interval_task = 0;
void vButtonTask(){
    gpio_init(5);
    gpio_set_dir(5, GPIO_IN);
    gpio_pull_up(5);

    while(true){
        //printf("Botão\n");
        uint32_t absolute_counter = to_us_since_boot(get_absolute_time());
        bool pressed = !gpio_get(5);
        if (absolute_counter - interval_task > 250000){
            if (pressed){
                nocturnal = !nocturnal;
                printf("Pressionado!\n");
                printf("Noc? \n%s\n", nocturnal ? "YES" : "NO");
                interval_task = absolute_counter;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}

int main()
{
    // Para ser utilizado o modo BOOTSEL com botão B

    // gpio button_a = {
    //     .pin = 5,
    //     .pin_dir = GPIO_IN
    // };

    // gpio g_pins[1] = {button_a};
    // set_interrupts(g_pins, 1);

    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    // Fim do trecho para modo BOOTSEL com botão B

    pio pio = {
        .pin = 7,
        .address = 0,
        .offset = 0,
        .state_machine = 0
    };

    config_pio(&pio);
    sketch sketch = {
        .background_color = {
            .blue = 0.0, .green = 0.0, .red = 0.0
        },
        .index = 0,
        .main_color = {
            .blue = 0.0, .green = 0.01, .red = 0.0
        },
        .figure = {
            0, 1, 1, 1, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 1, 1, 1, 0,
        } 
    };

    draw(sketch, 0, pio, 25);

    stdio_init_all();

    xTaskCreate(vButtonTask, "Button Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vBlinkLed1Task, "Blink Task Led1", configMINIMAL_STACK_SIZE,
         NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vBlinkLed2Task, "Blink Task Led2", configMINIMAL_STACK_SIZE, 
        NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vBlinkLed3Task, "Blink Task Led3", configMINIMAL_STACK_SIZE, 
        NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vDisplay3Task, "Cont Task Disp3", configMINIMAL_STACK_SIZE, 
        NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vBlinkNocturnal, "Cont Nocturnal", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    vTaskStartScheduler();
    panic_unsupported();

    //debug
    //gpio_init(11);
    //gpio_set_dir(11, GPIO_OUT);
    // while (true){
    //     gpio_put(11, 1);
    //     sleep_ms(500);
    //     gpio_put(11, 0);
    //     sleep_ms(500);
    //     printf("Olá\n");
    // }
}
