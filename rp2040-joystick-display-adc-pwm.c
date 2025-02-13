#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_ADDR 0x3C

#define LED_VERDE 11
#define LED_AZUL 12
#define LED_VERMELHO 13
#define JOYSTICK_X 26
#define JOYSTICK_Y 27
#define BUTTON_JOY 22
#define BUTTON_A 5

bool led_pwm_enabled = false;
bool led_g_state = false;
uint8_t border_style = 0;
ssd1306_t display;

// Função para debounce dos botões
bool debounce(uint gpio) {
    static uint64_t last_press_time[2] = {0, 0};
    uint64_t now = to_us_since_boot(get_absolute_time());
    uint64_t debounce_interval = 200000;  // 200ms de debounce

    if ((now - last_press_time[gpio]) < debounce_interval) {
        return false;
    }

    last_press_time[gpio] = now;
    return true;
}

// Alterna o LED verde e a borda ao pressionar o joystick
void toggle_led_g(uint gpio, uint32_t events) {
    
    printf("Executando toggle_led_g()\n");

    led_g_state = !led_g_state;
    if (led_g_state) {
        pwm_set_gpio_level(LED_VERDE, 2048);  // LED verde ligado
    } else {
        pwm_set_gpio_level(LED_VERDE, 0);  // LED verde desligado
    }

    border_style = (border_style + 1) % 2;

    printf("Botão do joystick pressionado. LED Verde: %s | Borda Estilo: %d\n",
           led_g_state ? "ON" : "OFF", border_style);
}

// Alterna os LEDs PWM ao pressionar o botão A
void toggle_led_pwm(uint gpio, uint32_t events) {
    
    printf("Executando toggle_led_pwm()\n");

    led_pwm_enabled = !led_pwm_enabled;

    if (led_pwm_enabled) {
        pwm_set_gpio_level(LED_AZUL, 2048);
        pwm_set_gpio_level(LED_VERMELHO, 2048);
    } else {
        pwm_set_gpio_level(LED_AZUL, 0);
        pwm_set_gpio_level(LED_VERMELHO, 0);
    }

    printf("Botão A pressionado. LEDs PWM: %s\n", led_pwm_enabled ? "ATIVADOS" : "DESATIVADOS");
}

// Configura PWM para os LEDs
void setup_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice, 4095);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(gpio), 0);
    pwm_set_enabled(slice, true);
}

void gpio_callback(uint gpio, uint32_t events) {
    if (!debounce(gpio)) return;

    printf("Interrupção detectada no GPIO %d\n", gpio);

    if (gpio == BUTTON_JOY) {
        printf("Chamando toggle_led_g()\n");
        toggle_led_g(gpio, events);
    } else if (gpio == BUTTON_A) {
        printf("Chamando toggle_led_pwm()\n");
        toggle_led_pwm(gpio, events);
    }
}

// Configuração inicial do sistema
void setup() {
    stdio_init_all();

    setup_pwm(LED_AZUL);
    setup_pwm(LED_VERDE);
    setup_pwm(LED_VERMELHO);

    adc_init();
    adc_gpio_init(JOYSTICK_X);
    adc_gpio_init(JOYSTICK_Y);

    // Configuração dos botões
    gpio_init(BUTTON_JOY);
    gpio_set_dir(BUTTON_JOY, GPIO_IN);
    gpio_pull_up(BUTTON_JOY);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Registra apenas um callback global
    gpio_set_irq_enabled_with_callback(BUTTON_JOY, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);  // Apenas ativa a interrupção

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&display, 128, 64, false, I2C_ADDR, I2C_PORT);
    ssd1306_config(&display);
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);

    printf("Display inicializado corretamente.\n");
}

// Desenha a borda de acordo com o estilo atual
void draw_border() {
    switch (border_style) {
        case 0:
            ssd1306_rect(&display, 0, 0, 127, 63, true, false);
            break;
        case 1:
            // Borda mais grossa
            ssd1306_rect(&display, 0, 0, 127, 63, true, false);
            ssd1306_rect(&display, 2, 2, 123, 59, true, false);
            break;
    }
}

// Loop principal do programa
void loop() {
    adc_select_input(0);
    uint16_t x_val = adc_read();
    adc_select_input(1);
    uint16_t y_val = adc_read();

    if (led_pwm_enabled) {
        pwm_set_gpio_level(LED_AZUL, abs((int)x_val - 2048) * 2);
        pwm_set_gpio_level(LED_VERMELHO, abs((int)y_val - 2048) * 2);
    }

    ssd1306_fill(&display, false);

    // Desenha a borda conforme o estilo atual
    draw_border();

    // Desenha o cursor do joystick
    uint8_t x_pos = (x_val * 112) / 4095;
    uint8_t y_pos = (y_val * 56) / 4095;
    ssd1306_rect(&display, y_pos, x_pos, 8, 8, true, true);

    ssd1306_send_data(&display);

    sleep_ms(50);
}

// Função principal
int main() {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
