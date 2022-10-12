/*
    
   PRINTER STATUS LED CONTROLLER
   -----------------------------
   
                         ATTINY25
                         --------
                 RESET -| *      |- VCC
                        |        |
             (N/C) PB3 -|        |- PB2 (PWR_PIN)
                        |        |
       (SENSE_PIN) PB4 -|        |- PB1 (READY_PIN)
                        |        |
                   GND -|        |- PB0 (N/C)
                         --------
   
    VCC       -  TO 5V WITH 0.1uF DECOUPLING CAPACITOR
    GND       -  TO GND
    RESET     -  TO 5V WITH A 10K SERIES PULL-UP RESISTOR 
    PB4       -  TO OUTPUT PIN ON ORANGE PI WITH 10K PULL-DOWN RESISTOR

    PIN_PB3  (PIN 2)  -  N/C
    PIN_PB4  (PIN 3)  -  SENSE_PIN
    PIN_PB0  (PIN 5)  -  N/C
    PIN_PB1  (PIN 6)  -  READY_PIN
    PIN_PB2  (PIN 7)  -  PWR_PIN
    
*/

#include <avr/interrupt.h>

const byte INT_PIN   = PB4;
const byte LED_PIN   = PB1;
const byte LED_PIN_2 = PB2;


void setup() {
  pinMode(INT_PIN, INPUT);                        // Input for 'READY' state status pin on Orange Pi
  pinMode(LED_PIN, OUTPUT);                       // First output pin for status LED
  pinMode(LED_PIN_2, OUTPUT);                     // Second output pin for status LED
  digitalWrite(LED_PIN, LOW);                     // Turn on STATUS LED to indicate PWR ON status
  digitalWrite(LED_PIN_2, HIGH);
  cli();                                          // Disable global interrupts
      GIMSK |= (1 << PCIE);                       // Enable global interrupt mask
      PCMSK |= (1 << PCINT4);                     // Ebable interupt on INT pin
  sei();                                          // Enable global interrupts
  update_status ();
}

void loop() {}

void update_status () {
  if (digitalRead(INT_PIN)) {                     // If printer is ready (INT PIN is HIGH)
      digitalWrite(LED_PIN_2, LOW);               // Turn on STATUS LED in one direction to indicate READY status
      digitalWrite(LED_PIN, HIGH);
  } else {                                        // Else printer is not ready (INT PIN is LOW)
      digitalWrite(LED_PIN, LOW);                 // Turn on STATUS LED in the other direction to indicate PWR ON status
      digitalWrite(LED_PIN_2, HIGH);
  }
}

ISR (PCINT0_vect) {                               // ISR interrupt for INT PIN (PB4, PCINT4)
  update_status ();
}
