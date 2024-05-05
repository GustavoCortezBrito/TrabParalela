#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_SENSORES 5
#define N_ATUADORES 3

pthread_mutex_t mutex_atuadores = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int atuadores[N_ATUADORES];
} CentralControle;

void* sensor_thread(void* arg) {
    CentralControle* central = (CentralControle*)arg;
    while (1) {
        int ds = rand() % 1001;
        int atuador = ds % N_ATUADORES;
        int nivel = rand() % 101;
        
        pthread_mutex_lock(&mutex_atuadores);
        printf("Alterando: %d com valor %d\n", atuador, nivel);
        central->atuadores[atuador] = nivel;
        pthread_mutex_unlock(&mutex_atuadores);
        
        printf("Dado sensorial: %d. Atuador: %d. Nível de atividade: %d\n", ds, atuador, nivel);
        sleep(rand() % 5 + 1);
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    CentralControle central;
    for (int i = 0; i < N_ATUADORES; i++) {
        central.atuadores[i] = 0;
    }

    pthread_t threads[N_SENSORES];
    for (int i = 0; i < N_SENSORES; i++) {
        pthread_create(&threads[i], NULL, sensor_thread, &central);
    }

    for (int i = 0; i < N_SENSORES; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
