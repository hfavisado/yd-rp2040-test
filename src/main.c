// YD-RP2040 board test: USR button toggles GPIO25 LED and randomizes the WS2812 color.
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define LED_PIN      25   // single-color LED
#define WS2812_PIN   23   // onboard WS2812 data
#define BUTTON_PIN   24   // USR button, active low
#define DEBOUNCE_MS  30

static PIO ws_pio = pio0;
static uint ws_sm;

// WS2812 expects GRB order, MSB first, in the top 24 bits of the word.
static void ws2812_put(uint8_t r, uint8_t g, uint8_t b) {
    pio_sm_put_blocking(ws_pio, ws_sm, ((uint32_t)g << 24) | ((uint32_t)r << 16) | ((uint32_t)b << 8));
}

static void random_color(void) {
    uint32_t v = get_rand_32();
    uint8_t r = v & 0xFF, g = (v >> 8) & 0xFF, b = (v >> 16) & 0xFF;
    ws2812_put(r, g, b);
    printf("WS2812 color: R=%u G=%u B=%u\n", r, g, b);
}

int main(void) {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    uint offset = pio_add_program(ws_pio, &ws2812_program);
    ws_sm = pio_claim_unused_sm(ws_pio, true);
    ws2812_program_init(ws_pio, ws_sm, offset, WS2812_PIN, 800000);

    // Startup indication: blink LED and flash WS2812 white
    for (int i = 0; i < 3; i++) {
        gpio_put(LED_PIN, 1);
        ws2812_put(32, 32, 32);
        sleep_ms(150);
        gpio_put(LED_PIN, 0);
        ws2812_put(0, 0, 0);
        sleep_ms(150);
    }

    bool led_on = false;
    bool last_pressed = false;
    while (true) {
        bool pressed = !gpio_get(BUTTON_PIN);
        if (pressed != last_pressed) {
            sleep_ms(DEBOUNCE_MS);              // wait out contact bounce
            pressed = !gpio_get(BUTTON_PIN);    // re-sample
            if (pressed != last_pressed) {
                last_pressed = pressed;
                if (pressed) {                  // act once per press
                    led_on = !led_on;
                    gpio_put(LED_PIN, led_on);
                    printf("LED GPIO25: %s\n", led_on ? "ON" : "OFF");
                    random_color();
                }
            }
        }
        sleep_ms(1);
    }
}
