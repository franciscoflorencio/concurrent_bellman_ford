#ifndef GENERATE_H
#define GENERATE_H

#include "graph.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>

void generate_complete_graph_edges(Edge *edges, int V, int *E, int with_negative);

void generate_sparse_connected_edges(Edge *edges, int V, int *E, int with_negative);

#endif // GENERATE_H