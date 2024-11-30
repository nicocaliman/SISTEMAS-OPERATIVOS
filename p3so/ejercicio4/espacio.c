#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	struct stat fileStat;

	if(lstat(fname, &fileStat) == -1)
	{
		perror("lstat get_size()");
		exit(EXIT_FAILURE);
	}

	if (S_ISDIR(fileStat.st_mode))
	{
		get_size_dir(fname, blocks);
	}

	else if(S_ISREG(fileStat.st_mode))
	{
		*blocks += fileStat.st_blocks;
	}

	else
	{
		fprintf(stderr, "No es ni un directorio ni un fichero");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;	
}


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
	DIR *dir;

	if((dir = opendir(dname)) == NULL)
	{
		fprintf(stderr, "error al abrir el directorio");
		exit(EXIT_FAILURE);
	}

	struct dirent *entry = readdir(dir);

	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			char path[1024];
			sprintf(path, "%s/%s", dname, entry->d_name);	//imprimir en un string

			get_size(path, blocks);	//llamada recusriva
		}		
	}

	closedir(dir);	//cierre del directorio	
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <list_files>", argv[0]);
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++)
	{
		size_t nBlocks = 0;
		get_size(argv[i], &nBlocks);

		printf("%ldK %s\n", nBlocks/2, argv[i]);
	}	

	return 0;
}

/*
	si cada bloque es de 512 B, para convertirlo bloques a KB: 1KB = 1024B

	TAM_BLOQUE = (nBloque *512 B) * 1 KB / 1024 B = nBloques / 2
 */