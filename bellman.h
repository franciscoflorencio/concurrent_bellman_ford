#ifndef BELLMAN_H
#define BELLMAN_H

#include "graph.h"
#include "barrier.h"

// definição do infinity que será utilizado no algoritmo
#include <limits.h>

#define INT_MAX_HALF (2147483647 / 2)
#define INFINITY INT_MAX_HALF

#define NEG_INF (-INT_MAX_HALF)

// estrutura representante dos argumentos que serão passados para cada thread
typedef struct {
    int start_edge;
    int end_edge;
    const int *dist_old;  // somente leitura
    int *dist_new;        // somente escrita (sem corrida: cada thread escreve em arestas disjuntas? NÃO — mas permitimos corridas, está OK!)
    Graph *graph;
} t_args;

// 
void bellman_ford(Graph* graph, int src, int *dist, int *parent);

// funções do Bellman-Ford paralelo
void parallel_bellman_ford(Graph *graph, int src, int num_threads, int *dist, int *parent);

#endif // BELLMAN_H
