#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "graph.h"

// estrutura para armazenar métricas de desempenho
typedef struct {
    double execution_time;    // tempo total de execução
    int num_threads;         // número de threads usado
    int num_vertices;        // número de vértices do grafo
    int num_edges;          // número de arestas do grafo
} Performance;

// função para gerar um grafo aleatório com V vértices e E arestas
Graph* generate_random_graph(int V, int E) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge*)malloc(E * sizeof(Edge));

    srand(time(NULL));
    for (int i = 0; i < E; i++) {
        graph->edge[i].src = rand() % V;
        graph->edge[i].dest = rand() % V;
        // peso de cada aresta entre -10 e 10 para ter alguns negativos
        graph->edge[i].weight = (rand() % 21) - 10;
    }
    return graph;
}

// função para medir o tempo em microsegundos
double get_time_usec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000.0 + tv.tv_usec;
}

// função para calcular speedup
double calculate_speedup(double serial_time, double parallel_time) {
    return serial_time / parallel_time;
}

// função para calcular eficiência
double calculate_efficiency(double speedup, int num_threads) {
    return speedup / num_threads;
}

#endif // TEST_UTILS_H