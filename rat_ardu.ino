/*
* By Mahmoud Shokry.
* check the MD file for ref 
*/

#include <avr/interrupt.h>        // Library to use interrupt
#include <avr/sleep.h>            // Library for putting our arduino into sleep modes

#include <avr/power.h>
#include <avr/wdt.h>
#include "PinChangeInterrupt.h"


#define DEBUG 0
#if (DEBUG==1)
  #define PRINTDEBUGLN(STR) Serial.println(STR)
  #define PRINTDEBUG(STR) Serial.print(STR)
#else
  #define PRINTDEBUGLN(STR) /*NOTHING*/
  #define PRINTDEBUG(STR) /*NOTHING*/
#endif

// Dont use SS,MOSI,MISO,SCK ; D10,11,12,13  // RX,TX ; 

const int PIRsensorInterrupt = 0 ;//interrupt 0 at arduino nano pin D2
const int PIRsensor          = 2 ;//interrupt 0 at arduino nano pin D2
const int BUZZER             = 9 ;// Feedback for human interface
const int LedPin             = 13;// external LED or relay connected to pin 13
const int btn_update         = A1;
const int btn_config         = A2;
const int btn_reset          = A3;
const int btn_test           = A4;
bool UPDATE_FLAG        = false;
bool CONFIG_FLAG        = false;
bool RESET_FLAG         = false;
bool TEST_FLAG          = false;

const int esp_wake    = 4;           //Reset and EN => To ESP
const int esp_motion  = 5;         // Motion detected 
const int esp_config  = 6;         // Config button
const int esp_update  = 7;         // Update button
const int esp_ack     = 8;            // Done oeration and ready to sleep
const int esp_ok      = A0;

const unsigned long max_wait_server = 1800; //3000 *100msec = 5 Minutes 
const unsigned long max_wait_config = 3000; //6000 *100msec = 15 Minutes 
unsigned long timer = 0;

volatile int lastPIRsensorState = 1;  // previous sensor state
volatile int PIRsensorState = 0;  // current state of the button
bool PIRstable = false;           // Indicates if the PIR goot enough time to stable
bool alarm = false;               // Alarm activated ???

volatile int wdt_wake=0;
volatile int wtd_delay = 0;     // Delay with WDT


/*************************************************************************************
 *  POR Wake UP
 *************************************************************************************/
void wakeUpNow(){                 // Interrupt service routine or ISR  
  PIRsensorState = !lastPIRsensorState;    // we negate previous state and assign to current state
}
/*************************************************************************************
 *  Change in PCINT for the buttons
 *************************************************************************************/
void reset_isr(void) {
  RESET_FLAG = true;
}
void config_isr(void) {
  CONFIG_FLAG = true;
}
void update_isr(void) {
  UPDATE_FLAG = true;
}
void test_isr(void){
  TEST_FLAG = true;
}
/*************************************************************************************
 *  WDT Wake up ISR
 *************************************************************************************/
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

/*************************************************************************************
 *  Deep Sleep
 *************************************************************************************/

void Hibernate()         // here arduino is put to sleep/hibernation
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
  // ADCSRA &= ~(1 << 7);   // Disable ADC - don't forget to flip back after waking up if you need ADC in your application ADCSRA |= (1 << 7);  (From Kevin's sketch)

  sleep_enable();                       // enable the sleep mode function
  sleep_bod_disable();                  //to disable the Brown Out Detector (BOD) before going to sleep. 
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_update), update_isr, FALLING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_config), config_isr, FALLING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_reset), reset_isr, FALLING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_test), test_isr, FALLING);
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
  detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_reset));
  detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_config));
  detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_reset));
  detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(btn_test));
}

/*************************************************************************************
 *  Light Sleep version
 *************************************************************************************/

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

/*************************************************************************************
 *  WDT Setup as A Timer
 *************************************************************************************/

