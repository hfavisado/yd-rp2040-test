# yd-rp2040-test

Minimal Pico SDK firmware to test a YD-RP2040 board.

Each press of the USR button (GPIO24, active low):
- toggles the LED on GPIO25
- sets the onboard WS2812 (GPIO23) to a random RGB color

On power-up the LED and WS2812 blink three times. Button presses are debounced (30 ms)
and logged over USB serial.

## Build

Requires the [Pico SDK](https://github.com/raspberrypi/pico-sdk), CMake and `arm-none-eabi-gcc`.

```sh
export PICO_SDK_PATH=/path/to/pico-sdk
cmake -B build
cmake --build build
```

## Flash

Hold BOOT, plug in USB-C (or press RESET), release; a `RPI-RP2` drive appears. Copy
`build/yd_rp2040_test.uf2` onto it.

## Ideas for more self-tests (not implemented)

- Long-press USR to cycle test modes
- PWM fade on GPIO25
- ADC read / on-chip temperature sensor
- I2C bus scan
- SPI loopback / flash ID check
