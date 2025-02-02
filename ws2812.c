#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define LED_RED_PIN 13
#define TEMPO_RED 100 // Piscar LED vermelho (5 vezes por segundo)

// Mapeamento dos números (0-9) para uma matriz 5x5
const bool number_to_matrix[10][NUM_PIXELS] = {
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // 0
    {0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0}, // 1
    {1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0}, // 2
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // 3
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1}, // 4
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1}, // 5
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1}, // 6
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // 7
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}  // 9
};

uint8_t current_number = 0; // Número atual exibido
volatile uint64_t last_interrupt_time_a = 0; // Para debounce do botão A
volatile uint64_t last_interrupt_time_b = 0; // Para debounce do botão B

// Função para enviar dados ao NeoPixel
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Converte valores RGB para o formato GRB
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Exibe um número na matriz
void display_number(uint8_t num) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (number_to_matrix[num][i]) {
            put_pixel(urgb_u32(0, 255, 0)); // Verde para LEDs acesos
        } else {
            put_pixel(0); // Apaga LEDs
        }
    }
}

// Configuração do botão A (Incrementa)
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint64_t current_time = time_us_64();

    if (gpio == BUTTON_A_PIN && (current_time - last_interrupt_time_a > 200000)) {
        current_number = (current_number + 1) % 10;
        printf("Botão A pressionado. Número atual: %d\n", current_number);
        last_interrupt_time_a = current_time;
    }

    if (gpio == BUTTON_B_PIN && (current_time - last_interrupt_time_b > 200000)) {
        current_number = (current_number == 0) ? 9 : current_number - 1;
        printf("Botão B pressionado. Número atual: %d\n", current_number);
        last_interrupt_time_b = current_time;
    }
}

int main() {
    stdio_init_all();

    // Configuração do LED RGB
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    // Configuração dos botões
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    // Configuração dos NeoPixels
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Configuração das interrupções dos botões
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);


    while (true) {
    // Diagnóstico contínuo
    printf("Número atual no loop principal: %d\n", current_number);

    // Piscar o LED vermelho
    gpio_put(LED_RED_PIN, 1);
    sleep_ms(TEMPO_RED);
    gpio_put(LED_RED_PIN, 0);
    sleep_ms(TEMPO_RED);

    // Exibir o número atual na matriz
    display_number(current_number);
}


    return 0;
}
