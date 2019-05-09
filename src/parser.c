#include "main.h"

/* Makes a note based on a character array, or NULL of the character array
   begins with a null terminator.

   literal:   (char*) pointer to a character array that either begins with a
              null terminator or is length at least three. It should consist of
              a capital letter A-G, a flat (b) sharp (#) or natural (_) modifier
              and an octave number, in that order.
*/
Note* note_from_literal(char* literal) {

  char letter = literal[0];

  // Return null if the string begins with null terminator
  if (letter == '\0' || letter == '\n') return NULL;

  // Break out modifier and octave values
  char modifier = literal[1];
  if (modifier != SHARP && modifier != FLAT) modifier = NATURAL;
  uint8_t octave = literal[2] - '0';

  // Allocate memory and return new pointer
  Note* new_note = (Note*)malloc(sizeof(Note));
  new_note -> letter = letter;
  new_note -> modifier = modifier;
  new_note -> octave = octave;
  new_note -> duration = 1; //TODO make duration modifiable

  return new_note;
}


/* Frees a note struct
*/
void free_note(Note* note) {

  // Maybe this is one of those functions that really doesn't need a wrapper
  free(note);

}


/* Allocates a new NoteArray object with a given length.
*/
NoteArray* make_note_array(uint16_t length) {

  // Allocate memory for object and note array
  NoteArray* return_array = (NoteArray*)malloc(sizeof(NoteArray));
  Note** note_array = (Note**)malloc(sizeof(Note*) * length);
  return_array -> note_array = note_array;
  return_array -> length = length;
  return return_array;

}

/* Creates a new note array from a character array. */
NoteArray* note_array_from_literal(char* literal) {

  // Determine length of the expression
  int length;
  for (length = 1; ; length++) {
    if (literal[length] = '\0') break;
  }

  // Print error if expression has invalid length.
  if (length%3 != 0) {
    printf("Literal expression not divisible by 3.\n");
    printf("Expression found was length %d.", length);
  }

  // Allocate and add notes to a new array
  uint16_t note_length = (int)length/3;
  NoteArray* new_array = make_note_array(note_length);
  for (int i = 0; i < note_length; i++) {
    (new_array -> note_array)[i] = note_from_literal(literal);
    literal += (3 * sizeof(char));
  }

  return new_array;
}

/* Frees a note array.
*/
void free_note_array(NoteArray* n) {

  // Free all notes in the array
  for (int i = 0; i < n -> length; i++) {
    free_note((n -> note_array)[i]);
  }

  // Free the array
  free(n);

}


/* Returns a frequency in Hertz given a note pointer.
*/
double note_to_frequency(Note* note_ptr) {

  // Frequency of middle c, for reference
  double middle_c = 261.6;

  // Start with that frequency
  double freq = middle_c;

  // Change frequency based on note
  int8_t letter_offset = (note_ptr -> letter) - 'C';
  int8_t note_offset;
  switch (letter_offset) {
    case -2: note_offset = 9; break;
    case -1: note_offset = 11; break;
    case 0: note_offset = 0; break;
    case 1: note_offset = 2; break;
    case 2: note_offset = 4; break;
    case 3: note_offset = 5; break;
    case 4: note_offset = 7; break;
  } // Lookup table for each letter to a half-step offset

  // Change frequency based on modifier
  switch (note_ptr -> modifier) {
    case NATURAL: break;
    case SHARP: note_offset += 1; break;
    case FLAT: note_offset -= 1; break;
  }
  freq *= pow(2, note_offset/12.0);

  // Change frequency based on octave
  int8_t octave_offset = (note_ptr -> octave) - 4;
  freq *= pow(2, octave_offset);

  if (note_ptr -> letter == 'R') freq = 0;

  return freq;
}


/* Generates a tone object given a note.
*/
Tone* tone_from_note(Note* note, uint8_t wave_type) {

  Tone* return_tone;
  return_tone = make_tone(wave_type, note_to_frequency(note), 0);
  return return_tone;

}
