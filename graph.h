#ifndef GRAPH_H
#define GRAPH_H

// estrutura representante de uma aresta com peso de um grafo
typedef struct
{
    int src, dest, weight;
} Edge;

// estrutura representante de um grafo
typedef struct
{
    int V, E;
    Edge *edge;
} Graph;

#endif // GRAPH_H
