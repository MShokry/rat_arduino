
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
`.
  SLEEP_MODE_IDLE - the lowest power saving mode
  SLEEP_MODE_ADC
  SLEEP_MODE_PWR_SAVE
  SLEEP_MODE_STANDBY
  SLEEP_MODE_PWR_DOWN - the highest power saving mode
  "The Power-down mode saves the register contents but freezes the Oscillator, disabling all other chip functions 
  until the next interrupt or hardware reset."  text from ATMEGA328P datasheet
 .`
 ### attachInterrupt(A, B, C)
`<
    A   can be either 0 or 1 for interrupts on pin 2 or 3.  
   
    B   Name of a function you want to execute while in interrupt A.
   
    C   Trigger mode of the interrupt pin. can be:
                LOW        a low level trigger
                CHANGE     a change in level trigger
                RISING     a rising edge of a level trigger
                FALLING    a falling edge of a level trigger
  >`
    In all but the IDLE sleep modes only LOW can be used.
    here since PIR sensor has inbuilt timer to swtich its state from OFF to ON, we are detecting its CHANGE IN STATE to control our LED/relay at pin 13. 
    therefore, we will not need to use arduino delay timer to Set "ON time" of our LED/relay, it can be adjusted physically using potentiometer provided on PIR sensor board.
    This further helps in using SLEEP_MODE_PWR_DOWN which is ultimate lowest power consumption mode for ATMEGA8328P chip  
    (please note - because of onboard power regulators of arduino boards, power consumption cannot be reduced to predicted few microAmps level of bare chips. 
    To achieve further reduction in current consumption, we will need bare ATMEGA328P chip)
---
## WDT
// The MCU Status Register (MCUSR) is used to tell the cause of the last
// reset, such as brown-out reset, watchdog reset, etc.
// NOTE: for security reasons, there is a timed sequence for clearing the
// WDE and changing the time-out configuration. If you don't use this
// sequence properly, you'll get unexpected results.

// Configure the Watchdog timer Control Register (WDTCSR)
  // The WDTCSR is used for configuring the time-out, mode of operation, etc

  // In order to change WDE or the pre-scaler, we need to set WDCE (This will
  // allow updates for 4 clock cycles).

  // Set the WDCE bit (bit 4) and the WDE bit (bit 3) of the WDTCSR. The WDCE
  // bit must be set in order to change WDE or the watchdog pre-scalers.
  // Setting the WDCE bit will allow updates to the pre-scalers and WDE for 4
  // clock cycles then it will be reset by hardware.
### WDTCSR 
  /**
   *  Setting the watchdog pre-scaler value with VCC = 5.0V and 16mHZ
   *  WDP3 WDP2 WDP1 WDP0 | Number of WDT | Typical Time-out at Oscillator Cycles
   *  0    0    0    0    |   2K cycles   | 16 ms
   *  0    0    0    1    |   4K cycles   | 32 ms
   *  0    0    1    0    |   8K cycles   | 64 ms
   *  0    0    1    1    |  16K cycles   | 0.125 s
   *  0    1    0    0    |  32K cycles   | 0.25 s
   *  0    1    0    1    |  64K cycles   | 0.5 s
   *  0    1    1    0    |  128K cycles  | 1.0 s
   *  0    1    1    1    |  256K cycles  | 2.0 s
   *  1    0    0    0    |  512K cycles  | 4.0 s
   *  1    0    0    1    | 1024K cycles  | 8.0 s
  */

*
**
*Italic*
**Bold**
---
[name](link)