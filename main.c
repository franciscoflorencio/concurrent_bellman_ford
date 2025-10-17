#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "bellman.h"

int main(int argc, char *argv[])
{
    // A = 0, B = 1, C = 2, D = 3, E = 4
    int V = 5; // número de vértices
    int E = 9; // número de arestas
    int NUM_THREADS = 2; // número de threads

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));

    graph->edge[0] = (Edge){0, 1, 4};  // A -> B
    graph->edge[1] = (Edge){0, 2, 2};  // A -> C
    graph->edge[2] = (Edge){1, 2, 3};  // B -> C
    graph->edge[3] = (Edge){1, 3, 2};  // B -> D
    graph->edge[4] = (Edge){1, 4, 3};  // B -> E
    graph->edge[5] = (Edge){2, 1, 1};  // C -> B
    graph->edge[6] = (Edge){2, 3, 4};  // C -> D
    graph->edge[7] = (Edge){2, 4, 5};  // C -> E
    graph->edge[8] = (Edge){4, 3, -5}; // E -> D

    int source_vertex = 0; // vértice fonte (A)

    if (NUM_THREADS > E) {
        NUM_THREADS = E; // não faz sentido ter mais threads do que arestas
    }

    parallel_bellman_ford(graph, source_vertex, NUM_THREADS);

    free(graph->edge);
    free(graph);

    return 0;
}