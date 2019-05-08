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

  // Assign values to attributes
  wave_ptr -> data = data_ptr;
  wave_ptr -> envelope = NULL;
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
