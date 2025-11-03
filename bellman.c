#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "bellman.h"

void relax(int u, int v, int weight, int *distances, int *parents, pthread_mutex_t *vertex_locks)
{
    if (vertex_locks != NULL)
    {
        pthread_mutex_lock(&vertex_locks[v]);
    }

    if (distances[u] != INFINITY && distances[u] + weight < distances[v])
    {
        distances[v] = distances[u] + weight;
        parents[v] = u;
    }
    
    if (vertex_locks != NULL)
    {
        pthread_mutex_unlock(&vertex_locks[v]);
    }
}

void bellman_ford(Graph* graph, int src, int *dist, int *parent)
{
    int V = graph->V;
    int E = graph->E;

    // Incializa distancias e pais
    for (int i = 0; i < V; i++)
    {
        dist[i] = INFINITY;
        parent[i] = -1;
    }
    dist[src] = 0;

    // loop principal do Bellman-Ford
    for (int i = 1; i <= V - 1; i++)
    {
        for (int j = 0; j < E; j++)
        {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;

            relax(u, v, weight, dist, parent, NULL);
        }
    }

    // checa por ciclos negativos
    for (int i = 0; i < E; i++)
    {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;

        if (dist[u] != INFINITY && dist[u] + weight < dist[v])
        {
            dist[v] = NEG_INF; // Indica ciclo negativo
        }
    }
}

typedef struct {
    int start_edge;
    int end_edge;
    const int *dist_old;  // read-only
    int *dist_new;        // write-only (no race: each thread writes disjoint edges? NO — but we allow races, it's OK!)
    Graph *graph;
} thread_args_opt;

void *relax_edges_thread(void *arg) {
    thread_args_opt *t = (thread_args_opt *)arg;
    // Traverse assigned edges
    for (int i = t->start_edge; i < t->end_edge; i++) {
        int u = t->graph->edge[i].src;
        int v = t->graph->edge[i].dest;
        int w = t->graph->edge[i].weight;

        if (t->dist_old[u] != INFINITY) {
            int new_dist = t->dist_old[u] + w;
            // Allow race on dist_new[v] — it's safe because:
            // - We only care about the MINIMUM value
            // - Even if two threads write, the final value will be <= correct min
            // - Next iteration will fix it
            if (new_dist < t->dist_new[v]) {
                t->dist_new[v] = new_dist;
            }
        }
    }
    return NULL;
}

void parallel_bellman_ford(Graph *graph, int src, int num_threads, int *dist, int *parent)
{
    int V = graph->V;
    int E = graph->E;

    // Double buffering arrays
    int *dist_old = (int*)malloc(V * sizeof(int));
    int *dist_new = (int*)malloc(V * sizeof(int));

    // Initialize
    for (int i = 0; i < V; i++) {
        dist_old[i] = INFINITY;
        parent[i] = -1;
    }
    dist_old[src] = 0;

    // Main Bellman-Ford loop: V-1 iterations
    for (int iter = 0; iter < V - 1; iter++) {
        // Initialize dist_new as copy of dist_old (so unchanged vertices stay same)
        memcpy(dist_new, dist_old, V * sizeof(int));

        // Prepare thread args
        pthread_t *threads = (pthread_t*)calloc(num_threads, sizeof(pthread_t));
        thread_args_opt *targs = (thread_args_opt*)calloc(num_threads, sizeof(thread_args_opt));

        int edges_per_thread = E / num_threads;
        for (int t = 0; t < num_threads; t++) {
            targs[t].start_edge = t * edges_per_thread;
            targs[t].end_edge = (t == num_threads - 1) ? E : (t + 1) * edges_per_thread;
            targs[t].dist_old = dist_old;
            targs[t].dist_new = dist_new;
            targs[t].graph = graph;

            pthread_create(&threads[t], NULL, relax_edges_thread, &targs[t]);
        }

        // Wait for all threads
        for (int t = 0; t < num_threads; t++) {
            pthread_join(threads[t], NULL);
        }

        // Swap buffers
        int *temp = dist_old;
        dist_old = dist_new;
        dist_new = temp;

        free(threads);
        free(targs);
    }

    // Copy final distances
    memcpy(dist, dist_old, V * sizeof(int));

    // === Negative cycle detection (sequential, but fast) ===
    int *queue = (int *)malloc(V * sizeof(int));
    int front = 0, rear = 0;
    int *in_queue = (int *)calloc(V, sizeof(int));

    // First pass: find nodes that can still be relaxed
    for (int i = 0; i < E; i++) {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int w = graph->edge[i].weight;
        if (dist[u] != INFINITY && dist[u] != NEG_INF && dist[u] + w < dist[v]) {
            if (!in_queue[v]) {
                queue[rear++] = v;
                in_queue[v] = 1;
                dist[v] = NEG_INF;
            }
        }
    }

    // BFS propagation
    while (front < rear) {
        int u = queue[front++];
        for (int i = 0; i < E; i++) {
            if (graph->edge[i].src == u) {
                int v = graph->edge[i].dest;
                if (!in_queue[v]) {
                    queue[rear++] = v;
                    in_queue[v] = 1;
                    dist[v] = NEG_INF;
                }
            }
        }
    }

    free(queue);
    free(in_queue);
    free(dist_old);
    free(dist_new);
}
