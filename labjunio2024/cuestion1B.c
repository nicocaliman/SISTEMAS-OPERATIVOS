#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 3
#define PROFESSOR 1
#define STUDENT 0

pthread_mutex_t mutex;
pthread_cond_t cond_professors_can_enter, cond_students_can_enter, cond_library_open;

int library_users = 0;
int professors_at_queue = 0;
int students_at_queue = 0;
int library_open = 1; // 1 if open, 0 if closed

struct user_args {
    int id;
    int role; // PROFESSOR or STUDENT
};

void *library_control(void *arg) {
    while (1) {
        sleep(4); // Open for 4 seconds
        pthread_mutex_lock(&mutex);
        library_open = 0;
        printf("Library is closing for vacation.\n");
        pthread_cond_broadcast(&cond_library_open);
        pthread_mutex_unlock(&mutex);

        sleep(2); // Closed for 2 seconds
        pthread_mutex_lock(&mutex);
        library_open = 1;
        printf("Library is now open after vacation.\n");
        pthread_cond_broadcast(&cond_library_open);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void enter_student(int id) {
    pthread_mutex_lock(&mutex);
    students_at_queue++;
    printf("User %d (student) waiting on the queue.\n", id);

    while (!library_open || library_users >= CAPACITY || professors_at_queue > 0) {
        pthread_cond_wait(&cond_students_can_enter, &mutex);
    }

    students_at_queue--;
    library_users++;
    printf("User %d (student) is reading books for 2 seconds.\n", id);
    pthread_mutex_unlock(&mutex);

    sleep(2);
}

void enter_professor(int id) {
    pthread_mutex_lock(&mutex);
    professors_at_queue++;
    printf("User %d (professor) waiting on the queue.\n", id);

    while (!library_open || library_users >= CAPACITY) {
        pthread_cond_wait(&cond_professors_can_enter, &mutex);
    }

    professors_at_queue--;
    library_users++;
    printf("User %d (professor) is reading books for 2 seconds.\n", id);
    pthread_mutex_unlock(&mutex);

    sleep(2);
}

void exit_library(int id, int role) {
    pthread_mutex_lock(&mutex);
    library_users--;
    printf("User %d (%s) leaves the library.\n", id, role == PROFESSOR ? "professor" : "student");

    if (library_open && library_users < CAPACITY) {
        if (professors_at_queue > 0) {
            pthread_cond_signal(&cond_professors_can_enter);
        } else if (students_at_queue > 0) {
            pthread_cond_signal(&cond_students_can_enter);
        }
    }

    pthread_mutex_unlock(&mutex);
}

void *user_thread(void *arg) {
    struct user_args *user = arg;

    if (user->role == PROFESSOR) {
        enter_professor(user->id);
    } else {
        enter_student(user->id);
    }

    exit_library(user->id, user->role);

    free(user);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int user_count;
    if (fscanf(file, "%d", &user_count) != 1) {
        fprintf(stderr, "Error reading user count\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int *roles = malloc(sizeof(int) * user_count);
    for (int i = 0; i < user_count; i++) {
        if (fscanf(file, "%d", &roles[i]) != 1) {
            fprintf(stderr, "Error reading role for user %d\n", i);
            fclose(file);
            free(roles);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_professors_can_enter, NULL);
    pthread_cond_init(&cond_students_can_enter, NULL);
    pthread_cond_init(&cond_library_open, NULL);

    pthread_t *threads = malloc(sizeof(pthread_t) * user_count);
    pthread_t control_thread;
    pthread_create(&control_thread, NULL, library_control, NULL);

    for (int i = 0; i < user_count; i++) {
        struct user_args *args = malloc(sizeof(struct user_args));
        args->id = i;
        args->role = roles[i];
        pthread_create(&threads[i], NULL, user_thread, args);
    }

    for (int i = 0; i < user_count; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_cancel(control_thread);
    pthread_join(control_thread, NULL);

    free(threads);
    free(roles);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_professors_can_enter);
    pthread_cond_destroy(&cond_students_can_enter);
    pthread_cond_destroy(&cond_library_open);

    return 0;
}
