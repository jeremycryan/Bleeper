#include "main.h"

uint8_t* pins;

/* Set all output pins to digital write,
   and instantiate an array for them.
*/
void initialize_pins() {

  pins = (uint8_t*)malloc(sizeof(uint8_t)*8);

  // Put each pin number into the array.
  // Change here if inputs change.
  pins[0] = 0;  //  DAC LSB
  pins[1] = 1;
  pins[2] = 2;
  pins[3] = 3;
  pins[4] = 4;
  pins[5] = 5;
  pins[6] = 6;
  pins[7] = 7;  //  DAC MSB

  // Set all pins to output mode and set them low
  for (uint8_t i; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }

}

/* Set each digital pin for the DAC such that
   they create the 8-bit numeber determined by
   val.
*/
void set_speaker(uint8_t val) {

  for (uint8_t i = 0; i < 8; i++) {
    uint8_t this_digit = val % 2;
    val = val >> 1;
    digitalWrite(pins[i], this_digit);
  }

}
