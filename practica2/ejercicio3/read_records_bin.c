#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#define LABEL_MAX_LEN 15

typedef struct 
{
    int id;
    double value;
    char label[LABEL_MAX_LEN+1];
}SimpleRecord;


int main(int argc, char* argv[]){

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <fichero_binario>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE* stream = NULL;

    //apertura del fichero
    stream = fopen(argv[1], "rb");

    //error de apertura
    if (stream == NULL)
    {
        err(EXIT_FAILURE, "fopen() failed!");
    }

    SimpleRecord sr;

    //mientras podamos leer una estructura
    while (fread(&sr, sizeof(SimpleRecord), 1, stream) == 1)
    {
        fprintf(stdout, "ID: %d, Valor: %.2f, Etiqueta: '%s'\n", sr.id, sr.value, sr.label);
    }    
    
    //cierre del fichero
    fclose(stream);

    return 0;
}

/*
vboxuser@SO:~/SO/practica2/ejercicio3$ ls -l mis_datos.txt mis_datos.bin
-rw-rw-r-- 1 vboxuser vboxuser 96 Oct  7 11:32 mis_datos.bin
-rw-rw-r-- 1 vboxuser vboxuser 48 Oct  6 12:39 mis_datos.txt
*/

/*
vboxuser@SO:~/SO/practica2/ejercicio3$ cat mis_datos.txt
Barcelona 3.10 1
Madrid 19.77 0
Valencia 7.42 2
*/

/*
vboxuser@SO:~/SO/practica2/ejercicio3$ cat mis_datos.bin
�����@Barcelona���Q��3@Madrid�G�z�@Valencia

No es legible el fichero binario
*/

/* PARA VER EL CONTENIDO REAL DEL FICHERO BINARIO
vboxuser@SO:~/SO/practica2/ejercicio3$ xxd mis_datos.bin 
00000000: 0100 0000 0000 0000 cdcc cccc cccc 0840  ...............@
00000010: 4261 7263 656c 6f6e 6100 8000 0000 0000  Barcelona.......
00000020: 0000 0000 0000 0000 85eb 51b8 1ec5 3340  ..........Q...3@
00000030: 4d61 6472 6964 0000 0000 0000 0000 0000  Madrid..........
00000040: 0200 0000 0000 0000 ae47 e17a 14ae 1d40  .........G.z...@
00000050: 5661 6c65 6e63 6961 0000 0000 0000 0000  Valencia........
*/

/* PARA VER EL CONTENIDO REAL DEL FICHERO BINARIO
vboxuser@SO:~/SO/practica2/ejercicio3$ hexdump -C mis_datos.bin
00000000  01 00 00 00 00 00 00 00  cd cc cc cc cc cc 08 40  |...............@|
00000010  42 61 72 63 65 6c 6f 6e  61 00 80 00 00 00 00 00  |Barcelona.......|
00000020  00 00 00 00 00 00 00 00  85 eb 51 b8 1e c5 33 40  |..........Q...3@|
00000030  4d 61 64 72 69 64 00 00  00 00 00 00 00 00 00 00  |Madrid..........|
00000040  02 00 00 00 00 00 00 00  ae 47 e1 7a 14 ae 1d 40  |.........G.z...@|
00000050  56 61 6c 65 6e 63 69 61  00 00 00 00 00 00 00 00  |Valencia........|
00000060
*/

/*
mis_datos.bin (96 bytes) es más grande que mis_datos.txt (48 bytes).
Esto es debido a que en binario, la estructura se guarda con un tamaño fijo por registro.
sizeof(SimpleRecord) ocupa 32 bytes en memoria (4 bytes para id, 4 bytes de padding de alineamiento, 
8 bytes para double value y 16 bytes para char label[16]). Con 3 registros: 3 * 32 = 96 bytes.

En texto, los datos se almacenan codificados en caracteres ASCII con longitud variable.
El id "1" ocupa solo 1 byte, el valor "3.10" ocupa 4 bytes y los nombres solo ocupan sus letras más los separadores
(espacios y saltos de línea).
*/

/*
cat interpreta cada byte del fichero como un carácter del mapa de codificación de la terminal (ASCII/UTF-8).

En el fichero binario, los tipos numéricos (int y double) y los bytes de relleno no están almacenados como texto, sino en su representación complemento a dos  punto flotante.
Al encontrarse con bytes nulos (0x00) o secuencias no imprimibles, la terminal muestra símbolos extraños o no imprime nada. 
Solo se leen con claridad los fragmentos de texto ASCII correspondientes a las etiquetas "Barcelona", "Madrid" y "Valencia"
*/

/*
En la salida de hexdump/xxd se identifican con exactitud los 3 registros alineados en bloques de 32 bytes (0x20 en hexadecimal):

    Registro 1 (0x0000 a 0x001F):

        01 00 00 00: id = 1 (4 bytes, Little Endian).

        00 00 00 00: Padding (4 bytes añadidos por el compilador para alinear el double a una dirección múltiplo de 8).

        cd cc cc cc cc cc 08 40: value = 3.10 en IEEE 754 de 64 bits (8 bytes).

        42 61 72 63 65 6c 6f 6e 61 00...: label = "Barcelona\0" seguido de los bytes de relleno hasta completar los 16 bytes del array.

    Registro 2 (0x0020 a 0x003F): Datos de Madrid.

    Registro 3 (0x0040 a 0x005F): Datos de Valencia.
*/

/*
Formato de Texto:

    Ventaja: Legible y editable con cualquier editor de texto básico. Portable entre distintas arquitecturas sin importar el endianness o alineamiento de memoria.

    Desventaja: Lectura/escritura más lenta al requerir parseo (fscanf/sscanf) y posible pérdida de precisión al convertir valores flotantes a texto.

Formato Binario:

    Ventaja: Máxima velocidad de E/S al leer/escribir bloques completos directamente a memoria y preservación exacta de la precisión numérica.

    Desventaja: No es legible por humanos y depende de la arquitectura
*/

/*
A. Escritura en fichero binario

    ¿Se puede escribir el array con un único fwrite()? NO.

    ¿Por qué? Si ejecutas fwrite(array, sizeof(SimpleRecord), N, stream), guardarás en el disco las direcciones de memoria (punteros de 64 bits) del heap donde residen las cadenas. Una dirección de memoria no tiene ningún valor fuera de la ejecución actual del programa.

    Mecanismo correcto de escritura (Serialización):
    Debes recorrer el array elemento a elemento y guardar por separado los campos fijos y la cadena apuntada:

        Escribir los campos fijos: fwrite(&sr.id, sizeof(int), 1, stream) y fwrite(&sr.value, sizeof(double), 1, stream).

        Escribir la cadena: Determinar la longitud de la cadena len = strlen(sr.label) + 1 (incluyendo '\0') y escribirla directamente desde el heap:

B. Lectura desde fichero binario

    ¿Se puede leer el array con un único fread()? NO.

    ¿Por qué? Si lees la estructura de golpe con fread, el campo label tomará bytes brutos del archivo como si fuesen una dirección de memoria que causará un Segmentation Fault al intentar usarlo.

    Mecanismo correcto de lectura (Deserialización):
    Debes leer elemento por elemento en un bucle:

        Leer los campos numéricos fijos id y value con fread.

        Leer la cadena alojando memoria dinámicamente (por ejemplo, leyendo carácter a carácter hasta '\0' o leyendo primero un entero con la longitud).

        Reservar memoria en el heap con malloc(longitud + 1).

        Leer los caracteres del archivo dentro del búfer de malloc y asignar ese puntero devuelto a sr.label.
*/