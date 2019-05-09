#include "main.h"

/*  Allocate memory for a tone structure and return a pointer to it.
    Length starts at 0 and envelope starts out unallocated, so an envelope must
    be added after this instantiation.
*/
Tone* make_tone(double shape, double frequency, double phase_sweep) {

  // Allocate memory for tone object
  Tone* tone_ptr = (Tone*)malloc(sizeof(Tone));

  // Assign values to attributes
  tone_ptr -> shape = shape;
  tone_ptr -> frequency = frequency;
  tone_ptr -> phase = 0;
  tone_ptr -> phase_sweep = phase_sweep;
  tone_ptr -> envelope = make_envelope(0, 0, 1, 0, 0);
  tone_ptr -> length = 0;
  tone_ptr -> rate = STD_RATE;

  // Return new object
  return tone_ptr;

}

// Prints a representation of a tone object to stdout.
void print_tone(Tone* tone_ptr) {

  printf("Tone object: ");

  // Print shape
  switch (tone_ptr -> shape) {
    case SINE: printf("SINE "); break;
    case SQUARE: printf("SQUARE "); break;
  }

  // Print frequency
  printf("%fHz\n", tone_ptr -> frequency);

}

/* Frees a tone structure. The tone's envelope must be freed prior to this call,
   as multiple tones can share a single envelope pointer.
*/
void free_tone(Tone* tone_ptr) {

  // Free envelope, if it exists
  if (tone_ptr -> envelope != NULL) free_envelope(tone_ptr -> envelope);

  // Free tone
  free(tone_ptr);

}

/* Returns a value [-128, 127] representing a single sample of the tone
   at a certain time.
   tone_ptr:  a pointer to a Tone struct
   time:      time to sample, in seconds
*/
int8_t get_value(Tone* tone_ptr, double sample_time) {

  // Determine the number of the sample to fetch
  uint32_t rate = tone_ptr -> rate;
  uint32_t sample = (int32_t)(tone_ptr -> rate * sample_time);

  // Calculate the value at that sample
  for (int i = 0; i++; i < 10000000){}
  uint8_t env_amp = get_env_val(tone_ptr -> envelope, sample_time);
  int8_t signal_amp;
  uint32_t frequency = tone_ptr -> frequency;
  double x_val = frequency * (sample_time - tone_ptr -> phase);
  double through;

  switch (tone_ptr -> shape) {

    case SINE:
      signal_amp = sin(x_val * 2 * PI) * 127;
      break;

    case SQUARE:
      signal_amp = (fmod(x_val, 1.0) > 0.5) ? 127 : -128;
      break;

    case TRIANGLE:
      through = fmod(x_val + 0.25, 1.0);
      signal_amp = (through <= 0.5) ? through * 510 - 127
        : (1 - through) * 510 - 127;
      break;

    case SAWTOOTH:
      through = fmod(x_val + 0.5, 1.0);
      signal_amp = through * 254 - 127;
      printf("%d\n", signal_amp);
      break;

    case NOISE:
      signal_amp = rand()%254 - 127;
      if (frequency == 0) signal_amp = 128;
      break;

  }

  return env_amp * signal_amp / 256;

}


/* Gets the average of the first four ToneNodes in a list, and returns the value
*/
uint8_t get_list_value(ToneNode* head) {

  uint8_t each[] = {128, 128, 128, 128};

  // Get the value of each item in the list, up to the fourth
  for (int i = 0; i < 4; i++) {
    if (head == NULL) break;
    each[i] = get_value(head -> value, head -> cur_time) + 128;
    head = head -> next;
  }

  // Return their average
  return each[0]/4 + each[1]/4 + each[2]/4 + each[3]/4;

}

/* Adds an envelope of a certain length to the Tone structure.
*/
void add_envelope(Tone* tone_ptr, Envelope* e) {

  // Determine length of envelope
  uint32_t length = (uint32_t)((e -> A + e -> S + e -> D + e -> R) * STD_RATE);

  // Replace envelope attribute of tone and update length
  tone_ptr -> envelope = e;
  tone_ptr -> length = length;

}


/* Instantiates a ToneNode and returns a pointer.
*/
ToneNode* make_tone_node(Tone* value) {

  // Allocate memory for the structure
  ToneNode* node_ptr = (ToneNode*)malloc(sizeof(ToneNode));
  node_ptr -> cur_time = 0;
  node_ptr -> next = NULL;
  node_ptr -> completed = FALSE;
  node_ptr -> value = value;

  return node_ptr;

}

/* Frees a ToneNode
*/
void free_tone_node(ToneNode* node_ptr) {

  free_tone(node_ptr -> value);
  free(node_ptr);

}


/* Adds the 'appendage' node to the beginning of the linked list that starts
   with node head, then returns the new head.
*/
ToneNode* tone_node_append(ToneNode* head, ToneNode* appendage) {

  appendage -> next = head;
  return appendage;

}

/* Deletes the given node from the linked list that starts with node head,
   if it exists. Returns the new head of the list.
*/
ToneNode* tone_node_delete(ToneNode* head, ToneNode* delete_me) {

  // Start iterating from the head of the list
  ToneNode* cur_node;
  ToneNode* last_node;
  cur_node = head;
  last_node = NULL;

  // If the node to delete is the head, return the next value.
  if (head == delete_me) return delete_me -> next;

  // Otherwise, search for the delete node.
  while (1) {

    // If you've reached the end of the list, the node was not found, so
    // return the old head
    if (cur_node == NULL) return head;

    // If you find the deleteworthy node, jump over it from the previous node.
    if (cur_node == delete_me) {
      last_node -> next = cur_node -> next;
      return head;
    }

    // If neither happened, continue to iterate through the list.
    last_node = cur_node;
    cur_node = cur_node -> next;

  }

}

/* Updates the given node based on how much time has elapsed.
   node_ptr:  pointer to a ToneNode struct
   dt:        elapsed time, in millionths of a second
*/
void tone_node_update(ToneNode* node_ptr, uint32_t dt) {

  // If given null pointer, return
  if (node_ptr == NULL) {
    return;
  }

  // Update the object
  Tone* tone = node_ptr -> value;
  node_ptr -> cur_time += dt / 1000000.0;
  tone -> phase += tone -> phase_sweep / 1000000.0;

  // If the time has exceeded the length of the tone, flag that
  // node so it can be deleted.
  if (node_ptr -> cur_time > 1.0 * tone -> length / tone -> rate) {
    node_ptr -> completed = TRUE;
  }

  // Update the next object in the list.
  tone_node_update(node_ptr -> next, dt);

}

/* Iterates through list and deletes all nodes that have completed. Takes in the
   current head of the list and returns the new head.
*/
ToneNode* delete_completed(ToneNode* head) {

  // If list is empty, return null
  if (head == NULL) return NULL;

  // If head isn't ready to delete, continue along the list
  if (head -> completed == FALSE) {
    head -> next = delete_completed(head -> next);
    return head;
  }

  // Otherwise, delete it.
  // TODO Fix memory leak if it becomes a problem
  else {
      ToneNode* next = head -> next;
      return delete_completed(next);
  }

}
