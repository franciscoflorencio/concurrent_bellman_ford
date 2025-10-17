#include "barrier.h"

#include <pthread.h>

// função para inicializar a barreira
void barrier_init(Barrier *barrier, int num_threads)
{
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    barrier->count = 0;
    barrier->total_threads = num_threads;
}

// função de wait da barreira
void barrier_wait(Barrier *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    barrier->count++;
    if (barrier->count == barrier -> total_threads)
    {
        // isso significa que estamos na última thread a chegar na barreira
        barrier->count = 0; // reset para a próxima utilização
        pthread_cond_broadcast(&barrier->cond);
    }
    else
    {
        // aguarda até que todas as threads cheguem na barreira
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }
    pthread_mutex_unlock(&barrier->mutex);
}

// função para "destruir" o mutex e o cond dentro da barreira
void barrier_destroy(Barrier *barrier)
{
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
}
