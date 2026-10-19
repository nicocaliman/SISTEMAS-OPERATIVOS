CC = gcc
CFLAGS = -Wall -g
#LIBS = -lm

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all : auxiliar.o init.o auxiliar.h
	gcc $(CFLAGS) -o prueba auxiliar.o init.o $(LIBS)

.PHONY: clean

clean:
	-rm *.o prueba

#variables definidas: CC, CFLAGS, LIBS
#objetivos: %.o, all, clean, .PHONY
#reglas: all, %.o, %.c, clean

#vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ make
#gcc -Wall -g -c -o auxiliar.o auxiliar.c
#gcc -Wall -g -c -o init.o init.c
#gcc -Wall -g -o prueba auxiliar.o init.o -lm
