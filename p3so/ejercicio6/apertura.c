#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define WRITE 1
#define READ 2

extern char *optarg;
extern int optind;

int openmode(int mode);

int openmode(int mode)
{
    int r;

    switch (mode)
    {
    case WRITE:
        r = O_WRONLY;
        break;
    
    case READ:
        r = O_RDONLY;
        break;

    case READ + WRITE:
        r = O_RDWR;
        break;

    default:
        fprintf(stderr, "Modo de apertura sin especificar o incorrecto\n");
        exit(EXIT_FAILURE);
        break;
    }

    return r;
}

int main(int argc, char* argv[])
{
    int opt;
    char *fName;
    int mode = 0;
    int flag;

    while ((opt = getopt(argc, argv, "f:rw")) != -1)
    {
        switch (opt)
        {
        case 'f':
            flag = 1;
            fName = optarg;
            break;

        case 'r':
            mode += READ;
            break;

        case 'w':
            mode += WRITE;
            break;
        
        default:
            fprintf(stderr, "Usage: %s -f filename <-r> <-w>\n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }
    
    if (!flag)
    {
        fprintf(stderr, "Se espera argumento despues de la opcion");
        exit(EXIT_FAILURE);
    }
    
    int fd;
    int open_mode = openmode(mode);

    if((fd = open(fName, open_mode, O_CREAT | O_TRUNC)) == -1)
    {
        fprintf(stderr, "no se ha podido abrir el fichero\n");
        exit(EXIT_FAILURE);
    }


    if(write(fd, "escritura\n", 11) == -1)
    {
        perror("write");
    }

    else
    {
        printf("se escribio correctamente\n");        
    }

    char buffer_lectura[11];

    lseek(fd, 0, SEEK_SET);

    if (read(fd, buffer_lectura, 11) == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("se leyo correctamente\n"); 
        printf("%s\n", buffer_lectura);       
    }

    close(fd);

    return 0;
}