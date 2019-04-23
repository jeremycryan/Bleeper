#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>


// Booleans are nice
#define TRUE 1
#define FALSE 0

// Define a standard sample rate
#define STD_RATE 44100

// Define some types of waves
#define SINE 0
#define SQUARE 1
#define PI 3.14159265358979323846


/* Structure for an audio waveform.

   data:     array of audio samples
   envelope: array of amplitudes corresponding to data points
   length:   length of data and envelope arrays, in samples
   rate:     sampling rate, in Hz

*/
typedef struct WaveStruct {
  int8_t*    data;
  uint8_t*   envelope;
  uint32_t   length;
  uint32_t   rate;
} Wave;


/* Structure for a tone generator for a single waveform.

   shape:       integer corresponding to the tone's wave type.
                0 -> sine wave
                1 -> square wave
   frequency:   frequency of the tone, in Hz
   phase:       positive phase offset for the tone, in seconds
   phase_sweep: change in phase, in seconds per second
   envelope:    array of amplitudes for dynamic volume over time
   length:      length of envelope array, in samples
   rate:        sampling rate, in Hz

*/
typedef struct ToneStruct {
  int8_t     shape;
  double     frequency;
  double     phase;
  double     phase_sweep;
  uint8_t*   envelope;
  uint32_t   length;
  uint32_t   rate;
} Tone;


/* Structure for storing tones in a linked list to play simultaneously.

   value:     a pointer to a Tone struct
   cur_time:  the current playback time of the Tone, in seconds. Starts at 0.
   next:      a pointer to the next ToneNode in the list, or null.
   completed: a flag that is true once the tone has finished

   It's not recommended to have two ToneNodes point to the same Tone, because
   it could result in extra update calls and/or memory errors.
*/
typedef struct ToneNodeStruct {

  Tone*   value;
  double  cur_time;
  struct  ToneNodeStruct *next;
  uint8_t completed;

} ToneNode;


// Functions for manipulating Wave objects
Wave* make_wave(uint32_t length);
void free_wave(Wave* wave_ptr);


// Functions for manipulating Tone objects
Tone* make_tone(double shape, double frequency, double phase_sweep);
void free_tone(Tone* tone_ptr);
int8_t get_value(Tone* tone_ptr, double time);
void add_envelope(Tone* tone_ptr, uint8_t* envelope, uint32_t length);


// Functions for manipulating tone linked lists
ToneNode* make_tone_node(Tone* value);
void free_tone_node(ToneNode* node_ptr);
ToneNode* tone_node_append(ToneNode* head, ToneNode* appendage);
ToneNode* tone_node_delete(ToneNode* head, ToneNode* delete_me);
void tone_node_update(ToneNode* node_ptr, uint32_t dt);
ToneNode* delete_completed(ToneNode* head);


// Functions for generating envelope arrays
uint8_t* generate_envelope_asdr(double A, double S, double D, double R,
                                uint8_t sustain_amp);
