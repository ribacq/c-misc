#c-misc Makefile

CC = gcc
CFLAGS = -Wall

all: pepe hilbertCurve jgol m8b

pepe:
	${CC} ${CFLAGS} -lSDL2 -lSDL2_image -o pepe.out pepe.c

hilbertCurve:
	${CC} ${CFLAGS} -lSDL2 -lm -o hilbertCurve.out hilbertCurve.c

jgol:
	${CC} ${CFLAGS} -lncurses -o jgol.out jgol.c

m8b:
	${CC} ${CFLAGS} -o m8b.out m8b.c

.PHONY: clean
clean:
	-rm *.out

