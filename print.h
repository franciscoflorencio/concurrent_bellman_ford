#ifndef PRINT_H
#define PRINT_H

#include "graph.h"
#include "bellman.h"
#include <stdio.h>
#include <stdlib.h>

void print_result(const Graph* graph, const char* filename, int dist[], int parent[], int source_vertex);

void print_graph(const Graph* graph, const char* filename);

void printar_caso(int caso, const char* descricao, Edge edges[], int V, int E, int source_vertex, int NUM_THREADS);

#endif // PRINT_H