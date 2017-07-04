#sdltest Makefile

CC = gcc
CFLAGS = -Wall

all: pepe hilbertCurve

pepe:
	${CC} ${CFLAGS} -lSDL2 -lSDL2_image -o pepe.out pepe.c

hilbertCurve:
	${CC} ${CFLAGS} -lSDL2 -lm -o hilbertCurve.out hilbertCurve.c

.PHONY: clean
clean:
	-rm ${OUT} ${OBJ}

