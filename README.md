# HOTAS Throttle

DIY USB HOTAS throttle based on **Arduino Pro Micro** and **ADS1115**.

Bu depo, Arduino Pro Micro ve ADS1115 kullanarak geliştirilen bir USB HOTAS gaz kolu (throttle) projesini içerir.

## Features / Özellikler

- 1 × high‑resolution **throttle axis** (ADS1115 16‑bit ADC)
- 1 × **rotary encoder** (CW/CCW + push button)
- 3 × **momentary push buttons**
- Appears in Windows as a standard USB joystick (tested in MSFS, DCS, X‑Plane)
- Noise‑filtered axis with calibration for custom potentiometers

...

See `docs/pinout.md` and `docs/calibration-guide.md` for wiring and calibration details.


## Calibration
Upload `calibration/Calibration.ino`, open Serial Monitor at 115200 and note RAW_MIN/RAW_MAX.

📦 3D Model Attribution

(EN & TR)

🇬🇧 English

The 3D components used in this project are based on the design:
“CtrlAltG13 Modular HOTAS Setup” by CtrlAltG13
Source: https://www.printables.com/model/54264-ctrlaltg13-modular-a-hotas-setup-inspired-by-the-l

License: Creative Commons BY-NC-SA 4.0

Under this license:

Commercial use is not allowed

Any derivative design must be shared under the same license (ShareAlike)

Attribution to the original designer is required

This repository includes only the electronics, firmware, and documentation created by me.
All rights for the original 3D design belong to CtrlAltG13.

🇹🇷 Türkçe

Bu projede kullanılan 3D parçalar aşağıdaki tasarıma dayanmaktadır:
“CtrlAltG13 Modular HOTAS Setup” – Tasarımcı: CtrlAltG13
Kaynak: https://www.printables.com/model/54264-ctrlaltg13-modular-a-hotas-setup-inspired-by-the-l

Lisans: Creative Commons BY-NC-SA 4.0

Bu lisans kapsamında:

Ticari kullanım yasaktır

Türetilmiş işler aynı lisansla paylaşılmalıdır (ShareAlike)

Orijinal tasarımcıya atıf yapılması zorunludur

Bu depo yalnızca tarafımdan geliştirilen elektronik, yazılım ve dokümanları içerir.
Orijinal 3D model tasarımının tüm hakları CtrlAltG13’a aittir.
