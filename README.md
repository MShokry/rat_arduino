
## Refrances

By Mahmoud Shokry.
PIR motion detector sensor with Arduino nano in ultimate power saving mode 
** (reducing power consumption almost by 87% (1.7mA) of normal operation consumption (13mA)) **
[Thanks to:- ](http://playground.arduino.cc/Learning/ArduinoSleepCode)  for information about sleep.h library and modes
[for useful explanation of  "Low Power Arduino! Deep Sleep Tutorial" for bare arduino uno chip](http://www.kevindarrah.com/download/arduino_code/LowPowerVideo.ino)

[Datasheet](http://www.atmel.com/images/atmel-8271-8-bit-avr-microcontroller-atmega48a-48pa-88a-88pa-168a-168pa-328-328p_datasheet_complete.pdf)

** NOTE: when coming back from POWER-DOWN mode, it takes a bit until the system is functional at 100%!! (typically <1sec) **

--- 
## Sleeping Modes helper function Hibernate  
###  set_sleep_mode
`
  SLEEP_MODE_IDLE - the lowest power saving mode
  SLEEP_MODE_ADC
  SLEEP_MODE_PWR_SAVE
  SLEEP_MODE_STANDBY
  SLEEP_MODE_PWR_DOWN - the highest power saving mode
  "The Power-down mode saves the register contents but freezes the Oscillator, disabling all other chip functions 
  until the next interrupt or hardware reset."  text from ATMEGA328P datasheet
 `
 ### attachInterrupt(A, B, C)
`
    attachInterrupt(A, B, C)
    A   can be either 0 or 1 for interrupts on pin 2 or 3.  
   
    B   Name of a function you want to execute while in interrupt A.
   
    C   Trigger mode of the interrupt pin. can be:
                LOW        a low level trigger
                CHANGE     a change in level trigger
                RISING     a rising edge of a level trigger
                FALLING    a falling edge of a level trigger
   `
    In all but the IDLE sleep modes only LOW can be used.
    here since PIR sensor has inbuilt timer to swtich its state from OFF to ON, we are detecting its CHANGE IN STATE to control our LED/relay at pin 13. 
    therefore, we will not need to use arduino delay timer to Set "ON time" of our LED/relay, it can be adjusted physically using potentiometer provided on PIR sensor board.
    This further helps in using SLEEP_MODE_PWR_DOWN which is ultimate lowest power consumption mode for ATMEGA8328P chip  
    (please note - because of onboard power regulators of arduino boards, power consumption cannot be reduced to predicted few microAmps level of bare chips. 
    To achieve further reduction in current consumption, we will need bare ATMEGA328P chip)
 
---
*
**
*Italic*
**Bold**
---
[name](link)