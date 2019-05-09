#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "main.h"

// Test script for generating sound files.

int main() {

  uint8_t tones_created = 1;
  uint32_t last_freq = 440;

  // Set some notes to play
  FILE* ch1_file = fopen("../inputs/gr_0.txt", "rb");
  FILE* ch2_file = fopen("../inputs/gr_1.txt", "rb");
  char* channel_1_stream = malloc(sizeof(char) * 1000);
  char* channel_2_stream = malloc(sizeof(char) * 1000);
  fgets(channel_1_stream, 1000, ch1_file);
  fgets(channel_2_stream, 1000, ch2_file);
  fclose(ch1_file);
  fclose(ch2_file);

  // Determine length of note stream
  uint16_t stream_length = 0;
  for (stream_length = 0; ; stream_length++) {
    if (channel_1_stream[stream_length] == '\0') break;
    if (channel_1_stream[stream_length] == '\n') break;
  }
  stream_length /= 3;

  printf("Stream length: %d\n", stream_length);

  // Create some object instances
  Tone* new_tone = make_tone(SINE, 440, 0);
  Note* new_note;
  Envelope* default_envelope = make_envelope(0.05, 0.15, 0.1, 0.1, 70);
  ToneNode* tone_head = make_tone_node(new_tone);
  ToneNode* new_node;

  double t = 0;   //  Current simulation time, in microseconds
  double quarter_time = 0.25;

  // Open a file to dump audio data
  FILE* f = fopen("data", "wb");

  while (t < quarter_time * stream_length + envelope_duration(default_envelope)) {

    // Update the list of tones, and delete finished tones
    tone_node_update(tone_head, 1000000/STD_RATE);
    tone_head = delete_completed(tone_head);

    // Write the current value of the tone list to a file
    fputc(get_list_value(tone_head), f);

    // Add a tone, if the time is right.
    if (t > tones_created*quarter_time) {

      // Add a note to channel 1 if the stream isn't empty
      new_note = note_from_literal(channel_1_stream);
      if (new_note != NULL) {
        channel_1_stream += 3*sizeof(char);

        new_tone = tone_from_note(new_note, SINE);
        add_envelope(new_tone, envelope_copy(default_envelope));
        new_node = make_tone_node(new_tone);
        tone_head = tone_node_append(tone_head, new_node);

      }

      // Add a note to channel 2 if the stream isn't empty
      new_note = note_from_literal(channel_2_stream);
      if (new_note != NULL) {
        channel_2_stream += 3*sizeof(char);

        new_tone = tone_from_note(new_note, SINE);
        add_envelope(new_tone, envelope_copy(default_envelope));
        new_node = make_tone_node(new_tone);
        //tone_head = tone_node_append(tone_head, new_node);

      }

      tones_created++;

    }

    // Increment time by one sample's worth
    t += 1.0/STD_RATE;
  }

  fclose(f);
}
