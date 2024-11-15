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
#targets: %.o, all, clean
#reglas -> %.o: %.c, all: auxiliar.o init.o auziliar.h, clean:

#Al aplicar touch aux.c se elimina una linea al hacer make de nuevo, es decir,
#la primera vez que se ejecuta, con make:
#ssoo@ssoohost:~/p1so/ejercicio1/2-Make$ make
#gcc -Wall -g -c -o auxiliar.o auxiliar.c
#gcc -Wall -g -c -o init.o init.c
#gcc -Wall -g -o prueba auxiliar.o init.o -lm
#al ejecutarlo por segunda vez, con make:
#ssoo@ssoohost:~/p1so/ejercicio1/2-Make$ make
#gcc -Wall -g -c -o auxiliar.o auxiliar.c
#gcc -Wall -g -o prueba auxiliar.o init.o -lm
#Cuando ejecutas make clean se ejecuta la regla clean del archivo makefile
#eliminando cualquier objeto .o creado y el fichero prueba
#si comentamos la variable definida LIBS, nos da problemas a la ohra de ejecutar la regla all:...
#pues esta regla hace uso de la variable LIBS que, al estar comentada, no existe a la hora de construir los ficheros