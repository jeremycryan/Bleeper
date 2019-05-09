# Bleeper
### Jeremy Ryan

A simple synthesizer made with C, as part of Software Systems at Olin College.

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

At the time of writing this final report, I believe that I have achieved my learning goals pretty well. My project uses reasonably complex structures, with several helper functions, and I've had to think deliberately about how to organize my code and files.

## Structures and Implementation

As when I started the project, I don't plan on using any outside libraries that I need to research. Part of my learning goals are developing the data structures I'll be using myself, from scratch.

### Code Structure

For this project, I opted to use a single header file with several C files. Each C file, for the most part, was fairly straightforward and had a reasonable number of function definitions, so I decided it would just make the process more tedious to have five separate header files.

[Code hierarchy image]

### Data Structures

Here are some of the data structures I implemented for this project:

- The **Wave** object holds arbitrary audio data, and an envelope. It could be used for loading audio data from a file or from another structure.

- The **Tone** object doesn't hold any audio data; it holds an envelope and waveform information such that it can generate a signal in real time.

- The **WaveNode** and **ToneNode** objects act as containers for their respective data types to put in linked lists. There are functions to update each node of a list, in the case of ToneNodes.

- The **Note** object acts as an intermediary between raw letter inputs (e.g. "Bb4") and the lower-level ```Tone``` object. It holds information about the note letter, octave, and accidentals. Additionally, they can be stored in a **NoteArray** object.

- While not a structure, I have a number of waveforms I've implemented behavior for, including a sine wave, square wave, triangle wave, sawtooth wave, and noise channel.

Between these structures, I am able to generate tones at arbitrary frequency and 8-bit amplitude resolution, apply basic ADSR enveloping, and write the data to files. Furthermore, I can generate these structures from a text input, using higher-level abstractions like note names and octaves. At present, two tracks can be read and played simultaneously to produce chords and harmonies. This fulfills all of my MVP requirements except integration with Arduino, and two of my three stretch goals.

## What About the Arduino?

One notably missing aspect of my Arduino synthesizer is the hardware. I did actually set up the hardware and get some sounds playing (you can see the ```arduino_synth``` branch on this repository to look over the code). However, there were some physical restrictions of the platform, coupled with my own learning goals, that eventually made me go back to a desktop script.

[arduino setup image]

### Sample Rate

First and foremost, the ATmega328P that serves as the processor for the Arduino Uno is not that fast. The clock, driven by an external crystal, runs at 8MHz, compared to the 2.3GHz clock on the laptop I'm using to write this report.

While an 8MHz clock isn't by itself prohibitive of creating a software synthesizer, the actual sample rate I was able to produce waveforms with was obviously much slower. Running at maximum speed, the highest sample rate I was able to get with my synthesizer code was somewhere between 2.0kHz and 2.5kHz. Even when I cut away the majority of my code body such that I was effectively just running ```sin``` and ```DigitalWrite``` each loop, I was still only able to increase this rate to 5kHz or so.

### Why Does This Matter?

It turns out that humans are very good at hearing, and undersampling results in a number of unfriendly artifacts in the audio. Human hearing goes up to around 20kHz, so because of Nyquist's Theorem, most digital audio recorded today has a sampling rate of either 44.1kHz or 48.0kHz. If your digital synthesizer only samples at 2kHz, the majority of the hearable spectrum is instead occupied by the high-frequency transitions between quantizations. All in all, this makes the Arduino make vaguely musical sounds buried beneath a cacophony of ungodly screeching.

This probably isn't helped by the fact that I chose to use 8 bits of depth for the audio waveforms (originally to conserve memory on the Arduino), compared to the 16 or 32 used in most digital audio today.

So, in order to generate reasonable audio, I had a number of options:

- **Optimize the Code**: I'm skeptical that this would result anywhere close to a 44.1kHz sampling rate, based on some of my experiments. Additionally, it would probably mean tossing some of my data structures and functions in favor of a great many for loops, which is contrary to my learning goals.

- **Use Arduino's Tone Function**: Arduino does have a function called ```tone```, which is the basis for a number of Arduino synthesizer projects I saw online. I would assume it uses separate hardware similar to the PWM pins, and thus wouldn't be affected by my fairly main loop. However, it's also only able to produce a square wave, so I would either have to drop some of the more complex sounds, or find a way to build them up from square waves. Ultimately, I decided that my learning goals more closely aligned to the third option.

- **Drop the Arduino**: I eventually opted just to drop the Arduino and instead have the synthesizer write to audio files on the computer. This allowed me to dedicate more time to solving interesting software problems, rather than spending hours trying to optimize firmware for an AVR chip.


## Try It Out

To hear some of the example outputs from my synthesizer, you can perform the following steps:

- Download the ```outputs``` directory, or any specific file inside.

Then play the file in one of the following ways:

**Aplay**: Play the track with ```aplay```, with a sample rate of 44.1kHz. For instance, ```aplay filename -r 44100```.

**Audacity**: Start an audio editing program like Audacity, then import the file as raw data with the following settings:

- Unsigned 8-bit PCM
- Mono (single channel)
- Sample rate 44100 Hz

**Play the WAV files**: Alternatively, you can just open one of the WAV files in the output directory that I generated after typing most of this section.


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
