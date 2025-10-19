#ifndef BELLMAN_H
#define BELLMAN_H

#include "graph.h"
#include "barrier.h"

// definição do infinity que será utilizado no algoritmo
#include <limits.h>
#define INFINITY INT_MAX

// estrutura representante dos argumentos que serão passados para cada thread
typedef struct
{
    int thread_id;
    int start_edge;
    int num_edges_for_thread;
    int *distances;
    Graph *graph;
    Barrier *barrier;
} t_args;

// 
void bellman_ford(Graph* graph, int src);

// funções do Bellman-Ford paralelo
void parallel_bellman_ford(Graph *graph, int src, int num_threads);

#endif // BELLMAN_H
