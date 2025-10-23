#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "bellman.h"

int main()
{
    int V, E;
    const int NUM_THREADS = 4;

    // cenario 1: todos os pesos positivos
    V = 4;
    E = 4;
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));

    // s = 0, t = 1, x = 2, y = 3, z = 4
    graph->edge[0] = (Edge){0, 1, 10}; // aresta s->t com peso 10
    graph->edge[1] = (Edge){0, 3, 5}; // aresta s->y com peso 5
    graph->edge[2] = (Edge){1, 3, 2}; // aresta t->y com peso 2
    graph->edge[3] = (Edge){1, 2, 1}; // aresta t->x com peso 1
    graph->edge[4] = (Edge){2, 4, 4}; // aresta x->z com peso 4
    graph->edge[5] = (Edge){3, 1, 3}; // aresta y->t com peso 3
    graph->edge[6] = (Edge){3, 2, 9}; // aresta y->x com peso 9
    graph->edge[7] = (Edge){3, 4, 2}; // aresta y->z com peso 2
    graph->edge[8] = (Edge){4, 2, 6}; // aresta z->x com peso 6
    graph->edge[9] = (Edge){4, 0, 7}; // aresta z->s com peso 7

    int source_vertex = 0;
    int num_threads = (NUM_THREADS > E) ? E : NUM_THREADS;

    int source_vertex = 0; // vértice fonte (A)

    if (NUM_THREADS > E) {
        num_threads = E; // não faz sentido ter mais threads que arestas
    }

    parallel_bellman_ford(graph, source_vertex, NUM_THREADS);

    free(graph->edge);
    free(graph);

    // cenario 2: pesos negativos, sem ciclos negativos (caso clássico)
    graph = (Graph *)malloc(sizeof(Graph));
    V = 5;
    E = 8;
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));
    graph->edge[0] = (Edge){0, 1, -1}; // aresta 0->1 com peso -1
    graph->edge[1] = (Edge){0, 2, 4}; // aresta 0->2 com peso 4
    graph->edge[2] = (Edge){1, 2, 3}; // aresta 1->2 com peso 3
    graph->edge[3] = (Edge){1, 3, 2}; // aresta 1->3 com peso 2
    graph->edge[4] = (Edge){1, 4, 2}; // aresta 1->4 com peso 2
    graph->edge[5] = (Edge){3, 2, 5}; // aresta 3->2 com peso 5
    graph->edge[6] = (Edge){3, 1, 1}; // aresta 3->1 com peso 1
    graph->edge[7] = (Edge){4, 3, -3}; // aresta 4->3 com peso -3
    
    source_vertex = 0;
    parallel_bellman_ford(graph, source_vertex, num_threads);
    free(graph->edge);
    free(graph);

    // cenario 3: ciclo negativo alcançável
    graph = (Graph *)malloc(sizeof(Graph));
    V = 3;
    E = 3;
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));
    graph->edge[0] = (Edge){0, 1, 1}; // aresta 0->1 com peso 1
    graph->edge[1] = (Edge){1, 2, -1}; // aresta 1->2 com peso -1
    graph->edge[2] = (Edge){2, 0, -1}; // aresta 2->0 com peso -1
    
    source_vertex = 0;
    parallel_bellman_ford(graph, source_vertex, num_threads);
    free(graph->edge);
    free(graph);

    // cenario 4: ciclo negativo não alcançável
    graph = (Graph *)malloc(sizeof(Graph));
    V = 4;
    E = 4;
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));
    graph->edge[0] = (Edge){0, 1, 1}; // aresta 0->1 com peso 1
    graph->edge[1] = (Edge){1, 2, 2}; // aresta 1->2 com peso 2
    graph->edge[2] = (Edge){2, 3, 3}; // aresta 2->3 com peso 3
    graph->edge[3] = (Edge){3, 1, -6}; // aresta 3->1 com peso -6

    source_vertex = 0;
    parallel_bellman_ford(graph, source_vertex, num_threads);
    free(graph->edge);
    free(graph);

    // cenario 5: grafo desconexo
    graph = (Graph *)malloc(sizeof(Graph));
    V = 6;
    E = 4;
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));
    graph->edge[0] = (Edge){0, 1, 2}; // aresta 0->1 com peso 2
    graph->edge[1] = (Edge){1, 2, 3}; // aresta 1->2 com peso 3
    graph->edge[2] = (Edge){3, 4, 4}; // aresta 3->4 com peso 4
    graph->edge[3] = (Edge){4, 5, 5}; // aresta 4->5 com peso 5
    source_vertex = 0;
    parallel_bellman_ford(graph, source_vertex, num_threads);
    free(graph->edge);
    free(graph);

    // cenario 6: grafo sem arestas
    graph = (Graph *)malloc(sizeof(Graph));
    V = 5;
    E = 0;
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));
    source_vertex = 0;
    parallel_bellman_ford(graph, source_vertex, num_threads);
    free(graph->edge);
    free(graph);

    return 0;
}