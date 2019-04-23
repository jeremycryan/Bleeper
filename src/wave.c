#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "main.h"

/*  Create a Wave structure of length 'length', and
    initialize its memory to be all zero.
*/
Wave* make_wave(uint32_t length) {

  // Allocate memory for data and wave object
  Wave* wave_ptr = malloc(sizeof(Wave));
  int8_t* data_ptr = calloc(length, sizeof(int8_t));
  uint8_t* envelope = malloc(sizeof(uint8_t) * length);

  // Initialize envelope to start at all maximum values
  for (int i = 0; i < length; i++) envelope[i] = 255;

  // Assign values to attributes
  wave_ptr -> data = data_ptr;
  wave_ptr -> envelope = envelope;
  wave_ptr -> length = length;
  wave_ptr -> rate = 44100;

  // Return new object
  return wave_ptr;

}


/*  Frees a Wave object.
*/
void free_wave(Wave* wave_ptr) {

  free(wave_ptr -> data);
  free(wave_ptr -> envelope);
  free(wave_ptr);

}
