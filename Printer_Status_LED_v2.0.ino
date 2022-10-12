/*
    
   PRINTER STATUS LED CONTROLLER
   -----------------------------
   
                         ATTINY25
                         --------
                 RESET -| *      |- VCC
                        |        |
       (INT_PIN_2) PB3 -|        |- PB2 (LED_PIN_2)
                        |        |
       (INT_PIN_1) PB4 -|        |- PB1 (LED_PIN_1)
                        |        |
                   GND -|        |- PB0 (N/C)
                         --------
   
    VCC       -  TO 5V WITH 0.1uF DECOUPLING CAPACITOR
    GND       -  TO GND
    RESET     -  TO 5V WITH A 10K SERIES PULL-UP RESISTOR 
    PB3       -  TO PIN PA5 ON SKR-3 (EXP.2) WITH 10K PULL-DOWN RESISTOR
    PB4       -  TO PIN PA5 ON SKR-3 (EXP.2) WITH 10K PULL-DOWN RESISTOR

    PIN_PB3  (PIN 2)  -  INT_PIN_2 -> SKR-3 EZ PIN PA5
    PIN_PB4  (PIN 3)  -  INT_PIN_1 -> SKR-3 EZ PIN PA6
    PIN_PB0  (PIN 5)  -  N/C
    PIN_PB1  (PIN 6)  -  LED_PIN_1
    PIN_PB2  (PIN 7)  -  LED_PIN_2
    
*/

#include <avr/interrupt.h>

const byte INT_PIN_1 = PB3;
const byte INT_PIN_2 = PB4;
const byte LED_PIN_1 = PB1;
const byte LED_PIN_2 = PB2;


void setup() {
  pinMode(INT_PIN_1, INPUT);
  pinMode(INT_PIN_2, INPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  digitalWrite(LED_PIN_1, LOW);                   
  digitalWrite(LED_PIN_2, LOW);
  cli();                                          // Disable global interrupts
      GIMSK |= (1 << PCIE);                       // Enable global interrupt mask
      PCMSK |= (1 << PCINT3);                     // Ebable interupt on INT_1 pin
      PCMSK |= (1 << PCINT4);                     // Ebable interupt on INT_2 pin
  sei();                                          // Enable global interrupts
  update_status ();
}

void loop() {}

void update_status () {
  if (!digitalRead(INT_PIN_1) && !digitalRead(INT_PIN_2)) {
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, LOW);   
   } else if (digitalRead(INT_PIN_1) && !digitalRead(INT_PIN_2)) {
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, HIGH);
   } else if (!digitalRead(INT_PIN_1) && digitalRead(INT_PIN_2)) {           
      digitalWrite(LED_PIN_2, LOW);
      digitalWrite(LED_PIN_1, HIGH);
   } else {
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, LOW);   
   }
}

ISR (PCINT0_vect) {                               // ISR interrupt for INT PINS (PB3 & PB4)
  update_status ();
}
