# Implementação Paralela do Algoritmo de Bellman-Ford

Este projeto implementa uma versão paralela do algoritmo de Bellman-Ford para encontrar caminhos mínimos em grafos, utilizando pthreads para paralelização em C.

## Estrutura do Projeto

### Arquivos Principais

- `main.c`: Contém a função principal do programa e implementa diversos casos de teste para validar o algoritmo, incluindo grafos com e sem ciclos negativos, grafos desconexos e grafos de diferentes tamanhos.

- `bellman.c` e `bellman.h`: Implementam o algoritmo de Bellman-Ford tanto em sua versão sequencial quanto paralela. A versão paralela utiliza uma estratégia de double buffering para otimizar o acesso à memória e inclui detecção de ciclos negativos. O arquivo header define estruturas importantes como `t_args` para passar argumentos às threads.

- `generate.c` e `generate.h`: Responsáveis pela geração e manipulação de grafos de teste, incluindo a criação de diferentes tipos de grafos para validação do algoritmo.

- `print.c` e `print.h`: Implementam funções para visualização e output dos resultados, incluindo a geração de arquivos em formato DOT para visualização dos grafos.

- `graph.h`: Define as estruturas básicas para representação de grafos, incluindo `Edge` para arestas com peso e `Graph` para o grafo completo.

- `performance_test.c`: Arquivo dedicado a testes de desempenho, executando benchmarks com diferentes números de threads e tamanhos de grafos, medindo métricas como speedup e eficiência.

- `test_utils.h`: Contém funções utilitárias para os testes, incluindo medição de tempo e cálculo de métricas de desempenho.

### Estrutura de Diretórios

- `correctness/`: Contém os arquivos .dot gerados para validação da corretude do algoritmo
- `graphs/`: Armazena os arquivos de saída em formato .dot e suas conversões em .png

## Execução

Para compilar e executar o programa principal:
```bash
make
make run
```

O comando `make run` irá:
1. Compilar o programa
2. Converter automaticamente todos os arquivos .dot para .png
3. Executar o programa principal

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