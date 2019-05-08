#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "main.h"

/* Returns a pointer to an allocated envelope with the specified ADSR
   characteristics, and amplitude sustain_amp during the sustain phase.
*/
Envelope* make_envelope(double A, double D, double S, double R,
                        uint8_t sustain_amp) {

  Envelope* e = (Envelope*)malloc(sizeof(Envelope));
  e -> A = A;
  e -> D = D;
  e -> S = S;
  e -> R = R;
  e -> sustain_amp = sustain_amp;
  return e;

}

/* Frees an envelope object.
*/
void free_envelope(Envelope* e) {
  free(e);
}

/* Gets the value of an envelope object at time t
*/
uint8_t get_env_val(Envelope* e, double t) {


  double A = e -> A;
  double D = e -> D;
  double S = e -> S;
  double R = e -> R;
  uint8_t sustain_amp = e -> sustain_amp;

  // Length of the return array, in samples
  double duration = A + D + S + R;
  uint32_t rate = STD_RATE;
  uint32_t length = (int)(duration * rate);

  // The proportion through the specific phase
  double prop_through;

  uint32_t i = (int)(t * STD_RATE);

  // Attack behavior
  if (i < A * rate) {
    prop_through = 1.0 * i / (A * rate);
    return (uint8_t)(prop_through * 255);
  }

  // Decay behavior
  else if (i >= A * rate && i < (A + D) * rate) {
    prop_through = 1.0 * (i - A*rate) / (D * rate);
    return (uint8_t)(255 - (prop_through * (255 - sustain_amp)));
  }

  // Sustain behavior
  else if (i >= (A + D) * rate && i < (A + D + S) * rate) {
    return sustain_amp;
  }

  // Release behavior
  else if (i >= (A + D + S) * rate) {
    prop_through = 1.0 * (i - (A + D + S) * rate) / (R * rate);
    return (uint8_t)(sustain_amp * (1.0 - prop_through));
  }

  return 0;

}
