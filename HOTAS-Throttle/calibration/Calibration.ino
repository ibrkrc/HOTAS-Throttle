
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!ads.begin()) {
    Serial.println("ADS1115 not found!");
    while (1);
  }

  ads.setGain(GAIN_ONE);
  Serial.println("Calibration Mode");
  Serial.println("Move throttle full back and forward and observe raw values.");
}

void loop() {
  int16_t val = ads.readADC_SingleEnded(0);
  Serial.println(val);
  delay(50);
}
