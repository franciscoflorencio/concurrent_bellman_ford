#ifndef BARRIER_H
#define BARRIER_H

#include <pthread.h>

// estrutura representante de uma barreira
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int total_threads;
    int cycle;
} Barrier;

// função para inicializar a barreira
void barrier_init(Barrier *barrier, int num_threads);

// função de wait da barreira
void barrier_wait(Barrier *barrier);

// função para "destruir" o mutex e o cond dentro da barreira
void barrier_destroy(Barrier *barrier);

#endif // BARRIER_H
