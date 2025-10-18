#include <stdio.h>
#include <stdlib.h>
#include "test_utils.h"
#include "bellman.h"

// configuração dos casos de teste
#define NUM_RUNS 3  // número de execuções para cada configuração (reduzido devido ao tamanho maior dos grafos)
#define MAX_THREADS 12  // testando até 16 threads para melhor análise de escalabilidade

typedef struct {
    int V;              // número de vértices
    int E;              // número de arestas
    char* desc;         // descrição do caso de teste
    double serial_time; // tempo da execução sequencial
} TestCase;

// casos de teste com diferentes tamanhos de grafo
TestCase test_cases[] = {
    {1000, 10000,     "Grafo médio, densidade média",      0.0},  // 1K vertices
    {5000, 50000,     "Grafo grande, densidade média",     0.0},  // 5K vertices
    {10000, 100000,   "Grafo muito grande, densidade média", 0.0}, // 10K vertices
};

int main() {
    printf("Iniciando testes de desempenho do Bellman-Ford Paralelo\n");
    printf("Configuração do sistema:\n");
    system("lscpu | grep 'Model name\\|CPU(s)\\|Thread'");
    printf("\nCada caso será executado %d vezes\n\n", NUM_RUNS);

    FILE *csv = fopen("performance_results.csv", "w");
    if (csv == NULL) {
        printf("Erro ao abrir arquivo performance_results.csv\n");
        return 1;
    }
    fprintf(csv, "Caso,Vertices,Arestas,Threads,Tempo_Medio,Aceleracao,Eficiencia\n");
    fflush(csv);

    for (size_t i = 0; i < sizeof(test_cases)/sizeof(test_cases[0]); i++) {
        TestCase tc = test_cases[i];
        printf("\n=================================================================");
        printf("\nCaso de teste %zu: %s", i+1, tc.desc);
        printf("\nVértices: %d, Arestas: %d", tc.V, tc.E);
        printf("\n=================================================================\n");
        
        printf("Gerando grafo aleatório...\n");
        Graph* graph = generate_random_graph(tc.V, tc.E);
        
        for (int num_threads = 1; num_threads <= MAX_THREADS; num_threads *= 2) {
            double total_time = 0;
            
            printf("\nExecutando com %d thread(s):\n", num_threads);
            
            for (int run = 0; run < NUM_RUNS; run++) {
                double start_time = get_time_usec();
                
                parallel_bellman_ford(graph, 0, num_threads);
                
                double end_time = get_time_usec();
                double run_time = (end_time - start_time) / 1000000.0; // converte para segundos
                total_time += run_time;
                
                printf("  Execução %d: %.4f segundos\n", run + 1, run_time);
            }
            
            double avg_time = total_time / NUM_RUNS;
            
            if (num_threads == 1) {
                // versão sequencial - guarda o tempo como referência
                printf("\n  Média: %.4f segundos\n", avg_time);
                fprintf(csv, "%s,%d,%d,%d,%.4f,1.00,1.00\n",
                        tc.desc, tc.V, tc.E, num_threads, avg_time);
                fflush(csv);
                tc.serial_time = avg_time;  // guarda para calcular aceleração
            } else {
                // versão paralela - calcula aceleração e eficiência
                double speedup = calculate_speedup(tc.serial_time, avg_time);
                double efficiency = calculate_efficiency(speedup, num_threads);
                
                printf("\n  Média: %.4f segundos", avg_time);
                printf("\n  Aceleração: %.2fx", speedup);
                printf("\n  Eficiência: %.2f%%\n", efficiency * 100);
                
                fprintf(csv, "%s,%d,%d,%d,%.4f,%.2f,%.2f\n",
                        tc.desc, tc.V, tc.E, num_threads,
                        avg_time, speedup, efficiency);
                fflush(csv);
            }
        }
        
        free(graph->edge);
        free(graph);
    }
    
    fclose(csv);
    printf("\nTestes concluídos! Resultados salvos em 'performance_results.csv'\n");
    return 0;
}