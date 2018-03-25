
## Refrances

* By Mahmoud Shokry.
* PIR motion detector sensor with Arduino nano in ultimate power saving mode 
** (reducing power consumption almost by 87% (1.7mA) of normal operation consumption (13mA)) **
[Thanks to:- ](http://playground.arduino.cc/Learning/ArduinoSleepCode)  for information about sleep.h library and modes
[for useful explanation of  "Low Power Arduino! Deep Sleep Tutorial" for bare arduino uno chip](http://www.kevindarrah.com/download/arduino_code/LowPowerVideo.ino)

[Datasheet](http://www.atmel.com/images/atmel-8271-8-bit-avr-microcontroller-atmega48a-48pa-88a-88pa-168a-168pa-328-328p_datasheet_complete.pdf)

** NOTE: when coming back from POWER-DOWN mode, it takes a bit until the system is functional at 100%!! (typically <1sec) **

--- 
## Sleeping Modes helper function Hibernate  
`
 set_sleep_mode
  SLEEP_MODE_IDLE - the lowest power saving mode
  SLEEP_MODE_ADC
  SLEEP_MODE_PWR_SAVE
  SLEEP_MODE_STANDBY
  SLEEP_MODE_PWR_DOWN - the highest power saving mode
  "The Power-down mode saves the register contents but freezes the Oscillator, disabling all other chip functions 
  until the next interrupt or hardware reset."  text from ATMEGA328P datasheet
 `
---
*
**
*Italic*
**Bold**
---
[name](link)