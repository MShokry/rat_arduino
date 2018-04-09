
# Refrances

By :: Mahmoud Shokry :: <br>

**reducing power consumption almost by 87% (1.7mA) of normal operation consumption(13mA)** <br>
## AtmegaPIR motion detector sensor with Arduino nano in ultimate power saving mode <br>
[Thanks to :](https://theelectromania.blogspot.com.eg/2016/02/pir-motion-detector-with-arduino.html) Afull project for PIR + Arduino <br>
[Thanks to:- ](http://playground.arduino.cc/Learning/ArduinoSleepCode) for information about sleep.h library and modes <br>
for useful explanation of  "Low Power Arduino! Deep Sleep Tutorial" for bare [Arduino uno chip](http://www.kevindarrah.com/download/arduino_code/LowPowerVideo.ino) <br>
The [Atmega 328p Datasheet](http://www.atmel.com/images/atmel-8271-8-bit-avr-microcontroller-atmega48a-48pa-88a-88pa-168a-168pa-328-328p_datasheet_complete.pdf) <br>
**NOTE :** when coming back from POWER-DOWN mode, it takes a bit until the system is functional at 100%!! (typically <1sec) <br>

## ESP

ESP [Data sheet](https://cdn-shop.adafruit.com/product-files/2471/0A-ESP8266__Datasheet__EN_v4.3.pdf) <br>
ESP [NODEMCU PIN](https://i.stack.imgur.com/w2Scr.jpg) <br>
ESP [PIN default state](http://rabbithole.wwwdotorg.org/2017/03/28/esp8266-gpio.html) <br>
--- 

# Ideas

PIR -(int)-> Nano <--(Wake,Motion,config,Update)--> ESP <br>
3btn -------^     

# Programing
## Tools:
- [FTDI Basic Breakout - 5V/3.3V (USB to Serial TTL) - Blue Board](http://ram-e-shop.com/oscmax/catalog/product_info.php?products_id=1797)
- [FTDI Basic Breakout - 5V/3.3V (USB to Serial TTL) - Blue Board](http://ram-e-shop.com/oscmax/catalog/product_info.php?products_id=3707)
- [USBasp AVR programmer](http://ram-e-shop.com/oscmax/catalog/product_info.php?products_id=2850)

# API
- When Power on : Soft Start takes 30 second to make sure everything is stable
- When motion detected All awake and wifi connected, The data sent to Server is, [name] => Nest12, [msg] => alarm,[v] => 2964,[mac] => 5C:CF:7F:A4:2C:13 ==> All data sent as A GET request with [User-Agent: ratnest] in headers :next_track_button: If the request reached the server then an internal alarm flag is activated so no other alarm will occure elese when **Reset** button is presed
- When **Test** Button pressed The data sent to Server is, [name] => Nest12, [msg] => test,[v] => 2964,[mac] => 5C:CF:7F:A4:2C:13
- When **Update** Button pressed The data sent to Server is, [name] => Nest12, [msg] => config,[v] => 2964,[mac] => 5C:CF:7F:A4:2C:13 and the data from the server should be [Link](http://7elol.com//update/config.php?name=Nest1&msg=config&v=2955&mac=5C:CF:7F:A4:2C:13)
```
{"ssid":"airlive","ssid_pwd":"password","update_server":"7elol.com","update_server_page":"\/update\/config.php","server":"7elol.com","server_page":"\/update\/rfid.php","place":"Nest12","ip":"192.168.0.15","gateway":"192.168.0.1","subnet":"255.255.255.0"}
```

- When **Config** Button pressed an access paint named **Rat_nest** with password **password** then the operator need to access the following [page](192.168.4.1) and follow the instructions
```
'ssid' => The Wifi SSID,
'ssid_pwd' => The Wifi Password
'update_server'=> server name like [google.com]
'update_server_page'=> The route to the page [/conf/config.aspx]
'server'=>Backend server [google.com]
'server_page'=> The route to the page [/request/index.aspx]
'place'=> The trap name 
'ip'=> Trap Ip in the lan
'gateway'=> Gateway 
'subnet'=> Subnet
```

- for all operations the buzzer will give one buzzer on operation done and twice on error 


# ToDo

**Project** started 07Feb2017

### Spex
---
- [x] Motion sensor 
- [x] Battery powered 
- [x] Battery Level 
- [x] Buttons, Update, Config, Test, Reset 
- [x] Buzzer feedback

### Battery 
---
:negative_squared_cross_mark: LDO (SPX3819, HT7333, XC6203) [Baterry Serial Resistor] 
:negative_squared_cross_mark: High Capacitor LOW ESR 
:negative_squared_cross_mark: Buck/Boost MC33063 ( 0.9:6v => 2.37$/Unit) 
:negative_squared_cross_mark: LiFePo4 battaries 3.6V 5$ 
:negative_squared_cross_mark: Battery Shield USB-Lithium 3.3V 
- [x] Direct connect battery 3 bat 

### Rat Arduino Atmega Part
---
- [x] PIR Sensor Connected to intterupt 
- [x] Motion detected 1st time Wakeup ESP 
- [x] wakeup on chnage of btn [PCINT](https://playground.arduino.cc/Main/PcInt) [used LIB](https://github.com/NicoHood/PinChangeInterrupt) 
- [x] Feedback Unit Buzzing(2 if not activated, @Config 1 Buzz OK, )
- [x] btn as interrupt in Power_down mode only (INT0,INT1,WDT) is working. [Source](#attachinterrupta-b-c)
- [x] change F_CPU to 1MHZ or 4MHZ => PROWN, Delay and speed check  [Using MiniCore](https://github.com/MCUdude/MiniCore)

- [ ] ?Sleep until ESP is DONE
- [ ] ?Delay with Timer or WDT LightSleep() or setupWatchDogTimer() :negative_squared_cross_mark: Not important If clock is 1 MHZ no need 

#### Limitations
---
- [x] Arduino @16MHZ normal + POR @ 2.7v
- [x] motion stable @ > 4V

### Rat Arduino ESP Part 
---

- [x] Wakeup init WIFI and connect [Sleeping Modes](#set_sleep_mode)
- [x] send to server (Battery level,Alarm) <==> [MAX17043 || Read_VCC function] NEEDS Some testing
- [x] WIFI OFF 
- [x] Moderated sleep 10uA @ Sleep 
- [x] Raise Flag for nono operations ok 
- [x] Power off from arduino 
- [x] Names online not local Domain + URL => to be tested
- [x] Modify the lib Remove No Scan , :heavy_exclamation_mark: WIFI Manager Change HTTP_PORTAL_OPTIONS :next_track_button: Removed Scan button
- [x] Label Names before input , :heavy_exclamation_mark: WIFI Manager Change HTTP_FORM_PARAM :next_track_button: add name {p} :
- [x] msg=Test @ Test
- [x] Rename place to name
- [x] 5 Min for config as a max 3 for sending
- [x] Update Config Button to request the data from the server
- [x] Esp light Sleep to keep flag high
- [ ] Default Name and servers for update and Max Len

- [ ] Lower WIFI RF Power 120mA @ Tx 13dpm, 56mA @ Rx 1024byte 
- [ ] Password for login,

- [x] What if server not connected => Buzzing twice or Wrong response => No alarm set,... 
:bangbang: What if esp raised the flag and not off or dealy to answer max retry time 10 minutes 

Wifi configuration [wifi config](https://github.com/tzapu/WiFiManager/blob/master/examples/AutoConnectWithFSParametersAndCustomIP/AutoConnectWithFSParametersAndCustomIP.ino) **OR** Wifi-Multi 

### Rat Eagle PCB Design
---
- [x] ESP EN by default low by resitor 10K to ground 
- [x] add 5V power source LDO and 3.3 Normal
- [x] add Jumper for power source 
- [x] Buzzer for feedback or server

- [ ] CAP to the button, Button to ground 
- [ ] Keep out the programming CHECK code
- [ ] Power off the PIR ??? Trainsitor 

### PCB Prototyping and Aseembly 

ESP [Programming](http://www.instructables.com/id/ESP8266-a-Complete-Beginners-Guide-IOT/) <br>
-U lfuse:w:0x62:m -U hfuse:w:0xd6:m -U efuse:w:0xfe:m <br>
(https://tosiek.pl/usbasp-v2-0-warning-cannot-set-sck-period/)
(https://jlcpcb.com)
(https://www.smart-prototyping.com)
(https://www.pcbway.com/orderonline.aspx)


### Testing

### Other
- [ ] API documentation
- [x] 100 Unit Price
---
:heavy_check_mark: Done
- [x] Done
:negative_squared_cross_mark: Not accepted
:black_square_button: Todo
:bangbang: Top Urgent
:heavy_exclamation_mark: Urgent


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