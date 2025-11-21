# Throttle Calibration Guide

1. Upload the simple calibration sketch (not the main firmware).
2. Open the Serial Monitor at 115200 baud.
3. Move the throttle to **full back** and wait a few seconds.
   - Note the lowest stable value → this is `RAW_MIN`.
4. Move the throttle to **full forward** and wait a few seconds.
   - Note the highest stable value → this is `RAW_MAX`.
5. Open `firmware/HOTAS_Throttle_Firmware.ino` and update:

```cpp
#define RAW_MIN  <your_min_here>
#define RAW_MAX  <your_max_here>
```

6. Re‑upload the main firmware.

TR:

1. Basit kalibrasyon sketch’ini karta yükleyin.
2. Seri monitörü 115200 baud’da açın.
3. Gaz kolunu tam geriye alın, birkaç saniye bekleyin.
   - En düşük stabil değeri not edin → `RAW_MIN`.
4. Gaz kolunu tam ileri alın, birkaç saniye bekleyin.
   - En yüksek stabil değeri not edin → `RAW_MAX`.
5. `HOTAS_Throttle_Firmware.ino` dosyasında bu değerleri güncelleyin.
6. Ana firmware’i tekrar yükleyin.
