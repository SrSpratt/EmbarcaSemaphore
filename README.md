# Embarcatech-Residência
## Embarca-Semáforo
#### Autor:
* Roberto Vítor Lima Gomes Rodrigues

### Semáforo
Como projeto introdutório ao conteúdo de tarefas, foi construído um semáforo utilizando tarefas e os seguintes periféricos: Matriz de LEDs, LEDs RGB, Display SSD1306, botões A e B e buzzer.

#### Vídeo de funcionamento
* https://drive.google.com/file/d/1NpWaId_sk3orjeTtkozWjPhio9ZLTc_-/view


#### Instruções de compilação
Certifique-se de que você tem o ambiente configurado conforme abaixo:
* Pico SDK instalado e configurado.
* VSCode com todas as extensões configuradas, CMake e Make instalados.
* Clone o repositório e abra a pasta do projeto, a extensão Pi Pico criará a pasta build
* Clique em Compile na barra inferior, do lado direito (ao lado esquerdo de RUN | PICO SDK)
* Verifique se gerou o arquivo .uf2
* Conecte a placa BitDogLab e ponha-a em modo BOOTSEL
* Arraste o arquivo até a placa, que o programa se iniciará

#### Manual do programa
Ao executar o programa, o semáforo se iniciará:
* O LED RGB verde ficará aceso por 6s; neste intervalo:
    * A matriz de LEDs mostrará duas setas verdes para a direita
    * O display mostrará um carrinho, indicando que o tráfego está liberado
    * O buzzer repetirá a rotina de: tocar por 1s e permanecer 1s em silêncio
* O LED RGB amarelo ficará aceso por 6s; neste intervalo:
    * A matriz de LEDs mostrará duas setas amarelas para a direita
    * O display mostrará uma exclamação, indicando que o semafóro "irá fechar", então deve-se ter atenção
    * O buzzer repetirá a rotina de: tocar por 0,3s e permanecer 0,3s em silêncio
* O LED RGB vermelho ficará aceso por 6s; neste intervalo:
    * A matriz de LEDs mostrará um X vermelho
    * O display mostrará um X, indicando que o semafóro fechou, e o tráfego deve parar
    * O buzzer repetirá a rotina de: tocar por 0,5s e permanecer 1,5s em silêncio
A qualquer momento, ao se pressionar o botão A, o fluxo do programa será modificado para o modo noturno, no qual:
* O LED RGB amarelo ficará piscando (0,3s aceso e 0,3s apagado)
* A matriz de LEDs mostrará duas setas amarelas para a direita
* O display mostrará uma exclamação, indicando que o semafóro "irá fechar", então deve-se ter atenção
* O buzzer repetirá a rotina de: tocar por 0,3s e permanecer 0,3s em silêncio