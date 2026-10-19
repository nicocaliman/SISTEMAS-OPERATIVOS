#include <stdio.h>
#include <string.h>

//Un puntero a puntero (char**): Es una variable que guarda la dirección de memoria de otro puntero
void copy2(char* org, char** dst)
{
	*dst = org;
}

void copy(char* org, char* dst)
{
	dst = org;
}

void copy_correcta(const char* org, char* dst)
{
    int i = 0;
    while (org[i] != '\0') {
        dst[i] = org[i];
        i++;
    }
    dst[i] = '\0'; /* Aseguramos el terminador nulo */
}

void mod(char* org, char* dst)
{
	int i;

	for (i=0;i<strlen(org);i++)
		dst[i] = org[i] - 32;
}

int main()
{
	char* str1 = "original";
	//char* str2 = "other";
	char str2[32] = "other";
	char str3[32];

	//copy(str1, str2);
	//copy2(str1, &str2);
	copy_correcta(str1, str2);
	printf("str1 %s str2 %s\n", str1, str2);

	mod(str1, str3);
	printf("str1 %s str3 %s\n", str1, str3);

	return 0;
}

/*
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/7-Strings$ ./strings2
str1 original str2 other
str1 original str3 ORIGINAL

En C, los parámetros se pasan siempre por valor. La función copy(char* org, char* dst) recibe una copia de la dirección guardada en str2.

Al ejecutar dst = org, únicamente estás modificando la variable local dst dentro del ámbito de la función. El puntero str2 de la función main() sigue apuntando a la dirección original ("other").

Al pasar &str2, la función copy2(char* org, char** dst) recibe un puntero a puntero (la dirección de memoria donde reside str2). Al hacer *dst = org;, se desreferencia el puntero y se modifica directamente el valor de str2 en main(), logrando que apunte a la misma dirección de memoria que str1 ("original").

Convierte la cadena str1 a letras mayúsculas y guarda el resultado en str3.

En el código ASCII, las letras mayúsculas se encuentran 32 posiciones antes que las minúsculas (por ejemplo, 'a' es 97 y 'A' es 65, donde 97 - 32 = 65).

    Memoria modificable: A diferencia de str2, str3 está declarada como un array en el stack (char str3[32];), por lo que su memoria es de lectura y escritura. Esto permite modificar sus posiciones dst[i] sin causar un fallo de segmentación (segmentation fault).

*/