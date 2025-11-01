# ==============================================================================
# Variáveis e Configurações
# ==============================================================================
CC = gcc
# CFLAGS contém flags de warning e a flag de thread (-pthread).
# É crucial manter -pthread aqui para a compilação de .c que usam pthreads (ex: barrier.c, bellman.c)
CFLAGS = -Wall -Wextra -pthread
# LDFLAGS é usado para flags de linkagem (bibliotecas)
LDFLAGS = -pthread -lm

TARGET = out
PERF_TARGET = perf_test

SRCS = main.c bellman.c barrier.c
PERF_SRCS = performance_test.c bellman.c barrier.c

OBJS = $(SRCS:.c=.o)
PERF_OBJS = $(PERF_SRCS:.c=.o)

# ==============================================================================
# Alvos Principais
# ==============================================================================

# Alvo padrão: constrói o executável principal
all: $(TARGET)

# Linkagem do Executável Principal
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Linkagem do Executável de Performance
$(PERF_TARGET): $(PERF_OBJS)
	$(CC) $(PERF_OBJS) -o $@ $(LDFLAGS)

# Regra Genérica de Compilação (compila .c para .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ==============================================================================
# Alvos de Execução
# ==============================================================================

run: $(TARGET)
	@echo "Executando $(TARGET)..."
	@mkdir -p graphs # Garante que o diretório 'graphs' exista
	@./$(TARGET)

run-args: $(TARGET)
	@echo "Executando $(TARGET) com argumentos: $(ARGS)..."
	@mkdir -p graphs
	@./$(TARGET) $(ARGS)

test: $(TARGET)
	@echo "Executando teste básico..."
	@mkdir -p graphs
	@./$(TARGET)

performance: $(PERF_TARGET)
	@echo "Executando testes de desempenho..."
	@mkdir -p metrics # Garante que o diretório 'metrics' exista
	@./$(PERF_TARGET) | tee metrics/performance_results.txt

check: test

# ==============================================================================
# Alvos de Limpeza
# ==============================================================================
clean:
	@echo "Limpando arquivos gerados..."
	rm -f $(OBJS) $(PERF_OBJS) $(TARGET) $(PERF_TARGET) metrics/performance_results.txt
	rm -f  correctness/*.dot
	rm -f *.dot
	rm -f *.png
	rm -rf graphs metrics

.PHONY: all clean run run-args test performance check