// Setup the Watch Dog Timer (WDT)
void setupWDTimer() {
  // Clear the reset flag on the MCUSR, the WDRF bit (bit 3).
  MCUSR &= ~(1<<WDRF);

  WDTCSR |= (1<<WDCE) | (1<<WDE);

  WDTCSR  = (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
  // Enable the WD interrupt (note: no reset).
  WDTCSR |= _BV(WDIE);
}
/*************************************************************************************
 *  Timer as Timing Function
 *************************************************************************************/
void setupTimer(){

}

/*************************************************************************************
 *  Human feedback Function
 *************************************************************************************/

void buzzing(int times=2,int delayh = 200,int delayl = 100,int red =0 ){
  for(int i=0;i<times;i++){
    digitalWrite(BUZZER, HIGH);         // Turns BUZZER ON
    delay(delayh);
    digitalWrite(BUZZER, LOW);         // Turns Relay Off
    delay(delayl);
  }
}

/*************************************************************************************
 *  Main Setup
 *************************************************************************************/
void setup() {

  pinMode(esp_wake, OUTPUT);
  digitalWrite(esp_wake, LOW);

  pinMode(PIRsensor, INPUT);      // define interrupt pin D2 as input to read interrupt received by PIR sensor
  pinMode(btn_config, INPUT_PULLUP);
  pinMode(btn_update, INPUT_PULLUP);
  pinMode(btn_reset, INPUT_PULLUP);
  pinMode(btn_test, INPUT_PULLUP);
  pinMode(esp_ack, INPUT);

  pinMode(LedPin, OUTPUT);        // initialize pin 13 as an output pin for LED.
  pinMode(esp_update, OUTPUT);
  pinMode(esp_config, OUTPUT);
  pinMode(esp_motion, OUTPUT);

  digitalWrite(LedPin, LOW);
  digitalWrite(esp_motion, LOW);
  digitalWrite(esp_config, LOW);
  digitalWrite(esp_update, LOW);
  
  pinMode(BUZZER, OUTPUT);        // initialize pin 13 as an output pin for LED.
  digitalWrite(BUZZER, LOW);

  
  
  #if (DEBUG==1)
    power_usart0_enable();
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
    //power_usart_disable();
    power_usart0_disable();
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

if ((PIRsensorState != lastPIRsensorState and not alarm) or TEST_FLAG){
  if (PIRsensorState == 1 or TEST_FLAG) { //Activate Process on High pulse of motion
    #if (DEBUG==1)
      digitalWrite(LedPin, HIGH);         // Indicator for testing
    #endif
    digitalWrite(esp_motion, HIGH);     // motion flag high
    if(TEST_FLAG){
      PRINTDEBUGLN("Alarm Testing");
      digitalWrite(esp_update, HIGH);
    }
    delay(20);
    digitalWrite(esp_wake, HIGH);       // Ready wake the esp 
    PRINTDEBUG("Awake- ");    // enable for debugging
    PRINTDEBUGLN(PIRsensorState);  // read status of interrupt pin   enable for debugging
    delay(200); // About 200 milliscond for esp to wake up and all stable
    PRINTDEBUGLN("ESP Waiting: ~100 msec each .");
    // #todo may goto sleep here for more power saving if esp sleep >10 sec
    timer = max_wait_server;
    while(!digitalRead(esp_ack) and timer>0){
      PRINTDEBUG(".");
      delay(100);
      timer --;
    }
    PRINTDEBUGLN("ESP ack");            // enable for debugging
    if(not TEST_FLAG){                  // If testing Dont consider sending done or not
      if(digitalRead(esp_ok)){
        alarm = true;
        PRINTDEBUGLN("Alarm activated-");            // enable for debugging
      }else{
        alarm = false;
        PRINTDEBUGLN("Alarm Not activated-");            // enable for debugging
        buzzing(2,200,200);
      } 
    }else{ //On test reset the alarm
      alarm = false;
    }   
  }
  else {       
    PRINTDEBUG("Sleeping-");            // enable for debugging
    PRINTDEBUGLN(PIRsensorState);   // read status of interrupt pin
  }
digitalWrite(esp_wake, LOW);
digitalWrite(esp_motion, LOW);     // motion flag high
digitalWrite(esp_update, LOW);
lastPIRsensorState = PIRsensorState;    // reset lastinterrupt state
TEST_FLAG = false;
}
if(UPDATE_FLAG){
  digitalWrite(esp_update, HIGH);     // motion flag high
  delay(20);
  digitalWrite(esp_wake, HIGH);       // Ready wake the esp 
  PRINTDEBUG("Awake- For Upadte");    // enable for debugging
  delay(200); // About 200 milliscond for esp to wake up and all stable
  PRINTDEBUGLN("ESP Waiting: ~100 msec each .");
  // #todo may goto sleep here for more power saving if esp sleep >10 sec
  timer = max_wait_server;
  while(!digitalRead(esp_ack) and timer>0){
    PRINTDEBUG(".");
    PRINTDEBUG(--timer);
    delay(100);
  }
  PRINTDEBUGLN("");
  PRINTDEBUGLN("ESP ack");            // enable for debugging
  if(digitalRead(esp_ok)){
        PRINTDEBUGLN("Done Update-");            // enable for debugging
        buzzing(1,200,200);
  } else{
      PRINTDEBUGLN("Error Update-");            // enable for debugging
        buzzing(2,200,200);
  }
  digitalWrite(esp_wake, LOW);
  digitalWrite(esp_update, LOW);
  UPDATE_FLAG = false;
}
if(CONFIG_FLAG){
  digitalWrite(esp_config, HIGH);     // motion flag high
  delay(20);
  digitalWrite(esp_wake, HIGH);       // Ready wake the esp 
  PRINTDEBUG("Awake- For Config");    // enable for debugging
  delay(200); // About 200 milliscond for esp to wake up and all stable
  PRINTDEBUGLN("ESP Waiting: ~100 msec each .");
  // #todo may goto sleep here for more power saving if esp sleep >10 sec
  timer = max_wait_config;
  while(!digitalRead(esp_ack) and timer>0){
    PRINTDEBUG(".");
    PRINTDEBUG(--timer);
    delay(100);
  }
  PRINTDEBUGLN("");
  if(digitalRead(esp_ok)){
        PRINTDEBUGLN("Done Config-");            // enable for debugging
        buzzing(1,200,200);
  } else{
    PRINTDEBUGLN("Error Config-");            // enable for debugging
        buzzing(2,200,200);
  }
  PRINTDEBUGLN("ESP ack");            // enable for debugging
  digitalWrite(esp_wake, LOW);
  digitalWrite(esp_config, LOW);  
  CONFIG_FLAG = false;   
}
if(RESET_FLAG){
 PRINTDEBUGLN("Soft Reset");            // enable for debugging
 alarm = false;
 RESET_FLAG = false;
}
digitalWrite(LedPin, LOW);
digitalWrite(esp_wake, LOW);       // Ready wake the esp 
delay(10);
Hibernate();   // go to sleep - calling sleeping function
}

