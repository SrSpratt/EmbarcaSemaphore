#include <RVgeneral.h>
#include <RVsketches.h>
#include <string.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#define led1 11
#define led3 13

#define buzzerA 10
#define buzzerB 21

typedef enum {
    GREEN,
    YELLOW,
    RED
} stage;
volatile stage current_stage = GREEN;
bool nocturnal = false;

void vBlinkLed1Task()
{
    while (true)
    {
        if (!nocturnal){

            current_stage = GREEN;
            gpio_put(led1, true);
            vTaskDelay(pdMS_TO_TICKS(6000));

            current_stage = YELLOW;
            gpio_put(led3, true);
            vTaskDelay(pdMS_TO_TICKS(6000));

            current_stage = RED;
            gpio_put(led1, false);
            vTaskDelay(pdMS_TO_TICKS(6000));
            gpio_put(led3, false);

        } else {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

void draw_new(sketch sketch, uint32_t led_cfg, pio pio, const uint8_t vector_size){

    for(int16_t i = 0; i < vector_size; i++){
        if (sketch.figure[i] == 1)
            led_cfg = rgb_matrix(sketch.main_color);
        else
            led_cfg = rgb_matrix(sketch.background_color);
        pio_sm_put_blocking(pio.address, pio.state_machine, led_cfg);
    }

};


void vMatrixTask(){

    pio my_pio = {
        .pin = 7,
        .address = 0,
        .offset = 0,
        .state_machine = 0
    };

    config_pio(&my_pio);

    sketch sketch1 = {
        .background_color = {
            .blue = 0.0, .green = 0.0, .red = 0.0
        },
        .index = 0,
        .main_color = {
            .blue = 0.0, .green = 0.01, .red = 0.0
        },
        .figure = {
            0, 0, 1, 0, 1,
            0, 1, 0, 1, 0,
            1, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 1
        } 
    };

    sketch sketch2 = {
        .background_color = {
            .blue = 0.0, .green = 0.0, .red = 0.0
        },
        .index = 0,
        .main_color = {
            .blue = 0.0, .green = 0.01, .red = 0.01
        },
        .figure = {
            0, 0, 1, 0, 1,
            0, 1, 0, 1, 0,
            1, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 1
        } 
    };

    sketch sketch3 = {
        .background_color = {
            .blue = 0.0, .green = 0.0, .red = 0.0
        },
        .index = 0,
        .main_color = {
            .blue = 0.0, .green = 0.00, .red = 0.01
        },
        .figure = {
            1, 0, 0, 0, 1,
            0, 1, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 1, 0,
            1, 0, 0, 0, 1 
        } 
    };


    while(true){
        switch(current_stage){
            case GREEN:
                sketch1.main_color.red = 0.00;
                draw_new(sketch1, 0, my_pio, 25);
                printf("green\n");
                break;
            case YELLOW:
                //vector_copy(sketch3.figure, sketchY.figure, 25);
                sketch1.main_color.red = 0.01;
                draw_new(sketch1, 0, my_pio, 25);
                printf("yellow\n");
                break;
            case RED:
                draw_new(sketch3, 0, my_pio, 25);
                printf("red\n");
                break;
            default:
                break;             
        }
        vTaskDelay(pdMS_TO_TICKS(1500));
    }

}

void vBlinkNocturnalTask()
{
    while(true){
        if (nocturnal){
            current_stage = YELLOW;
            gpio_put(led1, true);
            gpio_put(led3, true);
            vTaskDelay(pdMS_TO_TICKS(300));
            gpio_put(led1, false);
            gpio_put(led3, false);
            vTaskDelay(pdMS_TO_TICKS(300));
        } else {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

void vSoundTask()
{
    while(true){
        switch(current_stage){
            case GREEN:
                pwm_set_gpio_level(buzzerA, 1024);
                vTaskDelay(pdMS_TO_TICKS(1000));
                pwm_set_gpio_level(buzzerA, 0);
                vTaskDelay(pdMS_TO_TICKS(1000));
                break;
            case YELLOW:
                pwm_set_gpio_level(buzzerA, 1024);
                vTaskDelay(pdMS_TO_TICKS(300));
                pwm_set_gpio_level(buzzerA, 0);
                vTaskDelay(pdMS_TO_TICKS(300));
                break;
            case RED:
                pwm_set_gpio_level(buzzerA, 1024);
                vTaskDelay(pdMS_TO_TICKS(500));
                pwm_set_gpio_level(buzzerA, 0);
                vTaskDelay(pdMS_TO_TICKS(1500));
                break;
        }
    }
}

void draw_stick(ssd1306_t *ssd, bool color, int frame){
    ssd1306_fill_rect(ssd, !color, 4, 128 - 4, 26, 64 - 3);

    ssd1306_rect(ssd, 32, 57, 11, 5, color, color); // top e left estão invertidos em relação à linha
    ssd1306_rect(ssd, 32, 61, 3, 18, color, color);

    // switch(frame % 5) {
    //     case 0: // Frame completo - passo direito
    //         // Braço esquerdo avançado (para frente)
    //         ssd1306_line(ssd, 62, 39, 55, 46, color);
    //         // Braço direito recuado (para trás)
    //         ssd1306_line(ssd, 62, 39, 68, 41, color);
    //         // Perna esquerda recuada (levemente atrás)
    //         ssd1306_line(ssd, 62, 49, 57, 51, color);
    //         // Perna direita avançada (para frente)
    //         ssd1306_line(ssd, 62, 49, 67, 54, color);
    //         break;

    //     case 1: // Meio passo 1 - transição suave
    //         // Braços começando a alternar
    //         ssd1306_line(ssd, 62, 39, 58, 37, color);
    //         ssd1306_line(ssd, 62, 39, 65, 40, color);
    //         // Pernas em posição intermediária
    //         ssd1306_line(ssd, 62, 49, 59, 51, color);
    //         ssd1306_line(ssd, 62, 49, 65, 52, color);
    //         break;

    //     case 2: // Frame neutro - posição vertical
    //         // Braços quase verticais
    //         ssd1306_line(ssd, 62, 39, 59, 39, color);
    //         ssd1306_line(ssd, 62, 39, 65, 48, color);
    //         // Pernas quase verticais
    //         ssd1306_line(ssd, 62, 49, 60, 53, color);
    //         ssd1306_line(ssd, 62, 49, 64, 53, color);
    //         break;

    //     case 3: // Meio passo 2 - transição suave
    //         // Braços alternando
    //         ssd1306_line(ssd, 62, 39, 65, 37, color);
    //         ssd1306_line(ssd, 62, 39, 58, 40, color);
    //         // Pernas preparando próximo passo
    //         ssd1306_line(ssd, 62, 49, 65, 51, color);
    //         ssd1306_line(ssd, 62, 49, 59, 52, color);
    //         break;

    //     case 4: // Frame completo - passo esquerdo
    //         // Braço direito avançado
    //         ssd1306_line(ssd, 62, 39, 69, 36, color);
    //         // Braço esquerdo recuado
    //         ssd1306_line(ssd, 62, 39, 57, 41, color);
    //         // Perna direita recuada
    //         ssd1306_line(ssd, 62, 49, 67, 51, color);
    //         // Perna esquerda avançada
    //         ssd1306_line(ssd, 62, 49, 57, 54, color);
    //         break;
    // }

    switch(frame%5){
        case 1:
        default:
            ssd1306_line(ssd, 61, 39, 55, 45, color);
            ssd1306_line(ssd, 62, 39, 56, 45, color);

            ssd1306_line(ssd, 63, 39, 69, 45, color);
            ssd1306_line(ssd, 62, 39, 68, 45, color);

            ssd1306_line(ssd, 61, 49, 55, 58, color);
            ssd1306_line(ssd, 62, 49, 56, 58, color); 

            ssd1306_line(ssd, 63, 49, 69, 58, color);
            ssd1306_line(ssd, 62, 49, 68, 58, color);
            break;
    }
}

void draw_attention(ssd1306_t *ssd, bool color){
    ssd1306_fill_rect(ssd, !color, 4, 128 - 4, 26, 64 - 3);

    ssd1306_line(ssd, 57, 32, 70, 32, color);

    ssd1306_line(ssd, 57, 32, 61, 50, color);
    ssd1306_line(ssd, 66, 50, 70, 32, color);
    ssd1306_line(ssd, 58, 32, 62, 50, color);
    ssd1306_line(ssd, 65, 50, 69, 32, color);

    ssd1306_line(ssd, 61, 50, 66, 50, color);

    ssd1306_rect(ssd, 56, 61, 5, 5, color, color);
}

void draw_forbidding(ssd1306_t *ssd, bool color){
    ssd1306_fill_rect(ssd, !color, 4, 128 - 4, 26, 64 - 3);

    ssd1306_line(ssd, 50, 32, 75, 60, color);
    ssd1306_line(ssd, 51, 32, 76, 60, color);
    ssd1306_line(ssd, 52, 32, 77, 60, color);

    ssd1306_line(ssd, 50, 60, 75, 32, color);
    ssd1306_line(ssd, 49, 60, 74, 32, color);
    ssd1306_line(ssd, 48, 60, 73, 32, color);
}

void vDisplay3Task()
{
    ssd1306_t ssd;
    init_display(&ssd);

    char str_y[5]; // Buffer para armazenar a string
    int counter = 0;
    bool color = true;
    int animation_frame = 0;

    ssd1306_fill(&ssd, !color);                          // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 60, color, !color);      // Desenha um retângulo
    ssd1306_line(&ssd, 3, 25, 123, 25, color);           // Desenha uma linha
    ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 6); // Desenha uma string
    ssd1306_draw_string(&ssd, "EMBARCATECH", 20, 16);  // Desenha uma string
    while (true)
    {
        sprintf(str_y, "%d", counter); // Converte em string
        counter++;                     // Incrementa o contador
        switch(current_stage){
            case GREEN:
                draw_stick(&ssd, color,animation_frame);
                animation_frame++;
                break;
            case YELLOW:
                draw_attention(&ssd, color);
                break;
            case RED:
                draw_forbidding(&ssd, color);
                break;
            default:
                ssd1306_fill_rect(&ssd, !color, 4, 128 - 4, 26, 64 - 3);
                break;
        }

        //ssd1306_draw_string(&ssd, "  FreeRTOS", 10, 28); // Desenha uma string
        //ssd1306_draw_string(&ssd, "Contador  LEDs", 10, 41);    // Desenha uma string
        //ssd1306_draw_string(&ssd, str_y, 40, 52);          // Desenha uma string
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
    init_interfaces();

    gpio_init(led1);
    gpio_init(led3);
    gpio_set_dir(led1, GPIO_OUT);
    gpio_set_dir(led3, GPIO_OUT);

    gpio_init(buzzerA);
    gpio_set_dir(buzzerA, GPIO_OUT);
    gpio_set_function(buzzerA, GPIO_FUNC_PWM);
    uint8_t slice = pwm_gpio_to_slice_num(buzzerA);
    pwm_set_wrap(slice, PWM_WRAP);
    pwm_set_clkdiv(slice, PWM_CLKDIV);
    pwm_set_enabled(slice, true);

    xTaskCreate(vButtonTask, "Button Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vBlinkLed1Task, "Blink Task Led1", configMINIMAL_STACK_SIZE,
         NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vDisplay3Task, "Cont Task Disp3", configMINIMAL_STACK_SIZE, 
        NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vBlinkNocturnalTask, "Cont Nocturnal", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vSoundTask, "Sound", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vMatrixTask, "Matrix", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
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
