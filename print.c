#include "print.h"

void print_graph(const Graph* graph, const char* filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) 
    {
        printf("Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }

    // Bellman-Ford usa vértices direcionados, portanto chamamos de "digraph"
    fprintf(fp, "digraph G {\n");
    fprintf(fp, " rankdir=LR;\n");

    // escreve as vértices
    for(int i = 0; i < graph->V; i++)
    {
        fprintf(fp, " %d [label=\"%d\"];\n", i, i);
    }

    // escreve as arestas
    for(int i = 0; i < graph->E; i++)
    {
        Edge edge = graph->edge[i];
        fprintf(fp, " %d -> %d [label=\"%d\"];\n", edge.src, edge.dest, edge.weight);
    }

    fprintf(fp, "}\n");
    fclose(fp);
    printf("Grafo escrito em %s\n", filename);
}

void print_result(const Graph* graph, const char* filename, int dist[], int parent[], int source_vertex)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) 
    {
        printf("Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }

    fprintf(fp, "digraph G {\n");
    fprintf(fp, " rankdir=LR;\n");

    // escreve as vértices
    for(int i = 0; i < graph->V; i++)
    {
        char label_text[100];
        if(dist[i] == INFINITY) {
            sprintf(label_text, "%d (INF)", i);
        } else {
            sprintf(label_text, "%d (%d)", i, dist[i]);
        }

        // START node attributes
        fprintf(fp, " %d [label=\"%s\"", i, label_text);

        // Append style attributes
        if(i == source_vertex) {
            fprintf(fp, ", style=filled, fillcolor=lightblue");
        } else if (dist[i] == NEG_INF) { // ciclo negativo
            fprintf(fp, ", style=filled, fillcolor=red");
        }
        
        // END node attributes
        fprintf(fp, "];\n");
    }

    // escreve as arestas do caminho mínimo
    for(int i = 0; i < graph->E; i++)
    {
        int src = graph->edge[i].src;
        int dest = graph->edge[i].dest;
        int weight = graph->edge[i].weight;

        // Start edge attributes (always include label)
        fprintf(fp, " %d -> %d [label=\"%d\"", src, dest, weight);

        // Append shortest path style
        if (parent[dest] == src)
        {
            fprintf(fp, ", color=blue, penwidth=2.0");
        }
        
        // End edge attributes
        fprintf(fp, "];\n");
    }

    fprintf(fp, "}\n");
    fclose(fp);
    printf("Resultado escrito em %s\n", filename);
}


void printar_caso(int caso, const char* descricao, Edge edges[], int V, int E, int source_vertex, int NUM_THREADS)
{
    printf("\n===== CASO %d: %s =====\n", caso, descricao);
    printf("Vertices: %d, Arestas: %d\n", V, E);

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));

    for (int i = 0; i < E; i++) {
        graph->edge[i] = edges[i];
    }

    int dist[V];
    int parent[V];

    char filename[50];
    sprintf(filename, "graphs/caso_%d.dot", caso);
    print_graph(graph, filename);

    if (NUM_THREADS > E) {
        NUM_THREADS = E;
    }

    parallel_bellman_ford(graph, source_vertex, NUM_THREADS, dist, parent);

    char filename_res[100];
    sprintf(filename_res, "correctness/caso_%d_threads_%d.dot", caso, NUM_THREADS);
    print_result(graph, filename_res, dist, parent, source_vertex);

    free(graph->edge);
    free(graph);
}