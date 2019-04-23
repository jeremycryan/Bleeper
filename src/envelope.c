#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "main.h"

/* Allocates and populates an array with
   the specified ADSR characteristics. The array consists of uint8_ts with
   values ranging from 0 to 255.

   A:           attack time, in seconds
   D:           decay time, in seconds
   S:           sustain time, in seconds
   R:           release time, in seconds
   sustain_amp: amplitude of the envelope during the sustain phase
*/
uint8_t* generate_envelope_asdr(double A,
                                double S,
                                double D,
                                double R,
                                uint8_t sustain_amp) {

  // Length of the return array, in samples
  double duration = A + S + D + R;
  uint32_t rate = STD_RATE;
  uint32_t length = (int)(duration * rate);
  uint8_t* return_array = malloc(sizeof(uint8_t) * length);

  // The proportion through the specific phase
  double prop_through;

  // Populate array
  for (uint32_t i = 0; i < length; i++) {

    // Attack behavior
    if (i < A * rate) {
      prop_through = 1.0 * i / (A * rate);
      return_array[i] = (int)(prop_through * 255);
    }

    // Decay behavior
    else if (i >= A * rate && i < (A + D) * rate) {
      prop_through = 1.0 * (i - A*rate) / (D * rate);
      return_array[i] = (int)(255 - (prop_through * (255 - sustain_amp)));
    }

    // Sustain behavior
    else if (i >= (A + D) * rate && i < (A + D + S) * rate) {
      return_array[i] = sustain_amp;
    }

    // Release behavior
    else if (i >= (A + D + S) * rate) {
      prop_through = 1.0 * (i - (A + D + S) * rate) / (R * rate);
      return_array[i] = (int)(sustain_amp * (1.0 - prop_through));
    }

  }

  return return_array;

}
