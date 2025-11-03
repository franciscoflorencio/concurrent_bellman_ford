# Implementação Paralela do Algoritmo de Bellman-Ford

Este projeto implementa uma versão paralela do algoritmo de Bellman-Ford para encontrar caminhos mínimos em grafos, utilizando pthreads para paralelização em C.

## Estrutura do Projeto

### Arquivos Principais

- `main.c`: Contém a função principal do programa e implementa diversos casos de teste para validar o algoritmo, incluindo grafos com e sem ciclos negativos, grafos desconexos e grafos de diferentes tamanhos. O arquivo também inclui funções para visualização dos grafos em formato DOT.

- `bellman.c` e `bellman.h`: Implementam o algoritmo de Bellman-Ford tanto em sua versão sequencial quanto paralela. A versão paralela utiliza uma estratégia de double buffering para otimizar o acesso à memória e inclui detecção de ciclos negativos. O arquivo header define estruturas importantes como `t_args` para passar argumentos às threads.

- `barrier.c` e `barrier.h`: Implementam uma barreira de sincronização personalizada usando pthread_mutex e pthread_cond, essencial para sincronizar as threads durante a execução do algoritmo paralelo.

- `graph.h`: Define as estruturas básicas para representação de grafos, incluindo `Edge` para arestas com peso e `Graph` para o grafo completo.

- `performance_test.c`: Arquivo dedicado a testes de desempenho, executando benchmarks com diferentes números de threads e tamanhos de grafos, medindo métricas como speedup e eficiência.

- `test_utils.h`: Contém funções utilitárias para os testes, incluindo geração de grafos aleatórios, medição de tempo e cálculo de métricas de desempenho.

- `Makefile`: Gerencia a compilação do projeto, oferecendo diferentes alvos para compilação, execução de testes e limpeza. Inclui suporte para compilação tanto do programa principal quanto dos testes de desempenho.

## Execução

Para compilar e executar o programa principal:
```bash
make
make run
```

Para executar os testes de desempenho:
```bash
make performance
```

Para limpar os arquivos gerados:
```bash
make clean
```

## Observação para Pull/Merge Requests

Antes de criar um merge request, execute:
```bash
make clean
```

Isso garante que o repositório não contenha artefatos de compilação e evita conflitos de merge com binários gerados.