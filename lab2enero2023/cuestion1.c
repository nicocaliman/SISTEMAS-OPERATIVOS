#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BUF 1024

// Función para verificar si un archivo es regular
int es_archivo_regular(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        perror("Error al obtener información del archivo");
        return 0;
    }
    return S_ISREG(statbuf.st_mode);
}

// Función para leer los primeros n bytes de un archivo
void leer_archivo(const char *path, int nbytes, FILE *salida) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Error al abrir el archivo");
        return;
    }

    unsigned char buffer[MAX_BUF];
    size_t bytes_leidos;
    while ((bytes_leidos = fread(buffer, 1, sizeof(buffer), file)) > 0 && nbytes > 0) {
        size_t bytes_a_escribir = (nbytes < bytes_leidos) ? nbytes : bytes_leidos;
        fwrite(buffer, 1, bytes_a_escribir, salida);
        nbytes -= bytes_a_escribir;
    }

    fclose(file);
}

// Función principal con getopt
int main(int argc, char *argv[]) {
    int nbytes = 0;
    char *outFileName = NULL;
    int opt;

    // Procesar los argumentos con getopt
    while ((opt = getopt(argc, argv, "n:o:")) != -1) {
        switch (opt) {
            case 'n':
                nbytes = atoi(optarg);
                break;
            case 'o':
                outFileName = optarg;
                break;
            default:
                fprintf(stderr, "Uso: %s -n <numBytes> [-o <outFileName>]\n", argv[0]);
                return 1;
        }
    }

    // Validar que se haya proporcionado la opción -n
    if (nbytes <= 0) {
        fprintf(stderr, "Debe proporcionarse un valor válido para -n\n");
        return 1;
    }

    // Si no se proporciona archivo de salida, escribir en stdout
    FILE *salida = stdout;
    if (outFileName != NULL) {
        salida = fopen(outFileName, "wb");
        if (!salida) {
            perror("Error al abrir el archivo de salida");
            return 1;
        }
    }

    // Recorrer el directorio actual y procesar los archivos regulares
    DIR *dir = opendir(".");
    if (!dir) {
        perror("Error al abrir el directorio");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignorar los directorios y enlaces simbólicos
        if (entry->d_type == DT_REG) {
            // Es un archivo regular, leer los primeros nbytes
            leer_archivo(entry->d_name, nbytes, salida);
            fwrite("\n", 1, 1, salida); // Nueva línea después de cada archivo
        }
    }

    closedir(dir);

    if (salida != stdout) {
        fclose(salida); // Cerrar el archivo de salida si se abrió
    }

    return 0;
}
