#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 3
#define TYPESTR(type) ((type) ? "profesor" : "estudiante")
#define PROFESSOR 1
#define STUDENT 0

pthread_mutex_t mutex;
pthread_cond_t cond_professors_can_enter, cond_students_can_enter;

struct arg_user {
    int id;
    int type; // 1 para profesor, 0 para estudiante
};

int library_users = 0;
int professors_at_queue = 0;

void enter_student(int id) {
    pthread_mutex_lock(&mutex);

    printf("El estudiante con id %d ha llegado a la cola.\n", id);

    while (library_users >= CAPACITY || professors_at_queue > 0) {
        pthread_cond_wait(&cond_students_can_enter, &mutex);
    }

    printf("El estudiante con id %d ha entrado en la biblioteca.\n", id);
    library_users++;

    pthread_mutex_unlock(&mutex);
}

void enter_professor(int id) {
    professors_at_queue++;

    pthread_mutex_lock(&mutex);

    printf("El profesor con id %d ha llegado a la cola.\n", id);

    while (library_users >= CAPACITY) {
        pthread_cond_wait(&cond_professors_can_enter, &mutex);
    }

    printf("El profesor con id %d ha entrado en la biblioteca.\n", id);
    library_users++;
    professors_at_queue--;

    pthread_mutex_unlock(&mutex);
}

void use_library(int id, int type) {
    printf("El %s con id %d está utilizando la biblioteca.\n", TYPESTR(type), id);
    sleep(2); // Permanecerá en la biblioteca 2 segundos
}

void exit_library(int id, int type) {
    pthread_mutex_lock(&mutex);

    library_users--;
    printf("El %s con id %d ha salido de la biblioteca.\n", TYPESTR(type), id);

    if (library_users < CAPACITY && professors_at_queue > 0) {
        pthread_cond_signal(&cond_professors_can_enter);
    } else if (library_users < CAPACITY) {
        pthread_cond_signal(&cond_students_can_enter);
    }

    pthread_mutex_unlock(&mutex);
}

void *user(void *arg) {
    struct arg_user *user = arg;

    if (user->type == PROFESSOR) {
        enter_professor(user->id);
    } else {
        enter_student(user->id);
    }

    use_library(user->id, user->type);
    exit_library(user->id, user->type);

    free(user);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s archivo\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *fd = fopen(argv[1], "r");
    if (fd == NULL) {
        fprintf(stderr, "No se pudo leer el archivo: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int M;
    if (fscanf(fd, "%d", &M) != 1) {
        fprintf(stderr, "Error al leer el número de usuarios.\n");
        fclose(fd);
        exit(EXIT_FAILURE);
    }

    int *users = malloc(sizeof(int) * M);
    for (int i = 0; i < M; i++) {
        if (fscanf(fd, "%d", &users[i]) != 1) {
            fprintf(stderr, "Error al leer el tipo de usuario %d.\n", i + 1);
            fclose(fd);
            free(users);
            exit(EXIT_FAILURE);
        }
    }

    fclose(fd);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_professors_can_enter, NULL);
    pthread_cond_init(&cond_students_can_enter, NULL);

    pthread_t *threads = malloc(sizeof(pthread_t) * M);

    for (int i = 0; i < M; i++) {
        struct arg_user *arg = malloc(sizeof(struct arg_user));
        arg->id = i + 1;
        arg->type = users[i];

        pthread_create(&threads[i], NULL, user, arg);
    }

    free(users);

    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_professors_can_enter);
    pthread_cond_destroy(&cond_students_can_enter);

    return 0;
}
