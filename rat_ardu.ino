/*
* By Mahmoud Shokry.
* check the MD file for ref 
*/

#include <avr/interrupt.h>        // Library to use interrupt
#include <avr/sleep.h>            // Library for putting our arduino into sleep modes

#include <avr/power.h>
#include <avr/wdt.h>

#define DEBUG 1
#if (DEBUG==1)
  #define PRINTDEBUGLN(STR) Serial.println(STR)
  #define PRINTDEBUG(STR) Serial.print(STR)
#else
  #define PRINTDEBUGLN(STR) /*NOTHING*/
  #define PRINTDEBUG(STR) /*NOTHING*/
#endif

const int PIRsensorInterrupt = 0; //interrupt 0 at arduino nano pin D2
const int PIRsensor = 2;          //interrupt 0 at arduino nano pin D2
const int LedPin = 13;            // external LED or relay connected to pin 13
const int btn_update = 15;
const int btn_config = 16;
const int btn_reset = 17;

const int esp_wake = 4;           //Reset and EN => To ESP
const int esp_motion = 5;         // Motion detected 
const int esp_config = 6;         // Config button
const int esp_update = 7;         // Update button
const int esp_ack = 8;            // Done oeration and ready to sleep
const int esp_ok = 14;

volatile int lastPIRsensorState = 1;  // previous sensor state
volatile int PIRsensorState = 0;  // current state of the button
bool PIRstable = false;           // Indicates if the PIR goot enough time to stable
bool alarm = false;               // Alarm activated ???

volatile int wdt_wake=0;
volatile int wtd_delay = 0;     // Delay with WDT

void wakeUpNow(){                 // Interrupt service routine or ISR  
  PIRsensorState = !lastPIRsensorState;    // we negate previous state and assign to current state
}

// Watchdog Interrupt Service. This is executed when watchdog timed out.
ISR(WDT_vect) {
  if(wdt_wake == 0) {
    // the watchdog cycle needs to run longer than the maximum of eight
    // seconds.
    if (wtd_delay){
      wtd_delay--;
    }else{
      wdt_wake=1;
    }
  }
}

void Hibernate()         // here arduino is put to sleep/hibernation
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
  // ADCSRA &= ~(1 << 7);   // Disable ADC - don't forget to flip back after waking up if you need ADC in your application ADCSRA |= (1 << 7);  (From Kevin's sketch)

  sleep_enable();                       // enable the sleep mode function
  sleep_bod_disable();                  //to disable the Brown Out Detector (BOD) before going to sleep. 

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

void LightSleep(){
  set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here
  sleep_enable(); // enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();

  sleep_mode(); // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable(); // first thing after waking from sleep:
  // disable sleep...
  
}


// Setup the Watch Dog Timer (WDT)
void setupWDTimer() {
  // Clear the reset flag on the MCUSR, the WDRF bit (bit 3).
  MCUSR &= ~(1<<WDRF);

  WDTCSR |= (1<<WDCE) | (1<<WDE);

  WDTCSR  = (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
  // Enable the WD interrupt (note: no reset).
  WDTCSR |= _BV(WDIE);
}
void setupTimer(){

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
  #if (DEBUG==1)
    Serial.begin(115200);     // Initialize serial communications
    delay(100);
    Serial.println();
    Serial.println("Author:: Mahmoud Shokry");
  #endif
  // Disable all Not needed 
  power_adc_disable();
  power_spi_disable();
  //power_timer0_disable();
  //power_timer1_disable();
  //power_timer2_disable();
  power_twi_disable();
  #if (DEBUG==0)
    power_usart_disable();
  #endif
  PRINTDEBUGLN("Warming up... wait for a min...");
  //delay execution of sketch for a min, to allow PIR sensor get stabilized
  for( int i = 1; i <= 10; i++){
    delay(1000);  // # Todo change according the crystal
  }
  PRINTDEBUGLN("Ready");     // enable only for debugging purpose
  //setupTimer();            // set up time or WDT for the delay if needed
 }

void loop() {
interrupts();    // enable interrupts for Due and Nano V3

if (PIRsensorState != lastPIRsensorState and not alarm){
  if (PIRsensorState == 1) {
    #if (DEBUG==1)
      digitalWrite(LedPin, HIGH);         // Indicator for testing
    #endif
    digitalWrite(esp_motion, HIGH);     // motion flag high
    digitalWrite(esp_wake, HIGH);       // Ready wake the esp 
    PRINTDEBUG("Awake- ");    // enable for debugging
    PRINTDEBUGLN(PIRsensorState);  // read status of interrupt pin   enable for debugging
    delay(100); // About 50 milliscond for esp to wake up
    PRINTDEBUGLN("ESP Waiting: ~100 msec each .");
    // #todo may goto sleep here for more power saving if esp sleep >10 sec
    while(!digitalRead(esp_ack)){
      PRINTDEBUG(".");
      delay(100);
    }
    PRINTDEBUGLN("ESP ack");            // enable for debugging
    if(digitalRead(esp_ok)){
      alarm = true;
      PRINTDEBUGLN("Alarm activated-");            // enable for debugging
    }else{
      alarm = false;
      PRINTDEBUGLN("Alarm Not activated-");            // enable for debugging
    }    
  }
  else {       
    
    PRINTDEBUG("Sleeping-");            // enable for debugging
    PRINTDEBUGLN(PIRsensorState);   // read status of interrupt pin
  }
}
lastPIRsensorState = PIRsensorState;    // reset lastinterrupt state
digitalWrite(LedPin, LOW);
digitalWrite(esp_motion, LOW);     // motion flag high
digitalWrite(esp_wake, LOW);       // Ready wake the esp 
delay(10);
Hibernate();   // go to sleep - calling sleeping function
}

