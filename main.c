#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "bellman.h"

void executar_caso(int caso, Edge edges[], int V, int E, int source_vertex, int NUM_THREADS)
{
    printf("\n===== CASO %d =====\n", caso);

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));

    for (int i = 0; i < E; i++) {
        graph->edge[i] = edges[i];
    }

    if (NUM_THREADS > E) {
        NUM_THREADS = E;
    }

    parallel_bellman_ford(graph, source_vertex, NUM_THREADS);

    free(graph->edge);
    free(graph);
}

int main()
{
    int source_vertex = 0;
    int NUM_THREADS = 2;

    // CASO 1: Grafo sem ciclo negativo
    {
        int V = 5;
        int E = 9;
        Edge edges[] = {
            {0, 1, 4}, {0, 2, 2}, {1, 2, 3}, {1, 3, 2}, {1, 4, 3},
            {2, 1, 1}, {2, 3, 4}, {2, 4, 5}, {4, 3, 3}
        };
        executar_caso(1, edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 2: Grafo com peso negativo mas sem ciclo negativo
    {
        int V = 4;
        int E = 7;
        Edge edges[] = {
            {0, 1, -1}, {0, 2, 4}, {1, 2, 3}, {1, 3, 2}, {1, 4, 2},
            {3, 2, 5}, {3, 1, 1}, {4, 3, -2}
        };
        executar_caso(2, edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 3: Grafo com ciclo negativo
    {
        int V = 4;
        int E = 5;
        Edge edges[] = {
            {0, 1, 1}, {1, 2, -1}, {2, 3, -1}, {3, 1, -1}, {0, 3, 4}
        };
        executar_caso(3, edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 4: Grafo desconexo
    {
        int V = 6;
        int E = 4;
        Edge edges[] = {
            {0, 1, 2}, {1, 2, 3}, {3, 4, 1}, {4, 5, 2}
        };
        executar_caso(4, edges, V, E, source_vertex, NUM_THREADS);
    }

    return 0;
}