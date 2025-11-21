#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Joystick.h>

/*
  HOTAS Throttle – Firmware
  =========================
  Board   : Arduino Pro Micro (ATmega32u4, 5V / 16 MHz)
  ADC     : ADS1115 (I2C)
  Inputs  : 1 x Throttle axis (pot on ADS1115 A0)
            1 x Rotary encoder with push button
            3 x Momentary push buttons
  Output  : USB HID Joystick (1 axis + buttons)

  TR:
    Bu kod, Pro Micro + ADS1115 kullanarak USB joystick
    tabanlı bir throttle denetleyicisi oluşturur. Gaz ekseni
    ADS1115 üzerinden okunur, enkoder ve 3 buton joystick
    butonlarına map edilir.
*/

Adafruit_ADS1115 ads;

// ---- Throttle calibration ----
// Raw values from ADS1115 at full back and full forward.
//
// IMPORTANT:
//   Measure these for your own hardware using the
//   calibration sketch described in docs/calibration-guide.md
//
#define RAW_MIN  72       // full back (idle)
#define RAW_MAX  32767    // full forward

// Throttle filtering
static float   filteredRaw   = 0.0f;
static int16_t lastSentRaw   = 0;
#define CHANGE_THRESHOLD 40   // ignore small changes (raw units)

// ---- Encoder pins & button mapping ----
//
// Hardware wiring:
//   Encoder CLK (A)  -> D5
//   Encoder DT  (B)  -> D6
//   Encoder SW (btn) -> D7
//
const int ENC_A_PIN   = 5;
const int ENC_B_PIN   = 6;
const int ENC_BTN_PIN = 7;

// Joystick button indices (0‑based)
const int ENC_BTN_INDEX     = 0;  // encoder push (Joystick Button 1)
const int ENC_CW_BTN_INDEX  = 1;  // encoder clockwise tick (Button 2)
const int ENC_CCW_BTN_INDEX = 2;  // encoder counter‑clockwise tick (Button 3)

// Encoder state
int  lastEncA = HIGH;
int  lastEncB = HIGH;
long encoderValue = 0;
long lastReportedEncoderValue = 0;

// ---- Push buttons ----
//
// Each button: one side to GND, other side to pin below.
// All configured with INPUT_PULLUP.
//
// Suggested wiring:
//   BTN1 -> D8
//   BTN2 -> D9
//   BTN3 -> D10
//
const int BTN1_PIN = 8;
const int BTN2_PIN = 9;
const int BTN3_PIN = 10;

// Button indices in joystick HID report
const int BTN1_INDEX = 3;  // Joystick Button 4
const int BTN2_INDEX = 4;  // Joystick Button 5
const int BTN3_INDEX = 5;  // Joystick Button 6

// ---- Joystick object ----
//
// 1 throttle axis + up to 16 buttons.
// Other axes are disabled.
//
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK,
  16, 0,     // buttons, hats
  false, false, false, false, false, false, false,
  true,      // throttle
  false, false, false
);

// ----- Encoder read helper -----
void readEncoder()
{
  int a = digitalRead(ENC_A_PIN);
  int b = digitalRead(ENC_B_PIN);

  // On A change, detect direction
  if (a != lastEncA) {
    if (a == LOW) {      // falling edge
      if (b == HIGH) {
        encoderValue++;  // clockwise
      } else {
        encoderValue--;  // counter‑clockwise
      }
    }
  }

  lastEncA = a;
  lastEncB = b;
}

// ----- Setup -----
void setup()
{
  Serial.begin(115200);

  Wire.begin();
  ads.begin();
  ads.setGain(GAIN_ONE);   // 0–4.096 V range

  pinMode(ENC_A_PIN,   INPUT_PULLUP);
  pinMode(ENC_B_PIN,   INPUT_PULLUP);
  pinMode(ENC_BTN_PIN, INPUT_PULLUP);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  Joystick.begin();

  Serial.println(F("HOTAS Throttle firmware started."));
}

// ----- Main loop -----
void loop()
{
  // -------- 1) Throttle axis --------
  int16_t raw = ads.readADC_SingleEnded(0);

  // Clamp to calibration range
  if (raw < RAW_MIN) raw = RAW_MIN;
  if (raw > RAW_MAX) raw = RAW_MAX;

  // Low‑pass filter
  if (filteredRaw == 0.0f) {
    filteredRaw = raw;
  } else {
    filteredRaw = filteredRaw * 0.8f + 0.2f * raw;
  }

  int16_t filtered = (int16_t)filteredRaw;

  // Only send to joystick if there is a significant change
  if (abs(filtered - lastSentRaw) >= CHANGE_THRESHOLD) {
    lastSentRaw = filtered;

    // Map to joystick throttle range 0‑1023
    // If the direction feels reversed in‑game,
    // swap 0 and 1023 in this map() call.
    int throttle = map(filtered, RAW_MIN, RAW_MAX, 0, 1023);
    // int throttle = map(filtered, RAW_MIN, RAW_MAX, 1023, 0); // reversed

    throttle = constrain(throttle, 0, 1023);
    Joystick.setThrottle(throttle);
  }

  // -------- 2) Encoder --------
  readEncoder();

  // Encoder push button
  int encBtnState = digitalRead(ENC_BTN_PIN);
  Joystick.setButton(ENC_BTN_INDEX, encBtnState == LOW ? 1 : 0);

  // Encoder rotation -> short button pulses
  if (encoderValue > lastReportedEncoderValue) {
    Serial.println(F("ENC: CW"));
    Joystick.setButton(ENC_CW_BTN_INDEX, 1);
    delay(5);
    Joystick.setButton(ENC_CW_BTN_INDEX, 0);
    lastReportedEncoderValue = encoderValue;
  }
  else if (encoderValue < lastReportedEncoderValue) {
    Serial.println(F("ENC: CCW"));
    Joystick.setButton(ENC_CCW_BTN_INDEX, 1);
    delay(5);
    Joystick.setButton(ENC_CCW_BTN_INDEX, 0);
    lastReportedEncoderValue = encoderValue;
  }

  // -------- 3) Push buttons --------
  int b1 = digitalRead(BTN1_PIN);
  int b2 = digitalRead(BTN2_PIN);
  int b3 = digitalRead(BTN3_PIN);

  Joystick.setButton(BTN1_INDEX, b1 == LOW ? 1 : 0);
  Joystick.setButton(BTN2_INDEX, b2 == LOW ? 1 : 0);
  Joystick.setButton(BTN3_INDEX, b3 == LOW ? 1 : 0);

  delay(5);
}
