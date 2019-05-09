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
#define TRIANGLE 2
#define SAWTOOTH 3
#define NOISE 4

// Define mathematical constants and music things
#define PI 3.14159265358979
#define FLAT 'b'
#define SHARP '#'
#define NATURAL '_'

/* Structure for generating ADSR envelopes in real time.
   A:   (double) attack time, in seconds
   D:   (double) decay time, in seconds
   S:   (double) sustain time, in seconds
   R:   (double) release time, in seconds
   sustain_amp: (uint8_t) amplitude of envelope during sustain phase
*/
typedef struct EnvelopeStruct{

  double A;
  double S;
  double D;
  double R;
  uint8_t sustain_amp;

} Envelope;

/* Structure for an audio waveform.
   data:     array of audio samples
   envelope: array of amplitudes corresponding to data points
   length:   length of data and envelope arrays, in samples
   rate:     sampling rate, in Hz
*/
typedef struct WaveStruct {
  int8_t*    data;
  Envelope*   envelope;
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
  Envelope*  envelope;
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

/* Structure for a single note, specifying pitch and duration in beats.

   letter:    (char) capital note letter (A, B, C... G)
   modifier:  (char) # for sharp, b for flat, _ for natural.
   octave:    (uint8_t) octave number for the note. C4 is middle C.
   duration:  (double) duration of the note, in quarters.
              0.5 is an eighth note, 2 is a half note, etc.

*/
typedef struct NoteStruct {
  char letter;
  char modifier;
  uint8_t octave;
  double duration;
} Note;

/* Allocates memory for a note and returns a pointer.

   literal:   (char*) pointer to the first character of an array containing
              the literal expression for the note (e.g. 'Ab4' or 'F#6')
*/


/* Structure for an array of notes, containing a length and a pointer to the
   first element.

   length:      (uint16_t) number of elements in the array
   note_array:  (Note**) pointer to the first element

*/
typedef struct NoteArrayStruct {
  uint16_t length;
  Note** note_array;
} NoteArray;


// Functions for manipulating Wave objects
Wave* make_wave(uint32_t length);
void free_wave(Wave* wave_ptr);


// Functions for manipulating Tone objects
Tone* make_tone(double shape, double frequency, double phase_sweep);
void free_tone(Tone* tone_ptr);
int8_t get_value(Tone* tone_ptr, double time);
void add_envelope(Tone* tone_ptr, Envelope* envelope);
void print_tone(Tone* tone_ptr);


// Functions for manipulating tone linked lists
ToneNode* make_tone_node(Tone* value);
void free_tone_node(ToneNode* node_ptr);
ToneNode* tone_node_append(ToneNode* head, ToneNode* appendage);
ToneNode* tone_node_delete(ToneNode* head, ToneNode* delete_me);
void tone_node_update(ToneNode* node_ptr, uint32_t dt);
ToneNode* delete_completed(ToneNode* head);
uint8_t get_list_value(ToneNode* head);


// Functions for generating envelope arrays
Envelope* make_envelope(double A, double D, double S, double R, uint8_t sustain_amp);
uint8_t get_env_val(Envelope* e, double t);
void free_envelope(Envelope*);
Envelope* envelope_copy(Envelope* e);
double envelope_duration(Envelope* e);


// Functions for interfacing with Notes and NoteArrays
double note_to_frequency(Note* note_ptr);
Note* note_from_literal(char* literal);
Tone* tone_from_note(Note* note, uint8_t wave_type);
NoteArray* note_array_from_literal(char* literal);
