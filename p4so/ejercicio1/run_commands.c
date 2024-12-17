#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COMMANDS 100

pid_t launch_command(char** argv) {
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo
        execvp(argv[0], argv);
        perror("execvp failed"); // En caso de error
        exit(EXIT_FAILURE);
    }
    return pid; // Retorna el PID del hijo
}

char **parse_command(const char *cmd, int* argc) {
    size_t argv_size = 10;
    const char *end;
    size_t arg_len; 
    int arg_count = 0;
    const char *start = cmd;
    char **argv = malloc(argv_size * sizeof(char *));

    if (argv == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (*start && isspace(*start)) start++; // Skip leading spaces

    while (*start) {
        if (arg_count >= argv_size - 1) { // Reserve space for NULL
            argv_size *= 2;
            argv = realloc(argv, argv_size * sizeof(char *));
            if (argv == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        end = start;
        while (*end && !isspace(*end)) end++;

        arg_len = end - start;
        argv[arg_count] = malloc(arg_len + 1);
        if (argv[arg_count] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(argv[arg_count], start, arg_len);
        argv[arg_count][arg_len] = '\0'; // Null-terminate the argument
        arg_count++;

        start = end;
        while (*start && isspace(*start)) start++;
    }

    argv[arg_count] = NULL; // Null-terminate the array
    (*argc) = arg_count; // Return argc

    return argv;
}

void execute_from_file(const char *filename, int wait_for_completion) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return;
    }

    char line[256];
    pid_t pids[MAX_COMMANDS];
    int command_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Eliminar nueva línea

        int cmd_argc;
        char **cmd_argv = parse_command(line, &cmd_argc);
        pids[command_count] = launch_command(cmd_argv);
        printf("@@ Running command #%d: %s\n", command_count, line);
        command_count++;

        if (wait_for_completion) {
            int status;
            waitpid(pids[command_count - 1], &status, 0);
            printf("@@ Command #%d terminated (pid: %d, status: %d)\n", command_count - 1, pids[command_count - 1], WEXITSTATUS(status));
        }
    }

    if (!wait_for_completion) {
        for (int i = 0; i < command_count; i++) {
            int status;
            waitpid(pids[i], &status, 0);
            printf("@@ Command #%d terminated (pid: %d, status: %d)\n", i, pids[i], WEXITSTATUS(status));
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    int opt;
    int wait_for_completion = 1; // Por defecto, espera a que terminen los comandos

    while ((opt = getopt(argc, argv, "x:s:b")) != -1) {
        switch (opt) {
            case 'x': {
                int cmd_argc;
                char **cmd_argv = parse_command(optarg, &cmd_argc);
                pid_t pid = launch_command(cmd_argv);
                printf("@@ Running command: %s\n", optarg);
                waitpid(pid, NULL, 0);
                break;
            }
            case 's':
                execute_from_file(optarg, wait_for_completion);
                break;
            case 'b':
                wait_for_completion = 0; // No esperar
                break;
            default:
                fprintf(stderr, "Usage: %s -x <command> | -s <file> [-b]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;

     /*char **cmd_argv;
    int cmd_argc;
    int i;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"command\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    cmd_argv=parse_command(argv[1],&cmd_argc);

    // Print parsed arguments
    printf("argc: %d\n", cmd_argc);
    for (i = 0; cmd_argv[i] != NULL; i++) {
        printf("argv[%d]: %s\n", i, cmd_argv[i]);
        free(cmd_argv[i]);  // Free individual argument
    }

    free(cmd_argv);  // Free the cmd_argv array

    return EXIT_SUCCESS;*/
}


/*
#include <unistd.h>

int execvp(const char *file, char *const argv[]);

se utilizan para ejecutar un programa diferente dentro del proceso actual. En lugar de crear un nuevo proceso (como lo hace fork),
execvp reemplaza la imagen del proceso actual por un nuevo programa. Esto es útil cuando se desea ejecutar un comando dentro de un proceso y reemplazar el programa actual sin crear un proceso hijo.

Parámetros
file:
El nombre del archivo o el comando que se va a ejecutar. Puede ser el nombre de un archivo ejecutable o un comando que esté en el $PATH (una lista de directorios donde se buscan los ejecutables).

argv:
Un arreglo de cadenas que representa los argumentos para el programa a ejecutar, donde:
El primer argumento (argv[0]) debe ser el nombre del archivo ejecutable.
El arreglo debe terminar con un puntero nulo (NULL).

Valor de retorno
execvp no devuelve si tiene éxito. Si se ejecuta correctamente, el proceso actual es reemplazado por el nuevo programa, y el nuevo programa se ejecuta.
Si hay un error (por ejemplo, el archivo no se encuentra o no se puede ejecutar), devuelve -1 y establece errno para describir el error (por ejemplo, ENOENT si el archivo no se encuentra).

#include <stdlib.h>

void* realloc(void *ptr, size_t size);
se utiliza para cambiar el tamaño de un bloque de memoria previamente asignado. 
Es especialmente útil cuando necesitas redimensionar dinámicamente un arreglo o una estructura de datos que ha sido previamente asignada mediante malloc o calloc.

Parámetros
ptr:
Un puntero al bloque de memoria previamente asignado con malloc, calloc o realloc. Si ptr es NULL, realloc actúa como malloc.
size:
El nuevo tamaño en bytes que deseas que tenga el bloque de memoria. Si el tamaño es menor que el tamaño original, la memoria se reduce. Si el tamaño es mayor, se añade más memoria.

Uso de comillas dobles vs simples en -x: Si no se encierran los argumentos de -x entre comillas dobles, 
la terminal puede interpretar los espacios como separadores de argumentos, y por tanto, el comando no se pasará correctamente. 
Las comillas simples pueden funcionar si el shell lo permite, pero es más seguro usar dobles.

Uso de execlp() en lugar de execvp(): Sí, se podría usar execlp(), pero requiere que cada argumento sea pasado como un argumento separado en la llamada a la función.
Esto puede complicar la implementación y es menos flexible que execvp(), que toma un array de argumentos.

Ejecución de comandos con redirecciones: Al ejecutar echo hola > a.txt, 
el redireccionamiento no funcionará correctamente porque el programa no está configurado para manejar redirecciones. 
Para manejar esto, tendrías que utilizar dup2() para redirigir la salida estándar. 
Similarmente, cat run_commands.c | grep int no funcionará sin un manejo de tuberías (pipes) en el código.
*/
