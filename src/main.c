#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "main.h"

int main() {

  uint8_t tones_created = 1;
  uint32_t last_freq = 440;

  Tone* tone = make_tone(SINE, 440, 0);
  uint8_t* envelope = generate_envelope_asdr(0.05, 0.1, 0.5, 0.1, 50);
  add_envelope(tone, envelope, (int)(STD_RATE * 0.75));

  ToneNode* tone_head = make_tone_node(tone);
  ToneNode* new_node;

  double t = 0;   //  Current simulation time, in microseconds

  FILE* f = fopen("data", "wb");

  while (t < 5) {

    // Update the list of tones, and delete finished tones
    tone_node_update(tone_head, 1000000/STD_RATE);
    tone_head = delete_completed(tone_head);

    // Print the value to pipe into a file
    if (tone_head != NULL) {
      fputc(get_value(tone_head -> value, tone_head -> cur_time) + 128, f);
    } else {
      fputc(128, f);
    }

    // Add a tone, if the time is right.
    if (t > tones_created) {
      last_freq *= 1.25;
      tone = make_tone(SINE, last_freq, 0);
      envelope = generate_envelope_asdr(0.05, 0.1, 0.5, 0.1, 50);
      add_envelope(tone, envelope, (int)(STD_RATE*0.75));
      new_node = make_tone_node(tone);
      tone_head = tone_node_append(tone_head, new_node);
      tones_created++;
    }

    // Increment time by one sample worth
    t += 1.0/STD_RATE;
  }

  fclose(f);
}
