CC = gcc
DEPS = main.h

# Make objects from C files and headers
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

# Compile final executable only after all object files
bleep: main.o wave.o tone.o envelope.o
	$(CC) -o bleep main.o wave.o tone.o envelope.o -lm

data: bleep
	./bleep > data
	aplay data -r 4000
