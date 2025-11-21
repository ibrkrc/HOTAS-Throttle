# Pinout (Arduino Pro Micro)

## I2C / ADS1115

- D2  → SDA  → ADS1115 SDA
- D3  → SCL  → ADS1115 SCL
- ADS1115 A0 → Throttle pot wiper (middle pin)
- Throttle pot other pins → 5V and GND

## Rotary Encoder

- D5  → Encoder CLK (A)
- D6  → Encoder DT  (B)
- D7  → Encoder SW  (push button)
- Encoder VCC → 5V
- Encoder GND → GND

## Push Buttons

Each button: one side to pin, one side to GND. Use INPUT_PULLUP in firmware.

- D8  → BTN1
- D9  → BTN2
- D10 → BTN3
