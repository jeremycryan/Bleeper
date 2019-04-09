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

## Learning Goals

For this project, I hope to get better at designing robust structures and frameworks that give better high-level control of the program. In the last project, while I had some opportunity to do this, it felt more results-focused, and some of the structure fell to the wayside.

Additionally, since I worked a lot with building off an external library (GTK) last project, I'll use this project as an opportunity to develop my own framework from the ground up.

## Getting Started

For a number of reasons, I'm getting started on this project a little later than the rest of the class, but I think I'm ready to hit the ground running right away. I have worked a little with audio synthesis in the past, and am looking to use this project more as an opportunity to develop my C programming skills than to explore something completely new conceptually.

I don't currently plan on using any outside libraries that I need to research; that being said, it's possible I'll end up using GLib if I decide that it would be worthwhile to start with some of their data structures.

## First Steps

### High-Level Structure

One of the more important aspects of this project will be how I choose to organize my code, what kind of objects I'll implement, and how I'll interface with the Arduino synthesizer. To get started, I'll create a bullet list, block diagram, or other representation of the framework. This document might evolve slightly over the course of the project.

### Hardware Demo

Another important piece to work on early is the hardware. I hope to get the first bleeps out of my synthesizer within a few days, so it's easier to troubleshoot and debug later on.
