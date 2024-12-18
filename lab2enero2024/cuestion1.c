#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMANDS 100

pid_t run_command(const char* command) {
    pid_t pid = fork();
    if (pid == 0) {
        // Proceso hijo
        execl("/bin/bash", "bash", "-c", command, NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error en fork
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid; // Proceso padre
}

void execute_option_x(const char* command) {
    pid_t pid = run_command(command);
    int status;
    waitpid(pid, &status, 0);
}

void execute_option_s(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    int command_count = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Remove newline
        printf("@@ Running command #%d: %s\n", command_count, line);
        pid_t pid = run_command(line);
        int status;
        waitpid(pid, &status, 0);
        printf("@@ Command #%d terminated (pid: %d, status: %d)\n", command_count, pid, WEXITSTATUS(status));
        command_count++;
    }

    fclose(file);
}

void execute_option_b(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char* commands[MAX_COMMANDS];
    pid_t pids[MAX_COMMANDS];
    int command_count = 0;

    char line[1024];
    while (fgets(line, sizeof(line), file) && command_count < MAX_COMMANDS) {
        line[strcspn(line, "\n")] = 0; // Remove newline
        commands[command_count] = strdup(line);
        printf("@@ Running command #%d: %s\n", command_count, commands[command_count]);
        pids[command_count] = run_command(commands[command_count]);
        command_count++;
    }

    fclose(file);

    for (int i = 0; i < command_count; i++) {
        int status;
        pid_t pid = waitpid(pids[i], &status, 0);
        printf("@@ Command #%d terminated (pid: %d, status: %d)\n", i, pid, WEXITSTATUS(status));
        free(commands[i]);
    }
}

int main(int argc, char* argv[]) {
    int opt;
    char* x_command = NULL;
    char* s_file = NULL;
    int b_flag = 0;

    while ((opt = getopt(argc, argv, "x:s:b")) != -1) {
        switch (opt) {
            case 'x':
                x_command = optarg;
                break;
            case 's':
                s_file = optarg;
                break;
            case 'b':
                b_flag = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-x command] [-s file] [-b]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (x_command) {
        execute_option_x(x_command);
    }

    if (s_file) {
        if (b_flag) {
            execute_option_b(s_file);
        } else {
            execute_option_s(s_file);
        }
    }

    return 0;
}
