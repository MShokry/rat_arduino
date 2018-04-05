
## Refrances

By :: Mahmoud Shokry :: 

PIR motion detector sensor with Arduino nano in ultimate power saving mode 

**reducing power consumption almost by 87% (1.7mA) of normal operation consumption(13mA)**

[Thanks to:- ](http://playground.arduino.cc/Learning/ArduinoSleepCode) for information about sleep.h library and modes

for useful explanation of  "Low Power Arduino! Deep Sleep Tutorial" for bare [Arduino uno chip](http://www.kevindarrah.com/download/arduino_code/LowPowerVideo.ino)

[Thanks to :](https://theelectromania.blogspot.com.eg/2016/02/pir-motion-detector-with-arduino.html) Afull project for PIR + Arduino 

The [Datasheet](http://www.atmel.com/images/atmel-8271-8-bit-avr-microcontroller-atmega48a-48pa-88a-88pa-168a-168pa-328-328p_datasheet_complete.pdf) Is here

** NOTE : ** when coming back from POWER-DOWN mode, it takes a bit until the system is functional at 100%!! (typically <1sec) 

[ESP](https://cdn-shop.adafruit.com/product-files/2471/0A-ESP8266__Datasheet__EN_v4.3.pdf)
[ESP NODEMCU PIN](https://i.stack.imgur.com/w2Scr.jpg)
--- 

## Sleeping Modes helper function Hibernate  

###  set_sleep_mode

```
  SLEEP_MODE_IDLE - the lowest power saving mode
  SLEEP_MODE_ADC
  SLEEP_MODE_PWR_SAVE
  SLEEP_MODE_STANDBY
  SLEEP_MODE_PWR_DOWN - the highest power saving mode
 ```
  
The Power-down mode saves the register contents but freezes the Oscillator, disabling all other chip functions 
until the next interrupt or hardware reset."  text from ATMEGA328P datasheet

### AttachInterrupt(A, B, C)

```
    A   can be either 0 or 1 for interrupts on pin 2 or 3.  
   
    B   Name of a function you want to execute while in interrupt A.
   
    C   Trigger mode of the interrupt pin. can be:
                LOW        a low level trigger
                CHANGE     a change in level trigger
                RISING     a rising edge of a level trigger
                FALLING    a falling edge of a level trigger
 
```

```
 In all but the IDLE sleep modes only LOW can be used.
    here since PIR sensor has inbuilt timer to swtich its state from OFF to ON, we are detecting its CHANGE IN STATE to control our LED/relay at pin 13. 
    therefore, we will not need to use arduino delay timer to Set "ON time" of our LED/relay, it can be adjusted physically using potentiometer provided on PIR sensor board.
    This further helps in using SLEEP_MODE_PWR_DOWN which is ultimate lowest power consumption mode for ATMEGA8328P chip  
    (please note - because of onboard power regulators of arduino boards, power consumption cannot be reduced to predicted few microAmps level of bare chips. 
    To achieve further reduction in current consumption, we will need bare ATMEGA328P chip)
```
---

## WDT

The MCU Status Register (MCUSR) is used to tell the cause of the last
reset, such as brown-out reset, watchdog reset, etc.
NOTE: for security reasons, there is a timed sequence for clearing the
WDE and changing the time-out configuration. If you don't use this
sequence properly, you'll get unexpected results.

Configure the Watchdog timer Control Register (WDTCSR)
The WDTCSR is used for configuring the time-out, mode of operation, etc

In order to change WDE or the pre-scaler, we need to set WDCE (This will
allow updates for 4 clock cycles).

Set the WDCE bit (bit 4) and the WDE bit (bit 3) of the WDTCSR.
The WDCE bit must be set in order to change WDE or the watchdog pre-scalers.
Setting the WDCE bit will allow updates to the pre-scalers and WDE for 4 clock cycles then it will be reset by hardware.

### WDTCSR 
```
Setting the watchdog pre-scaler value with VCC = 5.0V and 16mHZ
WDP3 WDP2 WDP1 WDP0 | Number of WDT | Typical Time-out at Oscillator Cycles

0    0    0    0    |   2K cycles   | 16 ms
0    0    0    1    |   4K cycles   | 32 ms
0    0    1    0    |   8K cycles   | 64 ms
0    0    1    1    |  16K cycles   | 0.125 s
0    1    0    0    |  32K cycles   | 0.25 s
0    1    0    1    |  64K cycles   | 0.5 s
0    1    1    0    |  128K cycles  | 1.0 s
0    1    1    1    |  256K cycles  | 2.0 s
1    0    0    0    |  512K cycles  | 4.0 s
1    0    0    1    | 1024K cycles  | 8.0 s
```
---
# Ideas

PIR -(int)-> Nano <--(Wake,Motion,config,Update)--> ESP
3btn -------^     

# ToDo

**Project** started 07Feb2017

### Spex
---
- [x] Motion sensor <br>
- [x] Battery powered <br>
- [x] Battery Level <br>
- [x] Buttons, Update, Config, Test, Reset <br>
- [ ] Buzzer feedback

### Battery 
---
:negative_squared_cross_mark: LDO (SPX3819, HT7333, XC6203) [Baterry Serial Resistor] <br>
:negative_squared_cross_mark: High Capacitor LOW ESR <br>
:negative_squared_cross_mark: Buck/Boost MC33063 ( 0.9:6v => 2.37$/Unit) <br>
:negative_squared_cross_mark: LiFePo4 battaries 3.6V 5$ <br>
:negative_squared_cross_mark: Battery Shield USB-Lithium 3.3V <br>
- [x] Direct connect battery 3 bat <br>

### Rat Arduino Atmega Part
---
- [x]  PIR Sensor Connected to intterupt <br>
- [x] Motion detected 1st time Wakeup ESP <br>
- [x] wakeup on chnage of btn [PCINT](https://playground.arduino.cc/Main/PcInt) [used LIB](https://github.com/NicoHood/PinChangeInterrupt) <br>

- [ ] Sleep until ESP is DONE  ?? <br>
- [ ] change F_CPU to 1MHZ or 4MHZ => PROWN, Delay and speed check <br>
- [ ] Delay with Timer or WDT LightSleep() or setupWatchDogTimer() :negative_squared_cross_mark: Not important If clock is 1 MHZ no need <br>

#### Limitations
---
- [o] btn as interrupt in Power_down mode only (INT0,INT1,WDT) is working. <br>
- [n] Arduino @16MHZ normal + POR @ 2.7v<br>
- [v] motion stable @ > 4V<br>

### Rat Arduino ESP Part 
---
[ESP PIN default state](http://rabbithole.wwwdotorg.org/2017/03/28/esp8266-gpio.html)<br>
- [x] Wakeup init WIFI and connect<br>
- [x] send to server (Battery level,Alarm) <==> [MAX17043 || Read_VCC function] NEEDS Some testing<br>
- [x] WIFI OFF - [] Moderated sleep 10uA @ Sleep<br>
- [x] Raise Flag for nono operations ok <br>
- [x] Power off from arduino <br>
- [ ] Names online not local Domain + URL => to be tested
- [ ] Modify the lib Remove No Scan , [ ] Password for login, [ ] Label Names before input
- [ ] Test @ Test
- [ ] Rename place to name
- [ ] 5 Min for config as a max
- [ ] Default Name and servers for update and Max Len
- [ ] Update Config Button to request the data from the server
- [ ] Lower WIFI RF Power 120mA @ Tx 13dpm, 56mA @ Rx 1024byte <br>

:bangbang: What if server not connected or Wrong response ,... <br>
:bangbang: What if esp raised the flag and not off or dealy to answer max retry time 10 minutes <br>

Wifi configuration [wifi config](https://github.com/tzapu/WiFiManager/blob/master/examples/AutoConnectWithFSParametersAndCustomIP/AutoConnectWithFSParametersAndCustomIP.ino) **OR** Wifi-Multi <br>

### Rat Eagle PCB Design
---
- [x] ESP EN by default low by resitor 10K to ground <br>
- [ ] add 5V power source
- [ ] CAP to the button, Button to ground <br>
- [ ] Keep out the programming CHECK code<br>
- [ ] Power off the PIR ??? Trainsitor <br>
- [ ] Buzzer for feedback or server<br>

### PCB Prototyping and Aseembly 

[ESP Programming](http://www.instructables.com/id/ESP8266-a-Complete-Beginners-Guide-IOT/)

### Testing

### Other
- [ ] API documentation
- [ ] 100 Unit Price
---
:heavy_check_mark: Done
- [x] Done
:negative_squared_cross_mark: Not accepted
:black_square_button: Todo
:bangbang: Top Urgent
:heavy_exclamation_mark: Urgent