#include <RVSemaphore.h>

// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}

int main()
{
    //inicia o botão B
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    //inicia as interfaces
    init_interfaces();

    //inicia os pinos a serem utilizados
    gpio_init(led1);
    gpio_init(led3);
    gpio_set_dir(led1, GPIO_OUT);
    gpio_set_dir(led3, GPIO_OUT);

    gpio_init(buttonA);
    gpio_set_dir(buttonA, GPIO_IN);
    gpio_pull_up(buttonA);

    gpio_init(buzzerA);
    gpio_set_dir(buzzerA, GPIO_OUT);

    //inicia o pwm no buzzer A
    gpio_set_function(buzzerA, GPIO_FUNC_PWM);
    uint8_t slice = pwm_gpio_to_slice_num(buzzerA);
    pwm_set_wrap(slice, PWM_WRAP);
    pwm_set_clkdiv(slice, PWM_CLKDIV);
    pwm_set_enabled(slice, true);

    //chama as tarefas (todas definidas no RVSemaphore.c e declaradas no RVSemaphore.h)
        //tarefa que "escuta" o pressionamento do botão A para chavear entre modo noturno e diurno
    xTaskCreate(vButtonTask, "Button Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
        //tarefa para os LEDs do semáforo diurno (cada LED fica aceso por 6s)
    xTaskCreate(vBlinkSemaphoreTask, "Blink Task Led1", configMINIMAL_STACK_SIZE,
         NULL, tskIDLE_PRIORITY, NULL);
        //tarefa para mostrar a animação no display
    xTaskCreate(vDisplay3Task, "Cont Task Disp3", configMINIMAL_STACK_SIZE, 
        NULL, tskIDLE_PRIORITY, NULL);
        //tarefa para piscar o LED amarelo no modo noturno
    xTaskCreate(vBlinkNocturnalTask, "Cont Nocturnal", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
        //tarefa para o buzzer
    xTaskCreate(vSoundTask, "Sound", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
        //tarefa para mostrar as animações na matriz
    xTaskCreate(vMatrixTask, "Matrix", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    vTaskStartScheduler();

    panic_unsupported();
}
