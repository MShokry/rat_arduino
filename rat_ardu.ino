/*
* By Mahmoud Shokry.
* check the MD file for ref 
*/

#include <avr/interrupt.h>        // Library to use interrupt
#include <avr/sleep.h>            // Library for putting our arduino into sleep modes

#include <avr/power.h>
#include <avr/wdt.h>

const int PIRsensorInterrupt = 0; //interrupt 0 at arduino nano pin D2
const int PIRsensor = 2;          //interrupt 0 at arduino nano pin D2
const int LedPin = 13;            // external LED or relay connected to pin 13
const int btn_update = 9;
const int btn_config = 10;
const int btn_reset = 11;

const int esp_wake = 4;           //Reset and EN
const int esp_motion = 5;         // Motion detected 
const int esp_config = 6;         // Config button
const int esp_update = 7;         // Update button
const int esp_ack = 8;            // Done oeration and ready to sleep

volatile int lastPIRsensorState = 1;  // previous sensor state
volatile int PIRsensorState = 0;  // current state of the button

volatile int wdt_wake=0;

void wakeUpNow(){                 // Interrupt service routine or ISR  
  PIRsensorState = !lastPIRsensorState;    // we negate previous state and assign to current state
}

// Watchdog Interrupt Service. This is executed when watchdog timed out.
ISR(WDT_vect) {
  if(wdt_wake == 0) {
    // here we can implement a counter the can set the wdt_wake to true if
    // the watchdog cycle needs to run longer than the maximum of eight
    // seconds.
    wdt_wake=1;
  }
}



void Hibernate()         // here arduino is put to sleep/hibernation
{
 /*
 * SLEEP_MODE_IDLE - the lowest power saving mode
 * SLEEP_MODE_ADC
 * SLEEP_MODE_PWR_SAVE
 * SLEEP_MODE_STANDBY
 * SLEEP_MODE_PWR_DOWN - the highest power saving mode
 * "The Power-down mode saves the register contents but freezes the Oscillator, disabling all other chip functions 
 * until the next interrupt or hardware reset."  text from ATMEGA328P datasheet
 */
 set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
 ADCSRA &= ~(1 << 7);   // Disable ADC - don't forget to flip back after waking up if you need ADC in your application ADCSRA |= (1 << 7);  (From Kevin's sketch)
    
 sleep_enable();                       // enable the sleep mode function
 sleep_bod_disable();                  //to disable the Brown Out Detector (BOD) before going to sleep. 

   /*
   * attachInterrupt(A, B, C)
   * A   can be either 0 or 1 for interrupts on pin 2 or 3.  
   *
   * B   Name of a function you want to execute while in interrupt A.
   *
   * C   Trigger mode of the interrupt pin. can be:
   *             LOW        a low level trigger
   *             CHANGE     a change in level trigger
   *             RISING     a rising edge of a level trigger
   *             FALLING    a falling edge of a level trigger
   *
   * In all but the IDLE sleep modes only LOW can be used.
   * here since PIR sensor has inbuilt timer to swtich its state from OFF to ON, we are detecting its CHANGE IN STATE to control our LED/relay at pin 13. 
   * therefore, we will not need to use arduino delay timer to Set "ON time" of our LED/relay, it can be adjusted physically using potentiometer provided on PIR sensor board.
   * This further helps in using SLEEP_MODE_PWR_DOWN which is ultimate lowest power consumption mode for ATMEGA8328P chip  
   * (please note - because of onboard power regulators of arduino boards, power consumption cannot be reduced to predicted few microAmps level of bare chips. 
   * To achieve further reduction in current consumption, we will need bare ATMEGA328P chip)
   */
 attachInterrupt(PIRsensorInterrupt,wakeUpNow, CHANGE);   // Attach interrupt at pin D2  (int 0 is at pin D2  for nano, UNO)
 /*
  //Convering all to input
   for (int i = 0; i < 20; i++) {
    if(i != 13)//  because the LED/Relay is connected to digital pin 13
    pinMode(i, INPUT);
  }*/
 
 sleep_mode();                // calls function to put arduino in sleep mode
 
 sleep_disable();            // when interrupt is received, sleep mode is disabled and program execution resumes from here
 detachInterrupt(PIRsensorInterrupt);   // we detach interrupt from pin D2, to avoid further interrupts until our ISR is finished
}


// Setup the Watch Dog Timer (WDT)
void setupWatchDogTimer() {
  // The MCU Status Register (MCUSR) is used to tell the cause of the last
  // reset, such as brown-out reset, watchdog reset, etc.
  // NOTE: for security reasons, there is a timed sequence for clearing the
  // WDE and changing the time-out configuration. If you don't use this
  // sequence properly, you'll get unexpected results.

  // Clear the reset flag on the MCUSR, the WDRF bit (bit 3).
  MCUSR &= ~(1<<WDRF);

  // Configure the Watchdog timer Control Register (WDTCSR)
  // The WDTCSR is used for configuring the time-out, mode of operation, etc

  // In order to change WDE or the pre-scaler, we need to set WDCE (This will
  // allow updates for 4 clock cycles).

  // Set the WDCE bit (bit 4) and the WDE bit (bit 3) of the WDTCSR. The WDCE
  // bit must be set in order to change WDE or the watchdog pre-scalers.
  // Setting the WDCE bit will allow updates to the pre-scalers and WDE for 4
  // clock cycles then it will be reset by hardware.
  WDTCSR |= (1<<WDCE) | (1<<WDE);

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
  WDTCSR  = (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
  // Enable the WD interrupt (note: no reset).
  WDTCSR |= _BV(WDIE);
}

void setup() {
  pinMode(LedPin, OUTPUT);        // initialize pin 13 as an output pin for LED or relay etc.
  pinMode(PIRsensor, INPUT);      // define interrupt pin D2 as input to read interrupt received by PIR sensor
  pinMode(btn_config, INPUT);
  pinMode(btn_update, INPUT);
  pinMode(btn_reset, INPUT);

  pinMode(esp_wake, OUTPUT);
  pinMode(esp_update, OUTPUT);
  pinMode(esp_config, OUTPUT);
  pinMode(esp_motion, OUTPUT);
  pinMode(esp_ack, INPUT);

  digitalWrite(LedPin, LOW);
  digitalWrite(esp_motion, LOW);
  digitalWrite(esp_config, LOW);
  digitalWrite(esp_update, LOW);
  digitalWrite(esp_wake, LOW);


  //Serial.begin(115200);     // initialize serial communication only for debugging purpose
  // Serial.println("Warming up... wait for a min...");

 // delay execution of sketch for a min, to allow PIR sensor get stabilized
 for( int i = 1; i <= 10; i++){  // LED at pin 13 blinks until PIR sensor is stabilized 10 seconds
    delay(1000); 
 }
 
 // Serial.println("Ready");     // enable only for debugging purpose
 setupWatchDogTimer(); 
  
}

void loop() {
 interrupts();    // enable interrupts for Due and Nano V3

 if (PIRsensorState != lastPIRsensorState){

  if (PIRsensorState == 0) {
     digitalWrite(LedPin, LOW);
   //  Serial.print("Sleeping-");            // enable for debugging
   //  Serial.println(PIRsensorState);   // read status of interrupt pin
  }
  
  else {       
     digitalWrite(LedPin, HIGH); 
    // Serial.print("Awake-");    // enable for debugging
    //   Serial.println(PIRsensorState);  // read status of interrupt pin   enable for debugging
     delay(50);
      }
  }

   lastPIRsensorState = PIRsensorState;    // reset lastinterrupt state
   delay(50);
   Hibernate();   // go to sleep - calling sleeping function
}

