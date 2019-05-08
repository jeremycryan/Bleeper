#include "main.h"

void setup() {

  initialize_pins();
  Serial.begin(9600);
  Serial.println();
  Serial.println("SEGFAULT?");
  Serial.print("Starting up.\n");
  
}

void loop() {

  Serial.println("Start of loop");

  uint8_t tones_created = 1;
  uint32_t last_freq = 300;
  delay(50);

  Tone* tone = make_tone(SQUARE, last_freq, 0);
  Serial.println(tone -> frequency);
  delay(50);
  Envelope* envelope = make_envelope(0.1, 0.2, 0.5, 0.1, 50);
  delay(50);
  add_envelope(tone, envelope);
  delay(50);

  ToneNode* tone_head = make_tone_node(tone);
  ToneNode* new_node;
  delay(50);

  double t = 0;   //  Current simulation time, in microseconds
  double then = micros();
  int samples_in = 0;

  while (tones_created < 5) {

    samples_in++;
    double now = micros();
    double dt = now - then;
    then = now;
    t += dt/1000000;

    // Update the list of tones, and delete finished tones
    tone_node_update(tone_head, dt);
    tone_head = delete_completed(tone_head);

    // Add a tone, if the time is right.
    if (t > tones_created*2) {
      last_freq *= 1.5;
      tone = make_tone(SINE, last_freq, 0);
      envelope = make_envelope(0.05, 0.1, 0.5, 0.1, 50);
      add_envelope(tone, envelope);
      new_node = make_tone_node(tone);
      tone_head = tone_node_append(tone_head, new_node);
      tones_created++;
    }

    // Set speakers to current value
    uint8_t pins_val = get_value(tone_head -> value, tone_head -> cur_time) + 128;
    set_speaker(pins_val);

  }
  
  Serial.print("Samples in 5 seconds: ");
  Serial.flush();
  Serial.println(samples_in);
  Serial.flush();

}
