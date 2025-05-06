#include <RVSemaphore.h>

volatile stage current_stage = GREEN;
volatile bool nocturnal = false;
volatile uint16_t tick = 10;

void vBlinkSemaphoreTask()
{
    while (true)
    {
        if (!nocturnal){

            if (!nocturnal){
                current_stage = GREEN;
                gpio_put(led3, false);
                gpio_put(led1, true);
                for (int i = 0; i < 6000/tick && !nocturnal; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
            }

            if (!nocturnal){
                current_stage = YELLOW;
                gpio_put(led3, true);
                for (int i = 0; i < 6000/tick && !nocturnal; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
            }

            if (!nocturnal){
                current_stage = RED;
                gpio_put(led1, false);
                for (int i = 0; i < 6000/tick && !nocturnal; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
                gpio_put(led3, false);
            }

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

    // Seta indicando passagem liberada
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

    //Seta, mas em amarelo, indicando atenção
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

    // X em vermelho, indicando para parar
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
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}

void vBlinkNocturnalTask()
{
    while(true){
        if (nocturnal){
            if (nocturnal){
                current_stage = YELLOW;
                gpio_put(led1, true);
                gpio_put(led3, true);
                for (int i = 0; i < 300/tick && nocturnal; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
            }

            if (nocturnal){
                gpio_put(led1, false);
                gpio_put(led3, false);
                for (int i = 0; i < 300/tick && nocturnal; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
            }
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
                pwm_set_gpio_level(buzzerA, PWMLEVEL);
                for (int i = 0; i < 1000/tick && !nocturnal; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
                pwm_set_gpio_level(buzzerA, 0);
                for (int i = 0; i < 1000/tick && !nocturnal; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
                break;
            case YELLOW:
                pwm_set_gpio_level(buzzerA, PWMLEVEL);
                for (int i = 0; i < 300/tick; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
                pwm_set_gpio_level(buzzerA, 0);
                for (int i = 0; i < 300/tick; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
                break;
            case RED:
                pwm_set_gpio_level(buzzerA, PWMLEVEL);
                for (int i = 0; i < 500/tick && !nocturnal; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
                pwm_set_gpio_level(buzzerA, 0);
                for (int i = 0; i < 1500/tick && !nocturnal; i++)
                    vTaskDelay(pdMS_TO_TICKS(tick));
                break;
        }
    }
}
//desenha a pessoa
void draw_stick(ssd1306_t *ssd, bool color, int frame){
    ssd1306_fill_rect(ssd, !color, 4, 128 - 4, 26, 64 - 3);

    ssd1306_rect(ssd, 32, 57, 11, 5, color, color); // top e left estão invertidos em relação à linha
    ssd1306_rect(ssd, 32, 61, 3, 18, color, color);

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

//desenha a exclamação
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

//desenha o X
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

    bool color = true;
    int animation_frame = 0;

    ssd1306_fill(&ssd, !color);                          // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 60, color, !color);      // Desenha um retângulo
    ssd1306_line(&ssd, 3, 25, 123, 25, color);           // Desenha uma linha
    ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 6); // Desenha uma string
    ssd1306_draw_string(&ssd, "EMBARCATECH", 20, 16);  // Desenha uma string
    while (true)
    {
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
        ssd1306_send_data(&ssd);                           // Atualiza o display
        sleep_ms(735);
    }
}

volatile uint32_t interval_task = 0;
void vButtonTask(){

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