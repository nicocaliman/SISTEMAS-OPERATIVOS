#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[])
{
	FILE* file=NULL;

	if (argc < 2)
	{
		fprintf(stderr,"Usage: %s <file_name><arg1><arg2>...\n",argv[0]);
		exit(1);
	}

	if((file = fopen(argv[1], "w")) == NULL)	//apertura del archivo en modo escritura
	{
		//si null		
		err(2,"The input file %s could not be opened",argv[1]);
		return EXIT_FAILURE;
	}

	for (int i = 2; i < argc; i++) 							//desde el primer string hasta el utlimo
	 {
        fwrite( argv[i], strlen(argv[i])+1, 1,file);
    }

	fclose(file);	//cierre del fichero
	
	return 0;
}