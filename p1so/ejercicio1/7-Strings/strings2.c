#include <stdio.h>
#include <string.h>

void copy2(char* org, char** dst)
{
	*dst = org;
}

void copy(char* org, char* dst)
{
	dst = org
}

void copy3(char* org, char* dst)
{
	dst = malloc(strlen(org) + 1);

	for(int i = 0; i < strlen(org)+1; i++)
	{
		dst[i] = org[i];
	}
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
	char* str2 = "other";
	char str3[32];

	copy(str1, str3);	//copy(str1, str2)
	//copy2(str1, &str2);
	printf("str1 %s str2 %s\n", str1, str3);	//printf("str1 %s str2 %s\n", str1, str2);

	mod(str1, str3);
	printf("str1 %s str3 %s\n", str1, str3);

	return 0;
}

/*
ssoo@ssoohost:~/p1so/ejercicio1/7-Strings$ ./strings2
str1 original str2 other
str1 original str3 ORIGINAL


	-La función copy no funciona porque la asignación dst = org; 
	solo cambia la dirección del puntero dst dentro de la función, pero no afecta al puntero que se pasó como argumento desde main(). 
	En C, los argumentos son pasados por valor, por lo que la modificación de dsten la funcióncopyno se refleja en main()

	-la funcion copy2() funciona ya que al pasar la direcion del destino y hacer que el puntero de origen apunte a la cadena destino. 
	ptr origen estara apuntando a la cadena origen.	
*/
