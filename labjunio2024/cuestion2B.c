#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>

int get_size_dir(char *fname, size_t *blocks);
int get_size(char *fname, size_t *blocks);
int is_symlink(char *fname);

/* Function to get size of a file or directory */
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
	else if(S_ISLNK(fileStat.st_mode))
	{
		/* Handle symbolic link by printing its path */
		printf("Link to %s\n", fname);
	}
	else
	{
		fprintf(stderr, "Unknown file type: %s\n", fname);
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;	
}

/* Function to process a directory */
int get_size_dir(char *dname, size_t *blocks)
{
	DIR *dir;

	if((dir = opendir(dname)) == NULL)
	{
		fprintf(stderr, "Error opening directory: %s\n", dname);
		exit(EXIT_FAILURE);
	}

	struct dirent *entry = readdir(dir);

	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			char path[1024];
			sprintf(path, "%s/%s", dname, entry->d_name);

			get_size(path, blocks);	// Recursive call
		}		
	}

	closedir(dir);	// Close directory	
}

/* Main function with getopt handling */
int main(int argc, char *argv[])
{
	int opt;
	char *folder1 = NULL, *folder2 = NULL;
	int threshold = 0;

	/* Parse command-line arguments */
	while ((opt = getopt(argc, argv, "l:s:t:")) != -1)
	{
		switch (opt)
		{
			case 'l':
				folder1 = optarg;
				break;
			case 's':
				folder2 = optarg;
				break;
			case 't':
				threshold = atoi(optarg);
				break;
			default:
				fprintf(stderr, "Usage: %s -l <folder1> -s <folder2> -t <threshold> <files>\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	/* Check if required arguments are provided */
	if (!folder1 || !folder2 || threshold == 0)
	{
		fprintf(stderr, "Error: Missing required arguments\n");
		fprintf(stderr, "Usage: %s -l <folder1> -s <folder2> -t <threshold> <files>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Process files */
	for (int i = optind; i < argc; i++)
	{
		size_t nBlocks = 0;
		get_size(argv[i], &nBlocks);

		/* Check if the file is above or below the threshold */
		if (nBlocks / 2 >= threshold)
		{
			/* File size is above the threshold */
			printf("%ldK %s (move to %s)\n", nBlocks / 2, argv[i], folder1);
		}
		else
		{
			/* File size is below the threshold */
			printf("%ldK %s (move to %s)\n", nBlocks / 2, argv[i], folder2);
		}
	}

	return 0;
}
