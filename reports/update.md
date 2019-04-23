# Bleeper
### Jeremy Ryan

A simple synthesizer made with C and Arduino, as part of Software Systems at Olin College.

## Project Goal

For this project, I plan to create a simple audio synthesizer in C using an Arduino. At minimum, it should be able to do the following:

- Simulate analog waveforms with an R2R ladder with at least 8 bits of resolution

- Produce sine waves and square waves of arbitrary frequency, within a range

- Use data structures that enable high-level control over the output (e.g. typing note letters)

As stretch goals, I could build on this lower bound in one or more of the following ways:

- Simulate more waveforms, like sawtooth or triangle waves, or combine sine waves to approximate the frequency profile of flute, trumpet, etc.

- Generate and combine several waves at once to produce chords

- Simulate common chiptune flourishes, like slides, envelopes, or "blipper" for single-channel chords/arpeggios

- Use buttons in hardware as inputs to generate notes real time

## Learning Goals

For this project, I hope to get better at designing robust structures and frameworks that give better high-level control of the program. In the last project, while I had some opportunity to do this, it felt more results-focused, and some of the structure fell to the wayside.

Additionally, since I worked a lot with building off an external library (GTK) last project, I'll use this project as an opportunity to develop my own framework from the ground up.

As of this mid-project update, I feel that I've made good progress on my learning goals. My project uses reasonably complex structures, with several helper functions, and I've had to think deliberately about how to organize my code and files.

## Getting Started

As when I started the project, I don't plan on using any outside libraries that I need to research. Part of my learning goals are developing the data structures I'll be using myself, from scratch.

So far, I have created some structures for use in synthesis:

- The **Wave** object holds arbitrary audio data, and an envelope. It could be used for loading audio data from a file or from another structure.

- The **Tone** object doesn't hold any audio data; it holds an envelope and waveform information such that it can generate a signal in real time.

- The **WaveNode** and **ToneNode** objects act as containers for their respective data types to put in linked lists. There are functions to update each node of a list, in the case of ToneNodes.

Additionally, I've structured the code into multiple files, made a makefile and gitignore, and created a test bench that lets me store data for testing.

## Next Steps

### Hardware Demo

One major part of the project I've overlooked until now is getting the hardware up and running. I don't anticipate many pitfalls, but it will still probably take some time to assemble and test the Arduino and DAC. It's probably in my best interests to get some actual bleeps as soon as possible.

### Envelope Object

I've been passing around integer arrays to serve as envelopes for my various structures, but it has occurred to me that a more elegant solution would be to make a separate structure for envelopes. Instead of passing an array with a separate length argument, I could include relevant information in the structure and expand it later if necessary.

This change should be fairly easy to implement. I'll also be on the lookout for other changes that make some of the interfacing between structures more elegant.

### Abstraction

One of the goals for this project was to interface with the synthesizer at a higher level --- for instance, by typing note names. Getting a neat wrapping around some of my lower-level structures to make this nice interface is a priority, especilally if I want to create reasonably complicated music on the final hardware.

## Links

In case you're viewing this from somewhere else, here is a link to [my Github repo](https://github.com/jeremycryan/Bleeper).

You can see my project's process on [my Trello board](https://trello.com/b/njsXTbJd/softsys-bleeper-jeremy). I've mostly been managing this project offline, so I may be slow to update it.
